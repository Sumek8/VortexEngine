#pragma once
#include <directxmath.h>
#include "Math.h"
#include "Actor.h"

using namespace DirectX;

class CameraClass : public Actor
{
public:
	CameraClass();
	~CameraClass();
	void SetPosition(float, float, float);
	void SetPosition(VVector Vector);
	void SetRotation(float, float, float);

	VVector GetForwardVector();

	XMFLOAT3 GetRotation();
	VVector GetUpVector();
	VVector GetLeftVector();
	VVector GetRightVector();
	VVector GetPosition();
	void SetArmLength(float length);
	void Render();
	void GetViewMatrix(XMMATRIX&);
	void UpdateTransforms();
	float armLength;
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	
	XMMATRIX m_viewMatrix;

};

