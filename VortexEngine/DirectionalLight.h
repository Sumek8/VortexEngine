#pragma once

#include <directxmath.h>
#include "Light.h"
using namespace DirectX;


class DirectionalLight : public Light
{


public:
	DirectionalLight();
	~DirectionalLight();


	
	
	void SetLookAt(float, float, float);
	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float ViewFar, float ViewNear);
	void GetViewMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);
private:
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
	XMFLOAT3 m_position;
	XMFLOAT3 m_lookAt;

};