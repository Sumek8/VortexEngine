#pragma once
#include "Actor.h"

#include "SkeletalMesh.h"
class SkeletalMeshActor :
	public Actor
{
public:
	SkeletalMeshActor();
	~SkeletalMeshActor();
	void Shutdown();
private:
	SkeletalMesh Mesh;


};

