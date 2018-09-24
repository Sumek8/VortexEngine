#pragma once

#include <directxmath.h>
#include "Light.h"
using namespace DirectX;

struct RotationTransform
{
	float Roll;
	float Pitch;
	float Yaw;

};

class DirectionalLight : public Actor
{


public:
	DirectionalLight();
	~DirectionalLight();

	RotationTransform Rotation;
	VColor LightColor;
	
	
	void SetLookAt(float, float, float);
	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float ViewFar, float ViewNear);
	void GetViewMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
	XMFLOAT3 m_position;
	XMFLOAT3 m_lookAt;

};