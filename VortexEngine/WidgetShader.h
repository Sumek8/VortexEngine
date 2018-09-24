#pragma once
#include "Shader.h"

class WidgetShader : public Shader
{
public:
	WidgetShader();
	~WidgetShader();

	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual void ShutdownShader();
	virtual bool Initialize(ID3D11Device*, HWND);
	virtual void RenderTexturedWidget(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView* Texture, bool bTransparent);

	ID3D11PixelShader* VTextPixelShader;
	ID3D11VertexShader* VTextVertexShader;
	
};

