#include "stdafx.h"
#include "PostProcessShader.h"


PostProcessShader::PostProcessShader()
{
}



bool PostProcessShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	D3DDevice = device;
	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"Shaders/PostProcessShader.fx", L"Shaders/PostProcessShader.fx");
	if (!result)
	{
		return false;
	}

	return true;
}



void PostProcessShader::RenderShader(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* Texture)
{
	// Set the vertex and pixel shaders that will be used to render this triangle.


	deviceContext->VSSetShader(VVertexShader, NULL, 0);
	deviceContext->PSSetShader(VPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &TextureSampler);
	deviceContext->PSSetShaderResources(0, 1, &Texture);

	// Render the triangle.
	deviceContext->Draw(4, 0);


	return;
}


bool PostProcessShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;


	D3D11_SAMPLER_DESC SamplerDesc;

	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&SamplerDesc, &TextureSampler);

	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "PostProcessFunctionVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
	
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
	
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "PostProcessFunctionPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &VVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &VPixelShader);
	if (FAILED(result))
	{
		return false;
	}



	
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	return true;
}

void PostProcessShader::ShutdownShader()
{
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (TextureSampler)
	{
		TextureSampler->Release();
		TextureSampler = 0;
	}

	if (VPixelShader)
	{
		VPixelShader->Release();
		VPixelShader = 0;
	}

	if (VVertexShader)
	{
		VVertexShader->Release();
		VVertexShader = 0;
	}



	return;
}

PostProcessShader::~PostProcessShader()
{
}
