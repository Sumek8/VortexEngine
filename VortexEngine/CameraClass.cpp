#include "stdafx.h"
#include "CameraClass.h"
#include <stdio.h>


using namespace std;

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	armLength = -30.0f;
	UpdateTransforms();
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetArmLength(float length)
{
	armLength += length;

}

void CameraClass::UpdateTransforms()
{
	XMFLOAT3 rotation = GetRotation();
	float pitch, yaw, roll;

	XMMATRIX rotationMatrix;
	XMFLOAT3 position;

	pitch = (rotation.x) / 360 * XM_2PI + XM_PI;
	yaw = (rotation.y) / 360 * XM_2PI + XM_PI;
	roll = (rotation.z) / 360 * XM_2PI + XM_PI;



	XMVECTOR newPositionVector;
	

		position.x = 0;
		position.y = armLength;
		position.z = 0;


		newPositionVector = XMLoadFloat3(&position);


	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	newPositionVector = XMVector3TransformCoord(newPositionVector, rotationMatrix);


	SetPosition(newPositionVector.m128_f32[0], newPositionVector.m128_f32[1], newPositionVector.m128_f32[2]);

}

void CameraClass::SetPosition(float x, float y, float z)
{	
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}
void CameraClass::SetPosition(VVector Vector)
{
	m_positionX = Vector.x;
	m_positionY = Vector.y;
	m_positionZ = Vector.z;

	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

VVector CameraClass::GetPosition()
{


	return VVector(m_positionX, m_positionY, m_positionZ);
}


VVector CameraClass::GetRightVector()
{
	float pitch, yaw, roll;
	XMFLOAT3 Forward;
	VVector OutputVector;
	XMMATRIX Rotation;
	XMVECTOR ForwardVector;


	roll = m_rotationX * 0.0174532925f;
	pitch = m_rotationY * 0.0174532925f;
	yaw = m_rotationZ * 0.0174532925f;


	Forward.x = -1;
	Forward.y = 0;
	Forward.z = 0;

	ForwardVector = XMLoadFloat3(&Forward);

	Rotation = XMMatrixMultiply(XMMatrixRotationX(roll), XMMatrixRotationZ(yaw));
	ForwardVector = XMVector3TransformCoord(ForwardVector, Rotation);
	OutputVector.x = ForwardVector.m128_f32[0];
	OutputVector.y = ForwardVector.m128_f32[1];
	OutputVector.z = ForwardVector.m128_f32[2];
	return OutputVector;
}

VVector CameraClass::GetUpVector()
{
	float pitch, yaw, roll;
	XMFLOAT3 Forward;
	VVector OutputVector;
	XMMATRIX Rotation;
	XMVECTOR ForwardVector;


	roll = m_rotationX * 0.0174532925f;
	pitch = m_rotationY * 0.0174532925f;
	yaw = m_rotationZ * 0.0174532925f;


	Forward.x = 0;
	Forward.y = 0;
	Forward.z = 1;

	ForwardVector = XMLoadFloat3(&Forward);

	Rotation = XMMatrixMultiply(XMMatrixRotationX(roll), XMMatrixRotationZ(yaw));
	ForwardVector = XMVector3TransformCoord(ForwardVector, Rotation);
	OutputVector.x = ForwardVector.m128_f32[0];
	OutputVector.y = ForwardVector.m128_f32[1];
	OutputVector.z = ForwardVector.m128_f32[2];
	return OutputVector;
}



VVector CameraClass::GetLeftVector()
{
	float pitch, yaw, roll;
	XMFLOAT3 Forward;
	VVector OutputVector;
	XMMATRIX Rotation;
	XMVECTOR ForwardVector;


	roll = m_rotationX * 0.0174532925f;
	pitch = m_rotationY * 0.0174532925f;
	yaw = m_rotationZ * 0.0174532925f;


	Forward.x = 1;
	Forward.y = 0;
	Forward.z = 0;

	ForwardVector = XMLoadFloat3(&Forward);

	Rotation = XMMatrixMultiply(XMMatrixRotationX(roll), XMMatrixRotationZ(yaw));
	ForwardVector = XMVector3TransformCoord(ForwardVector, Rotation);
	OutputVector.x = ForwardVector.m128_f32[0];
	OutputVector.y = ForwardVector.m128_f32[1];
	OutputVector.z = ForwardVector.m128_f32[2];
	return OutputVector;
}


VVector CameraClass::GetForwardVector()
{
	float pitch, yaw, roll;
	XMFLOAT3 Forward;
	VVector OutputVector;
	XMMATRIX Rotation;
	XMVECTOR ForwardVector;


	roll = m_rotationX * 0.0174532925f;
	pitch = m_rotationY * 0.0174532925f;
	yaw = m_rotationZ * 0.0174532925f;


	Forward.x = 0;
	Forward.y = 1;
	Forward.z = 0;

	ForwardVector = XMLoadFloat3(&Forward);

	Rotation = XMMatrixMultiply(XMMatrixRotationX(roll), XMMatrixRotationZ(yaw));
	
		ForwardVector = XMVector3TransformCoord(ForwardVector,Rotation);
		OutputVector.x = ForwardVector.m128_f32[0];
		OutputVector.y = ForwardVector.m128_f32[1];
		OutputVector.z = ForwardVector.m128_f32[2];
		return OutputVector;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}
void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;
	


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 0.0f;
	up.z = 1.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 1.0f;
	lookAt.z = 0.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	roll = m_rotationX * 0.0174532925f;
	pitch = m_rotationY * 0.0174532925f;
	yaw = m_rotationZ * 0.0174532925f;

	
	rotationMatrix = XMMatrixMultiply(XMMatrixRotationX(roll), XMMatrixRotationZ(yaw));
	// Create the rotation matrix from the yaw, pitch, and roll values.



	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	
	//XMVector3Rotate(lookAtVector,)

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}
//After the Render function has been called to create the view matrix we can provide the updated view matrix to calling functions using this GetViewMatrix function.The view matrix will be one of the three main matrices used in the HLSL vertex shader.

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}