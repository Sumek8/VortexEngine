#include "stdafx.h"
#include "ResourceManager.h"


ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::CreateMaterial()
{
	
	Material* VMaterial = new Material;
	VMaterial->SetName("NewMaterial" + to_string(VMaterials.size()));
	VMaterials.push_back(VMaterial);
	return;

}
void ResourceManager::CreateTexture()
{
	Texture* VTexture = new Texture;
	VTextures.push_back(VTexture);
	return;
}

void ResourceManager::AddActor(Actor* NewActor)
{
	
	GetWorld(0)->VActors.push_back(NewActor);	
}

ResourceType ResourceManager::GetTypeByName(string Name)
{
	for (size_t i = 0; i < VStaticMeshes.size(); i++)
	{
		if (VStaticMeshes.at(i)->GetName() == Name)
			return EMesh;
	
	}
	return ENull;

}
bool ResourceManager::AddEngineResource(StaticMesh* VMesh)
{
	DebugStaticMeshArray.push_back(VMesh);

	return true;
}

StaticMesh* ResourceManager::GetStaticMeshByName(string Name)
{

	for (size_t i = 0; i < VStaticMeshes.size(); i++)
	{
		if (VStaticMeshes.at(i)->GetName() == Name)
			return VStaticMeshes.at(i);

	}
	return 0;

}

World* ResourceManager::GetWorld(int ID)
{

	if (VWorlds.size() > ID)
		return VWorlds[ID];
	else return 0;

}

void ResourceManager::CreateWorld()
{
	World* VWorld = new World;
	VWorlds.push_back(VWorld);
}



Texture* ResourceManager::GetTextureByName(string Name)
{

	for (size_t i = 0; i < VTextures.size(); i++)
	{
		if (VTextures.at(i)->GetName() == Name)
			return VTextures.at(i);

	}
	return 0;

}
bool ResourceManager::AddTexture(Texture* VTexture)
{
	if (VTexture)
	{
		VTextures.push_back(VTexture);
		return true;
	}
	else return false;

}



VBitmap ResourceManager::ImportPNG(string FileName)
{
	int width;
	int height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
	FILE *fp;

	fopen_s(&fp, FileName.c_str(), "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, row_pointers);


	VBitmap VBitmap;
	VBitmap.Height = height;
	VBitmap.Width = width;


	vector<UINT8>PxArray;

	png_bytep px;
	for (int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for (int x = 0; x < width; x++) {
			px = &(row[x * 4]);
			PxArray.push_back(px[0]);

		}



	}


	VBitmap.ColorData = PxArray;

	fclose(fp);
	return VBitmap;
}


bool ResourceManager::AddStaticMesh(StaticMesh* VMesh)
{
	if (VMesh)
	{
		VStaticMeshes.push_back(VMesh);
		UpdatePolycount();
		return true;
	}
	else return false;
	
}



bool ResourceManager::SaveProject()
{


	return true;
}

int ResourceManager::GetScenePolycount()
{

	return Polycount;

}



void ResourceManager::UpdatePolycount()
{
	int ScenePolycount;
	ScenePolycount = 0;

	for (size_t i = 0; i < VStaticMeshes.size(); i++)
	{
		ScenePolycount += VStaticMeshes.at(i)->GetPolycount();
	}

	Polycount = ScenePolycount;
}

void ResourceManager::CreateGizmo()
{
	VGizmo = new Gizmo;
}

Gizmo* ResourceManager::GetGizmo()
{
	if (VGizmo)
	return VGizmo;
	else return 0;

}
ResourceManager& ResourceManager::GetResourceManager()
{
	static ResourceManager ManagerInstance;
	return ManagerInstance;
}

void ResourceManager::AddSkeleton(Skeleton* VSkeleton)
{
	if (VSkeleton)
	VSkeletons.push_back(VSkeleton);

	return;
}

vector<StaticMesh*> ResourceManager::GetStaticMeshes()
{
	return VStaticMeshes;
}

vector<Texture*>ResourceManager::GetTextures()
{

	return VTextures;

}

vector<Material*> ResourceManager::GetMaterials()
{
	return VMaterials;
}

void ResourceManager::Shutdown()
{
	for (size_t i = 0; i < VStaticMeshes.size(); i++)
	{
		if (VStaticMeshes[i])
		{
			VStaticMeshes[i]->Shutdown();
			delete VStaticMeshes[i];
		}
	}
	VStaticMeshes.clear();
	
	for (size_t i = 0; i < VWorlds.size(); i++)
	{
		VWorlds[i]->Shutdown();
		delete VWorlds[i];
	}
	VWorlds.clear();

	for (size_t i = 0; i < VTextures.size(); i++)
	{
		if (VTextures[i])
		{
			VTextures[i]->Shutdown();
			delete VTextures[i];
		}
	}

	for (size_t i = 0; i < VMaterials.size(); i++)
	{
		if (VMaterials[i])
		{
			VMaterials[i]->Shutdown();
			delete VMaterials[i];
		}
	}

	for (size_t i = 0; i < DebugStaticMeshArray.size(); i++)
	{
		if (DebugStaticMeshArray[i])
		{
			DebugStaticMeshArray[i]->Shutdown();
			delete DebugStaticMeshArray[i];
		}
	}
	if (VGizmo)
	{
		VGizmo->Shutdown();
		VGizmo = 0;
	}
}