#pragma once
#include "Actor.h"
#include "StaticMesh.h"






class StaticMeshActor:
	public Actor
{
public:

	void Destroy();
	bool bCastDynamicShadows;
	StaticMeshActor();
	~StaticMeshActor();
	StaticMesh* Mesh;
	bool bEnablePhysics;
	void Shutdown();
};

