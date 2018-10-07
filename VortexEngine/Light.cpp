#include "stdafx.h"
#include "Light.h"


Light::Light()
{
	LightColor = VColor(1, 1, 1, 1);
}


Light::~Light()
{
}

float Light::GetLightIntensity()
{
	return Intensity;
}

VColor Light::GetLightColor()
{
	return LightColor;
}