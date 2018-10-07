#pragma once
#include "Object.h"






class Actor :
	public Object
{
public:
	Actor();
	VVector GetVelocity();
	void UpdatePhysics();
	~Actor();
	bool		GetIsVisible();
	void		SetPosition(float x, float y, float z);
	void SetPosition(VVector Vector);
	
	void		SetScale(float x, float y, float z);
	void		SetRotation(float Pitch, float Roll, float Yaw);
	void		SetRotation(VRotation Rotation);
	VVector		GetPosition();
	VVector		GetScale();
	VRotation	GetRotation();
	XMMATRIX	WorldMatrix;
	void		CalculateMatrix();
	
	bool		bIsVisible;
	void		SetVelocity(VVector velocity);

protected:

	VRotation	Rotation;
	VVector		Scale;
	VVector		Position;
	VVector		Velocity;
};

