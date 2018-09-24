#include "stdafx.h"
#include "Math.h"


Math::Math()
{
}


Math::~Math()
{
}

VVector VVector::MultiplyFloat(VVector SrcVector,float srcFloat)
{
	VVector DestVector;
	DestVector.x = SrcVector.x *srcFloat;
	DestVector.y = SrcVector.y *srcFloat;
	DestVector.z = SrcVector.z *srcFloat;

return DestVector;
}

VVector RotationToVector(VRotation Rotation)
{
	VVector Vector;

	Vector.x = cos(Rotation.Yaw)*cos(Rotation.Pitch);
		Vector.y = sin(Rotation.Yaw)*cos(Rotation.Pitch);
		Vector.z = sin(Rotation.Pitch);


	
	//Vector.x = Rotation.Pitch;
	//Vector.y = Rotation.Roll;
//	Vector.z = Rotation.Yaw;
	Vector.Normalize();
	return Vector;
}

void VVector::Divide( float Value)
{
	
	x = x / Value;
	y = y / Value;
	z = z / Value;
	return;
}
VVector::VVector(float srcx,float srcy,float srcz)
{
	x = srcx;
	y = srcy;
	z = srcz;

}
VVector::VVector()
{}
void VVector::Add(VVector vector)
{

	x += vector.x;
	y += vector.y;
	z += vector.z;

}

VVector VectorAdd(VVector vector1, VVector vector2)
{
	VVector Out;
	Out.x = vector1.x + vector2.x;
	Out.y = vector1.y + vector2.y;
	Out.z = vector1.z + vector2.z;
	return Out;
}


VVector VectorSubtract(VVector vector1, VVector vector2)
{
	VVector Out;
	Out.x = vector1.x - vector2.x;
	Out.y = vector1.y - vector2.y;
	Out.z = vector1.z - vector2.z;

	return Out;
}

void  VVector::Subtract(VVector Vector)
{

	x = x - Vector.x;
	y = y - Vector.y;
	z = z - Vector.z;
	return;
};




float VVector::Length()
{
	float Length;
	Length = float(pow(pow(x, 2) + pow(y, 2), 0.5f));

	Length = float(pow(pow(Length,2)+pow(z, 2),0.5f));

	return Length;

}
VVector VectorCross(VVector vector1, VVector vector2)
{
	VVector CrossVector;

	CrossVector.x = vector1.y*vector2.z- vector1.z*vector2.y;
	CrossVector.y = vector1.z*vector2.x- vector1.x*vector2.z;
	CrossVector.z = vector1.x*vector2.y- vector1.y*vector2.z;
		return CrossVector;

}
float VectorDot(VVector vector1, VVector vector2)
{
	float Length;
	Length = 0;
	Length += vector1.x*vector2.x;
	Length += vector1.y*vector2.y;
	Length += vector1.z*vector2.z;
	return Length;

}

void VVector::Normalize()
{
	
	Divide(Length());	
}



VVector CalculateFaceNormal(VVector vector1, VVector vector2)
{

	VVector Normal;

	Normal = VectorCross(vector1, vector2);
		return Normal;


}
VColor::VColor()
{

}
VColor::VColor(float SourceR, float SourceG, float SourceB, float SourceA)
{
	r = SourceR;
	g = SourceG;
	b = SourceB;
	a = SourceA;


}

VVector2::VVector2(float srcX, float srcY)
{
	x = srcX;
	y = srcY;
}
VVector2::VVector2()
{}
VVector2 VVector2::MultiplyFloat(VVector2 vector, float srcFloat)
{
	VVector2 output;
	output.x = output.x * srcFloat;
	output.y = output.y * srcFloat;
	return output;
}

VVector VVector::operator * (VVector Vector)
{
	VVector SrcVector;
	SrcVector.x = x * Vector.x;
	SrcVector.y = y * Vector.x;
	SrcVector.z = z * Vector.x;
	return  SrcVector;
}

VVector VVector::operator / (float Divide)
{
	VVector SrcVector;
	SrcVector.x = x /Divide;
	SrcVector.y = y / Divide;
	SrcVector.z = z / Divide;
	return SrcVector;
}

VVector VVector::operator * (float Scale)
{

	VVector SrcVector;
	SrcVector.x = x * Scale;
	SrcVector.y = y * Scale;
	SrcVector.z = z * Scale;
	return  SrcVector;

}

VVector VVector::operator + (VVector SrcVector)
{
	VVector OutVector;
	OutVector.x = x + SrcVector.x;
	OutVector.y = y + SrcVector.y;
	OutVector.z = z + SrcVector.z;
	return OutVector;
}
VVector VVector::operator - (VVector SrcVector)
{

	VVector OutVector;
	OutVector.x = x - SrcVector.x;
	OutVector.y = y - SrcVector.y;
	OutVector.z = z - SrcVector.z;
	return OutVector;
}
void VVector::operator /= (float Divide)
{

	x = x / Divide;
	y = y / Divide;
	z = z / Divide;
}
void  VVector::operator += (VVector AddVector)
{
	 x = x + AddVector.x;
	 y = y + AddVector.y;
	 z = z + AddVector.z;

}

float VVector2::Length()
{
	return x + y;

}
void VVector2::Subtract(VVector2 vector)
{
	x = x - vector.x;
	y = y - vector.y;

}
void VVector2::Divide(float Value)
{
	x = x / Value;
	y = y / Value;


}
void VVector2::Add(VVector2 vector)
{
	x = x + vector.x;
	y = y + vector.y;

}

VQuaternion::VQuaternion(float SrcX, float SrcY, float SrcZ, float SrcW)
{
	x = SrcX;
	y = SrcY;
	z = SrcZ;
	w = SrcW;
}

VQuaternion::VQuaternion()
{
}


VRotation::VRotation(float SourcePitch, float SourceRoll, float SourceYaw)
{
	Pitch = SourcePitch;
	Roll = SourceRoll;
	Yaw = SourceYaw;

}
VRotation VRotation::operator / (float Value)
{
	VRotation Rotation;
	Rotation.Pitch = Pitch / Value;
	Rotation.Roll = Roll / Value;
	Rotation.Yaw = Yaw / Value;
	return Rotation;
}

VRotation::VRotation()
{

}

VVector	 LinePlaneTrace(VVector UnprojectedVector, VVector ViewPosition, VVector ObjectPosition,VAxis Axis)
{

	VVector DeltaLocation = ObjectPosition - ViewPosition;

	float DistanceScale;
	switch (Axis)
	{
	case  VAxisX:
	{
		DistanceScale = DeltaLocation.x / UnprojectedVector.x;
		break;
	}
	case  VAxisY:
	{
		DistanceScale = DeltaLocation.y / UnprojectedVector.y;
		break;
	}
	case  VAxisZ:
	{
		DistanceScale = DeltaLocation.z / UnprojectedVector.z;
		break;
	}
	}

	VVector IntersectionPoint = UnprojectedVector * DistanceScale + ViewPosition;
	return IntersectionPoint;

}




bool LineSphereTrace(VVector UnprojectedVector,VVector Position, float Radius, VVector ObjectPivot)
{

	float a, b, c, t;

	a = pow(UnprojectedVector.x, 2) + pow(UnprojectedVector.y, 2) + pow(UnprojectedVector.z, 2);
	b = 2 * (UnprojectedVector.x * (Position.x - ObjectPivot.x) + UnprojectedVector.y * (Position.y - ObjectPivot.y) + UnprojectedVector.z * (Position.z - ObjectPivot.z));
	c = pow(Position.x - ObjectPivot.x, 2) + pow(Position.y - ObjectPivot.y, 2) + pow(Position.z - ObjectPivot.z, 2) - pow(Radius, 2);
	t = (-b + pow(b, 2) - 4 * a*c) / 2 * a;

	if (t >= 0)
		return true;
	else
		return false;

}