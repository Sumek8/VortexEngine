#include "stdafx.h"
#include "PBRShaderClass.h"
#include <vector>

PBRShaderClass::PBRShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_constantBuffer = 0;
	m_sampleStateClamp = 0;
}


PBRShaderClass::~PBRShaderClass()
{
}

bool PBRShaderClass::Initialize(ID3D11Device* device,HWND hwnd)
{
	bool result;

	D3DDevice = device;
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"Shaders/DeferredVertexShader.fx", L"Shaders/DeferredPixelShader.fx");
	if (!result)
	{
		return false;
	}


	
	result = InitializePostProcessShader(device);
		if (!result)
		{
			return false;
		}
		
	return true;
}
void PBRShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}
bool PBRShaderClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,int indexCount,ID3D11ShaderResourceView* BaseColorMap, ID3D11ShaderResourceView* NormalMap, ID3D11ShaderResourceView* ShadowMap, ID3D11ShaderResourceView* CubeMap)
{


	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType*		 dataPtr;
	unsigned int			 bufferNumber;
	SkeletonBufferType*		     SkeletonBufferData;

	//	Make sure to transpose matrices before sending them into the shader, this is a requirement for DirectX 11.

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX TworldMatrix = XMMatrixTranspose(worldMatrix);
	XMMATRIX TviewMatrix = XMMatrixTranspose(viewMatrix);
	XMMATRIX TprojectionMatrix = XMMatrixTranspose(projectionMatrix);


		hr = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return false;
	}

	
	// Get a pointer to the data in the constant buffer.


	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = TworldMatrix;
	dataPtr->view = TviewMatrix;
	dataPtr->projection = TprojectionMatrix;



	deviceContext->Unmap(m_matrixBuffer, 0);

	//SkinnedShader
	
	deviceContext->Map(SkeletonMatrixBuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	SkeletonBufferData = (SkeletonBufferType*)mappedResource.pData;
	
	
	for (size_t i = 0; i < 255; i++)
	{
	
		SkeletonBufferData->TransformMatrices[i] = TworldMatrix;
	}
	
	
	deviceContext->Unmap(SkeletonMatrixBuffer,0);
	


	bufferNumber = 0;


	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->VSSetConstantBuffers(bufferNumber+1, 1, &SkeletonMatrixBuffer);

	deviceContext->PSSetShaderResources(0, 1, &BaseColorMap);
	deviceContext->PSSetShaderResources(1, 1, &NormalMap);
	deviceContext->PSSetShaderResources(2, 1, &ShadowMap);
	deviceContext->PSSetShaderResources(3, 1, &CubeMap);
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);


	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

void PBRShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);


				//SkinnedShader
	//deviceContext->VSSetShader(DeferredSkinnedVertexShader, NULL, 0);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);

	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	//deviceContext->DrawIndexedInstanced(indexCount,2000,0,0,0);


	return;
}

bool PBRShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* DeferredVertexShaderBuffer;
	ID3D10Blob* DeferredPixelShaderBuffer;
	ID3D10Blob* DeferredSkinnedVertexShaderBuffer;

	//Shader Input Types
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc,cbDesc,SkeletonBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	ConstantBufferType  ConstantBuffer;
	
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	DeferredSkinnedVertexShaderBuffer = 0;
	
	
	result = D3DCompileFromFile(vsFilename, NULL, NULL,"VertexShaderFunction", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
					// Set 0 or 1 for True or false

	char* bColorMap = "1";
	char* bEmission = "0";

	D3D_SHADER_MACRO Shader_Macros[] = {
		"BASECOLORMAP",bColorMap,
		"EMISSION",bEmission,
		"NORMALMAP","1",
		
	nullptr, nullptr };

	// Compile the pixel shader code.
	
	result = D3DCompileFromFile(psFilename,Shader_Macros, NULL, "PixelShaderFunction", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	
	result = D3DCompileFromFile(L"Shaders/DeferredBaseShading.fx", NULL, NULL, "VertexShaderFunction", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&DeferredVertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			
			OutputShaderErrorMessage(errorMessage, hwnd, L"Shaders/DeferredBaseShading.fx");
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, L"Shaders/DeferredBaseShading.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(L"Shaders/DeferredBaseShading.fx",NULL, NULL, "PixelShaderFunction", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&DeferredPixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"Shaders/DeferredBaseShading.fx");
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, L"Shaders/DeferredBaseShading.fx", L"Missing Shader File", MB_OK);
		}

		return false;
	}
	
	result = D3DCompileFromFile(L"Shaders/DeferredSkinnedVertexShader.fx", Shader_Macros, NULL, "VertexShaderFunction", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&DeferredSkinnedVertexShaderBuffer, &errorMessage);

	result = device->CreateVertexShader(DeferredSkinnedVertexShaderBuffer->GetBufferPointer(),DeferredSkinnedVertexShaderBuffer->GetBufferSize(), NULL, &DeferredSkinnedVertexShader);
	if (FAILED(result))
	{
		return false;
	}

		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	
	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}


	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(DeferredVertexShaderBuffer->GetBufferPointer(), DeferredVertexShaderBuffer->GetBufferSize(), NULL, &DeferredVertexShader);
	if (FAILED(result))
	{
		//MessageBox(hwnd, L"Error", L"Error", MB_OK);
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(DeferredPixelShaderBuffer->GetBufferPointer(), DeferredPixelShaderBuffer->GetBufferSize(), NULL, &DeferredPixelShader);
	if (FAILED(result))
	{
		return false;
	}


	

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;


	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;



	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	DeferredSkinnedVertexShaderBuffer->Release();
	DeferredSkinnedVertexShaderBuffer = 0;

	errorMessage->Release();
	errorMessage = 0;

	DeferredVertexShaderBuffer->Release();
	DeferredVertexShaderBuffer = 0;

	DeferredPixelShaderBuffer->Release();
	DeferredPixelShaderBuffer = 0;



	
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = sizeof(ConstantBufferType);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &ConstantBuffer;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;


	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	


	SkeletonBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	SkeletonBufferDesc.ByteWidth = sizeof(SkeletonBufferType);
	SkeletonBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	SkeletonBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	SkeletonBufferDesc.MiscFlags = 0;
	SkeletonBufferDesc.StructureByteStride = 0;



	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Create a clamp texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	
	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);
	if (FAILED(result))
	{	
	
		return false;
	}

	

	result = device->CreateBuffer(&cbDesc, &InitData,&m_constantBuffer);

	//CreateSkeletonBuffer
	result = device->CreateBuffer(&SkeletonBufferDesc,NULL, &SkeletonMatrixBuffer);

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;

	
}

bool PBRShaderClass::InitializePostProcessShader(ID3D11Device* device)
{
	
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;


	result = D3DCompileFromFile(L"Shaders/PostProcessBase.fx", NULL, NULL, "VertexShaderFunction", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{

		return false;
	}

	result = D3DCompileFromFile(L"Shaders/PostProcessBase.fx", NULL, NULL, "PixelShaderFunction", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		return false;
	}


	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &PostProcessVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &PostProcessPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	pixelShaderBuffer->Release();
	vertexShaderBuffer->Release();
	


	return true;
}

void PBRShaderClass::ShutdownShader()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}


	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	if (DeferredPixelShader)
	{
		DeferredPixelShader->Release();
		DeferredPixelShader = 0;
	}

	if (DeferredVertexShader)
	{
		DeferredVertexShader->Release();
		DeferredVertexShader = 0;

	}
	if (m_constantBuffer)
	{
		m_constantBuffer->Release();
		m_constantBuffer = 0;
	}
	if (SkeletonMatrixBuffer)
	{
		SkeletonMatrixBuffer->Release();
		SkeletonMatrixBuffer = 0;
	}
	

	if (DeferredSkinnedVertexShader)
	{
		DeferredSkinnedVertexShader->Release();
		DeferredSkinnedVertexShader = 0;

	}

	return;
}

void PBRShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("Error/shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}
//The SetShaderVariables function exists to make setting the global variables in the shader easier.The matrices used in this function are created inside the GraphicsClass, after which this function is called to send them from there into the vertex shader during the Render function call.





bool PBRShaderClass::RenderPostProcess(ID3D11DeviceContext* deviceContext,ID3D11ShaderResourceView*SceneColor)
{

	deviceContext->PSSetShaderResources(0, 1, &SceneColor);
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	

	deviceContext->VSSetShader(PostProcessVertexShader, NULL, 0);
	deviceContext->PSSetShader(PostProcessPixelShader, NULL, 0);

	deviceContext->Draw(4, 0);

	return true;
}


bool PBRShaderClass::RenderDeferredLightPass(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** GBufferPointer, ID3D11ShaderResourceView* ShadowMap,XMMATRIX& worldMatrix,XMMATRIX& viewMatrix,XMMATRIX& projectionMatrix,XMMATRIX& lightProjectionMatrix,XMMATRIX& lightViewMatrix,VRotation& lightDirection, VColor& LightColor,VVector& CameraDirection,float LightIntensity)
{

	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType*		 dataPtr;
	ConstantBufferType*		 dataPtr2;
	unsigned int			 bufferNumber;

	hr = deviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{

		return false;
	}



	dataPtr2 = (ConstantBufferType*)mappedResource.pData;

	dataPtr2->LightColor = LightColor;
	dataPtr2->LightDirection = RotationToVector(lightDirection);
	dataPtr2->LightPower = LightIntensity;
	dataPtr2->CameraDirection = CameraDirection;	
	dataPtr2->Padding = 0;
	deviceContext->Unmap(m_constantBuffer, 0);

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX TworldMatrix = XMMatrixTranspose(worldMatrix);
	XMMATRIX TviewMatrix = XMMatrixTranspose(viewMatrix);
	XMMATRIX TprojectionMatrix = XMMatrixTranspose(projectionMatrix);
	XMMATRIX TInverseProjectionMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr,projectionMatrix));
	XMMATRIX TlightprojectionMatrix = XMMatrixTranspose(lightProjectionMatrix);
	XMMATRIX TlightViewMatrix = XMMatrixTranspose(lightViewMatrix);
	hr = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;



	// Copy the matrices into the constant buffer.
	dataPtr->world = TworldMatrix;
	dataPtr->view = TviewMatrix;
	dataPtr->projection = TprojectionMatrix;
	dataPtr->InverseProjection = TInverseProjectionMatrix;
	dataPtr->lightprojection = TlightprojectionMatrix;
	dataPtr->lightView = TlightViewMatrix;
	



	deviceContext->Unmap(m_matrixBuffer, 0);


	bufferNumber = 0;

	deviceContext->PSSetShaderResources(0,4,GBufferPointer);
	deviceContext->PSSetShaderResources(4, 1, &ShadowMap);

	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	deviceContext->VSSetShader(DeferredVertexShader, NULL, 0);
	deviceContext->PSSetShader(DeferredPixelShader, NULL, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &m_matrixBuffer);
	deviceContext->PSSetConstantBuffers(1, 1, &m_constantBuffer);
	

	deviceContext->Draw(4, 0);
	
	return true;

}