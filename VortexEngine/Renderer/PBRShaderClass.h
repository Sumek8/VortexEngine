#pragma once
#pragma comment(lib, "d3dcompiler.lib")



#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "Math.h"

using namespace DirectX;
using namespace std;

class PBRShaderClass
{
public:
	PBRShaderClass();
	~PBRShaderClass();

private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightprojection;
		XMFLOAT3 CameraPosition;
	};
	struct ConstantBufferType
	{
		XMFLOAT3 LightDirection;
		XMFLOAT4 DiffuseColor;
		float padding;
	};



public:
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMMATRIX& lightprojectionMatrix, XMMATRIX& lightViewMatrix,int indexCount, XMFLOAT3& lightDirection, XMFLOAT4& diffuseColor, ID3D11ShaderResourceView* BaseColorMap, ID3D11ShaderResourceView* NormalMap, ID3D11ShaderResourceView* ShadowMap, ID3D11ShaderResourceView* CubeMap, XMFLOAT3& CameraPosition);
bool Initialize(ID3D11Device*, HWND);
bool InitializePostProcessShader(ID3D11Device*);
	void Shutdown();
	bool RenderLightPass(ID3D11DeviceContext* deviceContext,ID3D11ShaderResourceView*GBufferA, ID3D11ShaderResourceView*GBufferB, ID3D11ShaderResourceView*GBufferC, ID3D11ShaderResourceView*GBufferD, ID3D11ShaderResourceView* ShadowMap, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMMATRIX& lightProjectionMatrix, XMMATRIX& lightViewMatrix, XMFLOAT3& lightDirection, XMFLOAT4& diffuseColor, XMFLOAT3& CameraPosition);
	bool RenderPostProcess(ID3D11DeviceContext* deviceContext,ID3D11ShaderResourceView*SceneColor);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	
	void RenderShader(ID3D11DeviceContext*, int);
	ID3D11Resource* textureSample;
	ID3D11Texture2D* shadowMap;
	

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;

	ID3D11VertexShader* DeferredVertexShader;
	ID3D11PixelShader* DeferredPixelShader;


	ID3D11VertexShader* PostProcessVertexShader;
	ID3D11PixelShader* PostProcessPixelShader;


	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11Buffer* shadowBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Device* D3DDevice;
};

