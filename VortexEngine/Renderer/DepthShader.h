#pragma once
#include <d3d9.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX lightView;
	XMMATRIX lightProjection;
};

class DepthShader
{
public:
	DepthShader();
	~DepthShader();
bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& lightprojectionMatrix, XMMATRIX& lightViewMatrix, int indexCount);
void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

ID3D11VertexShader* m_vertexShader;
ID3D11PixelShader* m_pixelShader;
ID3D11InputLayout* m_layout;
ID3D11Buffer* m_matrixBuffer;
ID3D11Buffer* m_constantBuffer;
ID3D11Buffer* shadowBuffer;
ID3D11SamplerState* m_sampleState;
ID3D11SamplerState* m_sampleStateClamp;
ID3D11Device* D3DDevice;
bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
void ShutdownShader();
bool Initialize(ID3D11Device*, HWND);
void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

};

