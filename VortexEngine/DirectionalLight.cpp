#include "stdafx.h"
#include  "DirectionalLight.h"


DirectionalLight::DirectionalLight()
{
	Intensity = 1;
	LightColor = VColor(1.0f,1.0f,1.0f, 1.0f);
	SetPosition(0,0,0);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::GenerateViewMatrix()
{

	
	
	XMFLOAT3 up, lookAt;
	XMVECTOR upVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 0.0f;
	up.z = 1.0f;

	upVector = XMLoadFloat3(&up);

	
	pitch =(Rotation.Pitch)/360*XM_2PI + XM_PI;
	yaw = (Rotation.Yaw) / 360 * XM_2PI + XM_PI;
	roll = (Rotation.Roll) / 360 * XM_2PI + XM_PI;

	
	
	XMFLOAT3 position;
	position.x = Position.x;
	position.y = Position.y;
	position.z = Position.z;
	
	XMVECTOR targetVector;
	targetVector = XMLoadFloat3(&position);




	rotationMatrix = XMMatrixRotationRollPitchYaw( pitch, yaw,roll);

	targetVector = XMVector3TransformCoord(targetVector, rotationMatrix);

	SetPosition(targetVector.m128_f32[0],
		targetVector.m128_f32[1],
		targetVector.m128_f32[2]);


	lookAt.x = 0.0f;
	lookAt.y = 1.0f;
	lookAt.z = 0.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);


	m_viewMatrix = XMMatrixLookAtLH(targetVector, lookAtVector, upVector);
	

	return;
}

void DirectionalLight::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	
		float fieldOfView, screenAspect;


		// Setup field of view and screen aspect for a square light source.
		fieldOfView = (float)XM_PI / 2.0f;
		screenAspect = 1.0f;

	
		m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
		return;
	

}



void DirectionalLight::SetLookAt(float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}

void DirectionalLight::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void DirectionalLight::GetViewMatrix(XMMATRIX& viewMatrix)
{
	
	viewMatrix = m_viewMatrix;
	return;
}