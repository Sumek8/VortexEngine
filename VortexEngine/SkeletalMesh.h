#pragma once
#include "Object.h"
#include "Skeleton.h"
#include "Model.h"

class SkeletalMesh :
	public Object
{
public:
	SkeletalMesh();
	~SkeletalMesh();
	Skeleton* VSkeleton;
	Model* VModel;
};

