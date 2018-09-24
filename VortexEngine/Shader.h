#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Math.h"


class Shader
{
public:

	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView* Texture);
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual void ShutdownShader();
	virtual bool Initialize(ID3D11Device*, HWND);
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);


	ID3D11VertexShader* VVertexShader;
	ID3D11PixelShader* VPixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11BlendState* VBlendState;

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_constantBuffer;

	ID3D11SamplerState*  TextureSampler;
	ID3D11Device* D3DDevice;

	Shader();
	~Shader();
};

