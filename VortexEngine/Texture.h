#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")



#include <d3d11.h>
#include <directxmath.h>
#include <wincodec.h>
#include <d3d12shader.h>
#include <dxgiformat.h>
#include <assert.h>
#include <memory>
#include <stdint.h>
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include <string>
using namespace std;
class Texture
{
public:
	Texture();
	~Texture();
	bool LoadTexture(ID3D11Device* device, WCHAR* filename, ID3D11ShaderResourceView* out);
	string GetName();
	bool Initialize(ID3D11Device*, WCHAR*);
	// Standard version
	
	string Name;
	char* TexturePath;
	void Shutdown();
};

