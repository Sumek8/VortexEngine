#pragma once

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "Texture.h"
#include "Material.h"
#include "StaticMesh.h"
#include "Skeleton.h"
#include "World.h"
#include "Lib\libPNG\png.h"
#include "Gizmo.h"
#include "Animation.h"

enum VColorFormat
{
	RGBA_FORMAT,
	RGB_FORMAT,



};
struct VBitmap
{
	VColorFormat ColorFormat;
	vector<UINT8>ColorData;
	int Width;
	int Height;

};

enum ResourceType
{
	ENull,
	EMesh,
	ESkeleton,
	ETexture,
	EMaterial,

};



class ResourceManager
{
public:

	//static ResourceManager* ManagerInstance;

	ResourceManager();
	~ResourceManager();	
	bool   AddStaticMesh(StaticMesh* VMesh);
	bool   AddEngineResource(StaticMesh* VMesh);
	void   CreateWorld();
	bool   AddTexture(Texture*);
	void   Shutdown();
	void   UpdatePolycount();
	World* GetWorld(int ID);
	static VBitmap ImportPNG(string FileName);
	bool   SaveProject();
	void   CreateGizmo();
	Gizmo* GetGizmo();
	static ResourceManager& GetResourceManager();
	void   AddActor(Actor* NewActor);
	int					 Polycount;
	int					 GetScenePolycount();
	ResourceType		 GetTypeByName(string Name);


	
	StaticMesh*			 GetStaticMeshByName(string Name);
	Texture*			 GetTextureByName(string Name);
	void				 AddSkeleton(Skeleton* VSkeleton);

	
private:

	Gizmo*				 VGizmo;
	vector<Skeleton*>	 VSkeletons;
	vector<StaticMesh*>  VStaticMeshes;
	vector<Texture*>	 VTextures;
	vector<Material*>	 VMaterials;
	vector<StaticMesh*>	 DebugStaticMeshArray;
	vector<World*>		 VWorlds;
	
};

