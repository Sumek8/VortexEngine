#pragma once

#pragma warning(disable : 4067)
#include "Object.h"

#define DIRECTX = true

#ifdef DIRECTX;
#include <directxmath.h>
#include <d3d11.h>
#include <vector>
#include <math.h>
using namespace DirectX;

#else OPENGL
#include <GL/gl.h>
#include <GL/glu.h>

#endif // OPENGL



struct VertexStruct
{

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};

struct InstanceStruct
{
	VVector Position;
	VVector Rotation;
	VVector Scale;

};


struct SkinnedVertexStruct
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
	int		 BoneIndices[4];
	double	 BoneWeights[3];
};


enum VertexStructType
{
	
};

struct BoundingBox
{
	float MinZ;
	float MaxZ;
	float MinX;
	float MaxX;
	float MinY;
	float MaxY;
};

struct Chunk
{
	int	 				 VTriangleCount;
	int					 VIndexCount;
	vector<int>		     VIndexArray;
	vector<VertexStruct> Vertices;
	string				 MaterialName;


#ifdef DIRECTX;

	ID3D11Buffer* VVertexBuffer;
	ID3D11Buffer* VIndexBuffer;

#else OPENGL

	GLuint VVertexVBO;

#endif // OPENGL


};

class Model :
	public Object
{
public:
	Model();
	~Model();
	void		UpdateIndexCount();
	void		Shutdown();
	void		SetPivotPoint(float x, float y, float z);
	void		SetBoundingBox(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ);
	void		CreateBoundingSphere();
	void		CalculateGeometryCenter();
	int		    GetIndexCount();
	VVector     GetPivotPoint();
	BoundingBox GetBoundingBox();
	VVector		GeometryCenter;

	vector<Chunk*> Chunks;
private:
	
	int		    VIndexCount;
	BoundingBox VBoundingBox;
	float		BoundingSphereRadius;
	VVector		VPivot;

};

