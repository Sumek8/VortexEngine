



#include "stdafx.h"

#include "Texture.h"


using namespace DirectX;


Texture::Texture()
{
}


Texture::~Texture()
{
}


bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	
	
	
	return true;
}


bool Texture::LoadTexture(ID3D11Device* device, WCHAR* filename,ID3D11ShaderResourceView* out)
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

string Texture::GetName()
{
	return Name;

}

void Texture::Shutdown()
{}
