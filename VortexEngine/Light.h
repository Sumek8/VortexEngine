#pragma once
#include "Actor.h"
class Light :
	public Actor
{
public:
	Light();
	~Light();

	float GetLightIntensity();	

	VColor GetLightColor();

	bool bCastDynamicShadows;
	bool bCastStaticShadows;
	
protected:
	float  Intensity;
	VColor LightColor;
};

