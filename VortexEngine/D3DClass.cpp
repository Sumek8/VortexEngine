////////////////////////////////////////////////////////////////////////////////
// Filename: D3DClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "D3DClass.h"

using namespace DirectX;
D3DClass::D3DClass()
{

	VDevice = 0;
	VDeviceContext = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
	pShadowMapDepthView = 0;
	pShadowMapSRView = 0;
	pShadowMap = 0;
	BlurScale = 16;

}


D3DClass::D3DClass(const D3DClass& other)
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::CreateSwapChain(HWND hwnd)
{
	IDXGIFactory* factory;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	HRESULT	result;
	ID3D11Texture2D* backBuffer;
	IDXGISwapChain*  SwapChain;
	ID3D11RenderTargetView* RenderTargetView;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	SwapChainArray[0]->GetDesc(&swapChainDesc);
	swapChainDesc.OutputWindow = hwnd;
	
	result = factory->CreateSwapChain(VDevice, &swapChainDesc,&SwapChain);
	if (FAILED(result))
	{
		return false;
	}



	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		return false;
	}
	SwapChainArray.push_back(SwapChain);
	SwapChainCount += 1;

	SwapChain->Release();
	SwapChain = 0;

	// Create the render target view with the back buffer pointer.
	result = VDevice->CreateRenderTargetView(backBuffer, NULL, &RenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	WindowRenderTarget.push_back(RenderTargetView);
	
	
	backBuffer->Release();
	backBuffer = 0;


	factory->Release();
	factory = 0;

	return true;
	
}

int D3DClass::GetSwapChainCount()
{

	return SwapChainCount;
}


bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	IDXGISwapChain*  SwapChain;
	unsigned int numModes, i, numerator, denominator;
	//size_t  stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RenderTargetView* RenderTargetView;


	ScreenWidth = 1366;
	ScreenHeight = 768;

	//ScreenWidth = 1920;
	//ScreenHeight = 1080;


	float fieldOfView, screenAspect;
	
	RenderTargetView = 0;

	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}


	
	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	VideoCardName = adapterDesc.Description;
	// Convert the name of the video card to a character array and store it.
	

	
	
	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;


	BackBufferSize[0] = ScreenWidth;
	BackBufferSize[1] = ScreenHeight;
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = ScreenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;
	
	

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;
	
		// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_1;
	
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &VDevice,NULL, &VDeviceContext);
	if (FAILED(result))
	{
		return false;
	}
	SwapChainArray.push_back(SwapChain);

	SwapChainCount += 1;

	factory->Release();
	factory = 0;


	// Get the pointer to the back buffer.
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = VDevice->CreateRenderTargetView(backBufferPtr, NULL, &RenderTargetView);
	if (FAILED(result))
	{
		return false;
	}
	
	WindowRenderTarget.push_back(RenderTargetView);
	
	
	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;
	
	SwapChain->Release();
	SwapChain = 0;

	


	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = ScreenWidth;
	depthBufferDesc.Height = ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	depthBufferDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;
	depthBufferDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;


	if (FAILED(VDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer))) return false;
	

	


	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = VDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}


	// Set the depth stencil state.
	VDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	

	// Create the depth stencil view.
	result = VDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}



	// Bind the render target view and depth stencil buffer to the output render pipeline.
	VDeviceContext->OMSetRenderTargets(1, &WindowRenderTarget[0], m_depthStencilView);

	// Setup the viewport for rendering.
	viewport.Width = (float)ScreenWidth;
	viewport.Height = (float)ScreenHeight;
	
	
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.


	textureviewport.Width = 512.0f;
	textureviewport.Height = 512.0f;

	textureviewport.MinDepth = 0.0f;
	textureviewport.MaxDepth = 1.0f;
	textureviewport.TopLeftX = 0.0f;
	textureviewport.TopLeftY = 0.0f;

	BlurViewport.Width = (float)ScreenWidth/BlurScale;
	BlurViewport.Height = (float)ScreenHeight/BlurScale;

	BlurViewport.MinDepth = 0.0f;
	BlurViewport.MaxDepth = 1.0f;
	BlurViewport.TopLeftX = 0.0f;
	BlurViewport.TopLeftY = 0.0f;


	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	ZeroMemory(&texDesc, sizeof(texDesc));

	texDesc.Width = int(textureviewport.Width);
	texDesc.Height = int(textureviewport.Height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;
	texDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;


	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;


	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;


	if (FAILED(VDevice->CreateTexture2D(&texDesc, NULL, &pShadowMap)))  return false;

	if (FAILED(VDevice->CreateDepthStencilView(pShadowMap, &descDSV, &pShadowMapDepthView)))return false;

	if (FAILED(VDevice->CreateShaderResourceView(pShadowMap, &srvDesc, &pShadowMapSRView))) return false;


	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	
	textureDesc.Width = int(ScreenWidth);
	textureDesc.Height = int(ScreenHeight);

	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = swapChainDesc.SampleDesc.Count;
	textureDesc.SampleDesc.Quality = swapChainDesc.SampleDesc.Quality;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC GBufferRTViewDesc;
	ZeroMemory(&GBufferRTViewDesc, sizeof(GBufferRTViewDesc));
	GBufferRTViewDesc.Format = textureDesc.Format;
	GBufferRTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	GBufferRTViewDesc.Texture2D.MipSlice = 0;

	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = textureDesc.Format;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	
	

	for (int i = 0; i < BufferCount; i++)
	{
		if (FAILED(VDevice->CreateTexture2D(&textureDesc, NULL, &GBuffer[i])))  return false;	
	}
	for (int i = 0; i < BufferCount; i++)
	{		
		if (FAILED(VDevice->CreateRenderTargetView(GBuffer[i], &GBufferRTViewDesc, &GBufferRTView[i])))  return false;
	}
	for (int i = 0; i < BufferCount; i++)
	{
		if (FAILED(VDevice->CreateShaderResourceView(GBuffer[i], &SRVDesc, &GBufferSRView[i]))) return false;
	}


	for (int i = 0; i < PostProcessBuffers; i++)
	{
		if (FAILED(VDevice->CreateTexture2D(&textureDesc, NULL, &PostProcessBuffer[i])))  return false;
	}
	for (int i = 0; i < PostProcessBuffers; i++)
	{
		if (FAILED(VDevice->CreateRenderTargetView(PostProcessBuffer[i], &GBufferRTViewDesc, &PostProcessRTV[i])))  return false;
	}
	for (int i = 0; i < PostProcessBuffers; i++)
	{
		if (FAILED(VDevice->CreateShaderResourceView(PostProcessBuffer[i], &SRVDesc, &PostProcessSRV[i]))) return false;
	}

	//Blur

	textureDesc.Width = int(ScreenWidth/BlurScale);
	textureDesc.Height = int(ScreenHeight/BlurScale);

	D3D11_RENDER_TARGET_VIEW_DESC BlurRTViewDesc;
	ZeroMemory(&BlurRTViewDesc, sizeof(BlurRTViewDesc));
	BlurRTViewDesc.Format = textureDesc.Format;
	BlurRTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	BlurRTViewDesc.Texture2D.MipSlice = 0;


	D3D11_SHADER_RESOURCE_VIEW_DESC BlurSRVDesc;
	ZeroMemory(&BlurSRVDesc, sizeof(BlurSRVDesc));
	BlurSRVDesc.Format = textureDesc.Format;
	BlurSRVDesc.Texture2D.MipLevels = 1;
	BlurSRVDesc.Texture2D.MostDetailedMip = 0;
	BlurSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;


	if (FAILED(VDevice->CreateTexture2D(&textureDesc, NULL, &BlurPostProcessBuffer)))  return false;
	if (FAILED(VDevice->CreateRenderTargetView(BlurPostProcessBuffer, &BlurRTViewDesc, &BlurPostProcessRTV)))  return false;
	if (FAILED(VDevice->CreateShaderResourceView(BlurPostProcessBuffer, &BlurSRVDesc, &BlurPostProcessSRV))) return false;
	



//////////Blend State Description////////////////////////
/*	 D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	BlendState.RenderTarget[0].BlendEnable = false;

	
	VDevice->CreateBlendState(&BlendState, &VBlendState);
	*/
	//////////Rasterizer Description////////////////////////
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = VDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	VDeviceContext->RSSetState(m_rasterState);

	VDeviceContext->RSSetViewports(1,&viewport);

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)ScreenWidth / (float)ScreenHeight;
	
	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
	

	return true;
}
void D3DClass::SetWireframeMode(bool bSetWireframe)
{

	D3D11_RASTERIZER_DESC rasterDesc;
	
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;

	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (bSetWireframe)
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	else 
		rasterDesc.FillMode = D3D11_FILL_SOLID;

	
	
	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}
	HRESULT result = VDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);
	VDeviceContext->RSSetState(m_rasterState);

}

void D3DClass::RemoveSwapChain(int ID)
{
	
	SwapChainArray[ID]->Release();
	SwapChainArray[ID] = 0;
	SwapChainArray.erase(SwapChainArray.begin() + ID);
	SwapChainCount -= 1;


	WindowRenderTarget[ID]->Release();
	WindowRenderTarget[ID] = 0;
	WindowRenderTarget.erase(WindowRenderTarget.begin() + ID);
	

}

float D3DClass::GetPixelDepth(int mouseX, int mouseY)
{

	ID3D11Texture2D* textureBuf;
	D3D11_TEXTURE2D_DESC textureDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = ScreenWidth;
	textureDesc.Height = ScreenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.MiscFlags = 0;
	VDevice->CreateTexture2D(&textureDesc, NULL, &textureBuf);

	
	//GetPixelDepth
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	VDeviceContext->CopyResource(textureBuf, m_depthStencilBuffer);
	VDeviceContext->Map(textureBuf, 0, D3D11_MAP_READ, NULL, &MappedResource);


	
	unsigned int* Color  = (unsigned int*)MappedResource.pData;
	float Depth;
	Depth = 0;
	if (mouseX > 0 && mouseX < ScreenWidth && mouseY>0 && mouseY < ScreenHeight)
	{
		Depth = float(Color[(mouseY*((MappedResource.RowPitch) / 4) + mouseX)]);
		Depth /= 16777216.0f;

		Depth -= 256;
	}
		//depth /= 16777216.0f; // divide bei 2^24
		
		
	VDeviceContext->Unmap(textureBuf, 0);
	textureBuf->Release();

	return Depth;
}


void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (0 < SwapChainCount)
		for (size_t i = 0; i <SwapChainCount; i++)
		{
			SwapChainArray[i]->SetFullscreenState(false, NULL);
		}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	
	if (VDeviceContext)
	{
		VDeviceContext->Release();
		VDeviceContext = 0;
	}

	if (VDevice)
	{
		VDevice->Release();
		VDevice = 0;
	}

	
	if( 0 < SwapChainCount)
	for (size_t i = 0; i <SwapChainCount; i++)
	{
		SwapChainArray[i]->Release();
		SwapChainArray[i] = 0;
	}
	SwapChainArray.clear();
		
	if (0 < WindowRenderTarget.size())
		for (size_t i = 0; i <WindowRenderTarget.size(); i++)
		{
			WindowRenderTarget[i]->Release();
			WindowRenderTarget[i] = 0;
		}
	WindowRenderTarget.clear();

	


	if (pShadowMapDepthView)
	{
		pShadowMapDepthView->Release();
		pShadowMapDepthView = 0;
	}

	if (pShadowMap)
	{
		pShadowMap->Release();
		pShadowMap = 0;
	}
	
	if(pShadowMapSRView)
	{
		pShadowMapSRView->Release();
		pShadowMapSRView = 0;
	}
	for (int i = 0; i < BufferCount; i++)
	{
		if (GBuffer[i])
		{
			GBuffer[i]->Release();
			GBuffer[i] = 0;
		}
		if (GBufferRTView[i])
		{
			GBufferRTView[i]->Release();
			GBufferRTView[i] = 0;
		}
		if (GBufferSRView[i])
		{
			GBufferSRView[i]->Release();
			GBufferSRView[i] = 0;
		}
	}

	for (int i = 0; i < PostProcessBuffers; i++)
	{
		if (PostProcessBuffer[i])
		{
			PostProcessBuffer[i]->Release();
			PostProcessBuffer[i] = 0;
		}
		if (PostProcessRTV[i])
		{
			PostProcessRTV[i]->Release();
			PostProcessRTV[i] = 0;
		}
		if (PostProcessSRV[i])
		{
			PostProcessSRV[i]->Release();
			PostProcessSRV[i] = 0;
		}
	}
		if(BlurPostProcessBuffer)
		{
		BlurPostProcessBuffer->Release();
		BlurPostProcessBuffer = 0;
		}
		if (BlurPostProcessRTV)
		{
			BlurPostProcessRTV->Release();
			BlurPostProcessRTV = 0;
		}
		if (BlurPostProcessSRV)
		{
			BlurPostProcessSRV->Release();
			BlurPostProcessSRV = 0;
		}

	if (VBlendState)
	{
		VBlendState->Release();
		VBlendState = 0;
	}



	return;
}


void D3DClass::SetDeferredRenderTarget()
{

	VDeviceContext->OMSetRenderTargets(1, &PostProcessRTV[0],nullptr);
	
	return;
}

void D3DClass::SetPostProcessRenderTarget(int PostProcessTarget)
{
	VDeviceContext->RSSetViewports(1, &viewport);
	VDeviceContext->OMSetRenderTargets(1,&PostProcessRTV[PostProcessTarget], nullptr);
}


void D3DClass::SetBlurRenderTarget()
{
	VDeviceContext->RSSetViewports(1, &BlurViewport);
	VDeviceContext->OMSetRenderTargets(1, &BlurPostProcessRTV, nullptr);
}

ID3D11ShaderResourceView* D3DClass::GetBlurResourceView()
{
	return BlurPostProcessSRV;
}

void D3DClass::SetWindowRenderTarget(int ID)
{
	float color[4];
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
	color[3] = 1;
	
	VDeviceContext->ClearRenderTargetView(WindowRenderTarget[ID], color);
	VDeviceContext->OMSetRenderTargets(1, &WindowRenderTarget[ID], nullptr);

}


void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	


	VDeviceContext->RSSetViewports(1, &viewport);
	
	return;
}
void D3DClass::SetGBufferRenderTarget()
{

	float color[4];

	
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
	color[3] = 0;

	for (int i = 0; i<BufferCount; i++)
		VDeviceContext->ClearRenderTargetView(GBufferRTView[i], color);

	VDeviceContext->OMSetRenderTargets(BufferCount, GBufferRTView, m_depthStencilView);
	VDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	

}

void D3DClass::ClearBlendState()
{
	
	VDeviceContext->OMSetBlendState(NULL, 0, 0xffffffff);

}
ID3D11ShaderResourceView* D3DClass::GetPostProcessResourceView(int view)
{
	return PostProcessSRV[view];
}

ID3D11ShaderResourceView* D3DClass::GetShaderResourceView(int view)
{
	return GBufferSRView[view];
}

void D3DClass::RenderGizmo()
{
	
	VDeviceContext->OMSetRenderTargets(1,&PostProcessRTV[0],0);	

	return;
}

int D3DClass::GetBackBufferSize(int Resolution)
{
	if (Resolution > 1)
	return BackBufferSize[1];
	return BackBufferSize[Resolution];
}

void D3DClass::RenderShadows()
{

	VDeviceContext->RSSetViewports(1,&textureviewport);

	VDeviceContext->OMSetRenderTargets(0,0, pShadowMapDepthView);
	
	VDeviceContext->ClearDepthStencilView(pShadowMapDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}


void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		for (size_t i = 0; i < SwapChainArray.size(); i++)
		{
			SwapChainArray[i]->Present(1, 0);
		}		
	}
	else
	{
		// Present as fast as possible.
		for (size_t i = 0; i < SwapChainArray.size(); i++)
		{
			SwapChainArray[i]->Present(0, 0);
		}
	}



	return;
}



ID3D11Device* D3DClass::GetDevice()
{
	return VDevice;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return VDeviceContext;
}

ID3D11ShaderResourceView** D3DClass::GetGBufferResource()
{

	return GBufferSRView;

}
void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix,float ViewportAspect)
{

	float fieldOfView = 3.141592654f / 4.0f;
	
	float screenAspect = ViewportAspect;
	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 1, 1000);



	projectionMatrix = m_projectionMatrix;
	return;
}


void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

IDXGISwapChain* D3DClass::GetSwapChain(int ID)
{
	return SwapChainArray[ID];

}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}
