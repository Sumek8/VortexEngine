#pragma once
#pragma comment(lib, "d3dcompiler.lib")



#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
#include "Math.h"
#include <vector>

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
		XMMATRIX InverseProjection;
		XMMATRIX lightView;
		XMMATRIX lightprojection;
		
	};
	struct ConstantBufferType
	{
		VColor  LightColor;
		VVector LightDirection;
		float   LightPower;
		VVector CameraDirection;		
		float   Padding;
	};

	struct SkeletonBufferType
	{
		 XMMATRIX TransformMatrices[255];
	};



public:
	bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,int indexCount, ID3D11ShaderResourceView* BaseColorMap, ID3D11ShaderResourceView* NormalMap, ID3D11ShaderResourceView* ShadowMap, ID3D11ShaderResourceView* CubeMap);
bool Initialize(ID3D11Device*, HWND);
bool InitializePostProcessShader(ID3D11Device*);
	void Shutdown();

	bool RenderDeferredLightPass(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** GBufferPointer, ID3D11ShaderResourceView* ShadowMap, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,XMMATRIX& lightProjectionMatrix, XMMATRIX& lightViewMatrix,VRotation& lightDirection, VColor& diffuseColor, VVector& CameraPosition,float LightIntensity);
	
	bool RenderPostProcess(ID3D11DeviceContext* deviceContext,ID3D11ShaderResourceView*SceneColor, ID3D11ShaderResourceView*CustomTexture);

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

	ID3D11VertexShader* DeferredSkinnedVertexShader;

	ID3D11VertexShader* DeferredVertexShader;
	ID3D11PixelShader* DeferredPixelShader;


	ID3D11VertexShader* PostProcessVertexShader;
	ID3D11PixelShader* PostProcessPixelShader;

	ID3D11VertexShader* AOPostProcessVertexShader;
	ID3D11PixelShader* AOPostProcessPixelShader;


	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_constantBuffer;
	ID3D11Buffer* shadowBuffer;
	ID3D11Buffer* SkeletonMatrixBuffer;

	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Device* D3DDevice;
};

