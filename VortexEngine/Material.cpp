#include "stdafx.h"
#include "Material.h"


Material::Material()
{
}



Material::~Material()
{
}

void Material::Shutdown()
{
	if (VPixelShader)
	{
		VPixelShader->Release();
		VPixelShader = 0;
	}
	if (VVertexShader)
	{
		VVertexShader->Release();
		VVertexShader = 0;
	}
	
}