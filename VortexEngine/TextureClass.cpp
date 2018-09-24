



#include "stdafx.h"

#include "TextureClass.h"


using namespace DirectX;


TextureClass::TextureClass()
{
}


TextureClass::~TextureClass()
{
}
bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	
	
	
	return true;
}
bool TextureClass::LoadTexture(ID3D11Device* device, WCHAR* filename,ID3D11ShaderResourceView* out)
{
	DDS_ALPHA_MODE *t_Mode;
	DDS_ALPHA_MODE mode = DDS_ALPHA_MODE_OPAQUE;
	t_Mode = &mode;
	HRESULT result;
	
	const wchar_t* FileName;
	FileName = L"Content/Texture.DDS";
	// Load the texture in.
	ID3D11ShaderResourceView* ResView = nullptr;
	//		DirectX::CreateWICTextureFromFile
	result = DirectX::CreateDDSTextureFromFile(device, FileName, nullptr, &ResView);
	out = ResView;
	if (FAILED(result))
	{
		
		return false;
	}
	return true;
}



void TextureClass::Shutdown()
{}
