#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "Math.h"

class LineShader
{
public:
	LineShader();

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, int indexCount);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	ID3D11VertexShader* VVertexShader;
	ID3D11PixelShader* VPixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11Buffer* m_constantBuffer;

	ID3D11SamplerState*		m_sampleState;
	ID3D11Device*			D3DDevice;
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	bool Initialize(ID3D11Device*, HWND);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);



	~LineShader();
};

