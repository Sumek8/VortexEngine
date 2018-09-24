
#pragma once
#pragma warning(disable : 4244)

#include <d3d11.h>
#include <directxmath.h>
#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxmesh.h>
#include <fbxsdk/fileio/fbximporter.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <fstream>
#include <Commdlg.h>
#include <shobjidl.h> 
#include "TextureClass.h"


using namespace DirectX;

struct VertexType
{
	
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};



class ModelClass
{
public:
	ModelClass();
	~ModelClass();
	bool Initialize(ID3D11Device*, char* FilePath);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	void InitializeFBXManager(char* FilePath, ID3D11Device* device);
	int GetPolycount();
	bool CreateSkeletonHierarchy();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	
	FbxMesh* ObjMesh;
	int PolygonCount;
	
};

