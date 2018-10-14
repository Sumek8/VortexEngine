#include "stdafx.h"
#include "Material.h"


Material::Material()
{
}



Material::~Material()
{
}

string Material::GetName()
{
	return MaterialName;
}
void Material::SetName(string Name)
{
	MaterialName = Name;

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