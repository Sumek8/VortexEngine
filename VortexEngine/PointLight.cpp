#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight()
{
	LightColor = VColor(1, 1, 1, 1);
	Attentuation = 0;
	Falloff = 0;
	Intensity = 0;
}


PointLight::~PointLight()
{
}
