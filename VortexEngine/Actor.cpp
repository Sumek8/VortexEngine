#include "stdafx.h"
#include "Actor.h"




Actor::Actor()
{
		Rotation = VRotation(0,0,0);
		Scale = VVector(1,1,1);
		Position = VVector(0,0,0);
		Velocity = VVector(0, 0, 0);
}
VVector Actor::GetVelocity()
{
	return Velocity;
}
void Actor::UpdatePhysics()
{
	if (Velocity.z < 54.015f)
		SetVelocity(Velocity + VVector(0, 0, -0.01635));
	else
		Velocity.z = 54.015;
	SetPosition(Position + Velocity);

}

void Actor::SetVelocity(VVector NewVelocity)
{
	Velocity = NewVelocity;

}

void Actor::CalculateMatrix()
{

	WorldMatrix = XMMatrixIdentity();
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(Rotation.Roll/360*2*PI, Rotation.Pitch / 360 * 2 * PI, Rotation.Yaw / 360 * 2 * PI);
	XMMATRIX ScaleMatrix  = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
	WorldMatrix = XMMatrixMultiply(XMMatrixMultiply(RotationMatrix, ScaleMatrix), TranslationMatrix);
}
void	Actor::SetRotation(VRotation SourceRotation)
{

	Rotation = SourceRotation;
}

bool Actor::GetIsVisible()
{
	return bIsVisible;
}
void Actor::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;


}
void Actor::SetPosition(VVector Vector)
{
	Position.x = Vector.x;
	Position.y = Vector.y;
	Position.z = Vector.z;


}
void Actor::SetScale(float x, float y, float z)
{
	Scale = VVector(x, y, z);
}
void Actor::SetRotation(float Pitch, float Roll, float Yaw)
{

	Rotation = VRotation(Pitch, Roll, Yaw);
}
VVector Actor::GetScale()
{
	return Scale;
}
VVector Actor::GetPosition()
{

	return Position;
}
VRotation Actor::GetRotation()
{

	return Rotation;
}

Actor::~Actor()
{
}
