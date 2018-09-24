#pragma once

#include <DirectXMath.h>

using namespace DirectX;


const float PI = 3.141592471f;
struct VVector2
{
	float x;
	float y;

	VVector2(float x, float y);
	VVector2();
	VVector2 MultiplyFloat(VVector2 vector, float srcFloat);
	float Length();
	void Subtract(VVector2 vector);
	void Divide(float Value);
	void Add(VVector2 vector);
	

};

struct VQuaternion
{
	float x;
	float y;
	float z;
	float w;

	VQuaternion(float x,float y,float z,float w);
	VQuaternion();
};


struct VRotation
{
	float Pitch;
	float Roll;
	float Yaw;

	VRotation(float Pitch, float Roll, float Yaw);
	VRotation();
	VRotation operator / (float);
};

struct VVector
{
		float x;
		float y;
		float z;

		VVector(float x, float y, float z);
		VVector();
		VVector MultiplyFloat(VVector vector, float srcFloat);
		float Length();
		void Subtract(VVector vector);
		void Divide(float Value);
		void Add(VVector vector);
		void Normalize();

		VVector operator * (float Scale);
		VVector operator * (VVector Vector);
		VVector operator / (float);
		VVector operator - (VVector Vector);
		VVector operator + (VVector Vector);

		void operator += (VVector AddVector);
		void operator /= (float);
};

struct VColor
{
	float r;
	float g;
	float b;
	float a;
	VColor(float r, float g, float b, float a);
	VColor();

};

enum VAxis
{
	VAxisX,
	VAxisY,
	VAxisZ,
};



struct VMatrix
{
	float Matrix[4][4];

};

float VectorDot(VVector vector1, VVector vector2);
VVector VectorCross(VVector vector1, VVector vector2);

VVector CalculateFaceNormal(VVector vector1, VVector vector2);
VVector VectorSubtract(VVector vector1, VVector vector2);
VVector VectorAdd(VVector vector1, VVector vector2);

VVector	 LinePlaneTrace(VVector UnprojectedVector, VVector ViewPosition, VVector ObjectPivot,VAxis Axis);

 bool	 LineSphereTrace(VVector UnprojectedVector, VVector ViewPosition, float Radius, VVector ObjectPivot);

 VVector RotationToVector(VRotation Rotation);

class Math
{
public:
	Math();
	~Math();
};

