#pragma once
#include "Math.h"
#include <vector>
#include <string>
using namespace std;


struct Bone
{
	VVector		Position;
	VQuaternion Rotation;
	VVector		Scale;
	string		Name;
	float		Length;
	vector<int>ChildBonesID;

	Bone();
	~Bone();
};




class Skeleton
{

	
public:
	Skeleton();
	~Skeleton();
	vector<Bone>BoneList;
};

