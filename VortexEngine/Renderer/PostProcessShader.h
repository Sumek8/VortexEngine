#pragma once

#include "Shader.h"

class PostProcessShader : public Shader
{
public:
	PostProcessShader();
	~PostProcessShader();

	

	void RenderShader(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* Texture);
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	bool Initialize(ID3D11Device*, HWND);
	

	
	



};

