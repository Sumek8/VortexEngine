#pragma once
#pragma warning(disable : 4244)

#define FBXSDK_SHARED

#include <fstream>
#include <Commdlg.h>
#include <shobjidl.h> 
#include <fbxsdk.h>


class ResourceManager;
class Model;
class Skeleton;
class StaticMesh;
class SkeletalMesh;
class Animation;


class FBXImportManager
{
public:
	
	FBXImportManager();
	~FBXImportManager();

	Model*			InitializeFBXManager(const char* ModelPath);
	Animation *		ImportAnimation();
	Skeleton*		CreateSkeleton(FbxNode* SkeletonRoot);
	Model*			ImportModel(FbxMesh* ImportMesh, FbxNode* Geometry);
	SkeletalMesh*	ImportSkeletalMesh(FbxMesh& SkeletalMeshNode,Skeleton* VSkeleton);
	
private:
	bool bShouldImportSkeleton;
	void GenerateSkeletonNodes(FbxNode* SkeletonNode, Skeleton* VSkeleton, int ParentID);
	FbxScene* Scene;
};

