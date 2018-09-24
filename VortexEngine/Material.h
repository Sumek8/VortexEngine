#pragma once
#include "Graphics.h"

class Material
{
public:
	Material();
	~Material();
	void SetTexture(ID3D11ShaderResourceView* Texture);
	

	ID3D11ShaderResourceView* Textures;
	ID3D11PixelShader*		  VPixelShader;
	ID3D11VertexShader*		  VVertexShader;

	vector<Texture>TextureArray;

	void Shutdown();

	bool   Transparent;
	bool   TwoSided;
	string MaterialName;
};
