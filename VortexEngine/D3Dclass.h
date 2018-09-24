

#pragma once

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "D3Dcompiler.lib")
#include <vector>
#include <D3D11.h>
#include <directxmath.h>
#include <dxgi1_2.h>
#include "Math.h"


const int BufferCount = 4;
const int PostProcessBuffers = 2;

using namespace std;

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void RenderShadows();
	void BeginScene(float, float, float, float);
	void EndScene();
	void ClearBlendState();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain(int ID);

	bool  CreateSwapChain(HWND hwnd);
	void  GetProjectionMatrix(XMMATRIX&,float ViewportAspect);
	void  GetWorldMatrix(XMMATRIX&);
	void  GetOrthoMatrix(XMMATRIX&);
	void  SetDeferredRenderTarget();
	void  RenderGizmo();	
	void  GetVideoCardInfo(char*, int&);
	void  SetPostProcessRenderTarget(int PostProcessTarget);
	void  SetGBufferRenderTarget();
	void SetBlurRenderTarget();
	ID3D11ShaderResourceView * GetBlurResourceView();
	void  SetWindowRenderTarget(int ID);
	float GetPixelDepth(int mouseX, int mouseY);
	void  SetWireframeMode(bool SetWireframe);
	int   GetSwapChainCount();
	void  RemoveSwapChain(int ID);
	int   GetBackBufferSize(int Resolution);
	ID3D11ShaderResourceView** GetGBufferResource();
	ID3D11ShaderResourceView* GetShaderResourceView(int view);
	ID3D11ShaderResourceView* GetPostProcessResourceView(int view);

	vector<ID3D11RenderTargetView*>	WindowRenderTarget;
	ID3D11RenderTargetView*	  m_renderTargetView;


	ID3D11Texture2D*		  pShadowMap;
	ID3D11ShaderResourceView* pShadowMapSRView;

	ID3D11BlendState*		  VBlendState;

						  
private:
	int			BackBufferSize[2];
	bool		m_vsync_enabled;
	int			m_videoCardMemory;
	char	    m_videoCardDescription[128];

	vector<IDXGISwapChain*> SwapChainArray;
	ID3D11Device* VDevice;
	ID3D11DeviceContext* VDeviceContext;
	
	
	
	ID3D11DepthStencilState* RTDepthStencilState;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11DepthStencilView* pShadowMapDepthView;
	

	
	ID3D11Texture2D* GBuffer[BufferCount];
	ID3D11RenderTargetView* GBufferRTView[BufferCount];
	ID3D11ShaderResourceView* GBufferSRView[BufferCount];

	ID3D11Texture2D* PostProcessBuffer[PostProcessBuffers];
	ID3D11RenderTargetView* PostProcessRTV[PostProcessBuffers];
	ID3D11ShaderResourceView* PostProcessSRV[PostProcessBuffers];

	ID3D11Texture2D* BlurPostProcessBuffer;
	ID3D11RenderTargetView* BlurPostProcessRTV;
	ID3D11ShaderResourceView* BlurPostProcessSRV;

	D3D11_VIEWPORT viewport, textureviewport,BlurViewport;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	int ScreenWidth, ScreenHeight;
	wstring VideoCardName;
	int SwapChainCount;
	int BlurScale;
};

