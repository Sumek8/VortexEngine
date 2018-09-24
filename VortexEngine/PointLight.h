#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:

	VColor LightColor;
	float Intensity;
	float Attentuation;
	float Falloff;
	PointLight();
	~PointLight();
};

