#pragma once
#include "StaticMeshActor.h"

class Terrain :
	public Actor
{
public:
	Terrain();
	~Terrain();
	StaticMesh* Mesh;
	StaticMesh* GenerateTerrain(int Resolution,float Spacing);
	
};
