#pragma once
#include "Actor.h"
class Light :
	public Actor
{
public:
	Light();
	~Light();
	float Intensity;
	bool bCastDynamicShadows;
	bool bCastStaticShadows;

};

