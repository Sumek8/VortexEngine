#include "stdafx.h"
#include "Gizmo.h"


Gizmo::Gizmo()
{
	for (size_t i = 0; i < size(Meshes); i++)
	{
		Meshes[i] = 0;
	}
}


Gizmo::~Gizmo()
{
}



bool Gizmo::TransformationTrace(VVector UnprojectedVector,VAxis Axis)
{

	return true;
}

VAxis Gizmo::GetAxis()
{
	return GizmoAxis;

}
void Gizmo::SetAxis(VAxis Axis)
{
	GizmoAxis = Axis;
}

void Gizmo::SetRotationAngle(float Angle)
{

	RotationAngle = Angle;
}

float	Gizmo::GetRotationAngle()
{

	return RotationAngle;
}
bool Gizmo::RotationTrace(VVector ViewPosition,VVector UnprojectedVector, VAxis Axis)
{
	

	float Radius = 1.0f*Scale.x;
	float DeltaRadius = 0.1f*Scale.x;
		
	VVector Intersection  = LinePlaneTrace(UnprojectedVector, ViewPosition, Position, Axis);


	if ((Intersection - Position).Length() < (Radius + DeltaRadius) && (Intersection - Position).Length() > (Radius - DeltaRadius))
		return true;

	return false;
}

void Gizmo::AddMesh(StaticMesh* Mesh)
{
	
	Meshes.push_back(Mesh);

}

void Gizmo::SetState(VGizmoState NewState)
{

	State = NewState;

}
VGizmoState Gizmo::GetState()
{

	return State;
}

Model*  Gizmo::GetModel()
{
	switch (State)
	{

	case VGizmoTranslation:
	{
		return Meshes[0]->GetModel();

	}
	case	VGizmoRotation:
	{
		return Meshes[1]->GetModel();
	}

	case	VGizmoScale:
	{
		return Meshes[2]->GetModel();
	}
	}
	return 0;
}

void Gizmo::SetOrientation(VOrientation Orientation)
{
	OrientationMode = Orientation;
}
VOrientation Gizmo::GetOrientation()
{
	return OrientationMode;
}
void Gizmo::Shutdown()
{
	for (size_t i = 0; i < Meshes.size(); i++)
	{
		if (Meshes[i])
		{
			Meshes[i]->Shutdown();
			delete Meshes[i];
		}
	}
	Meshes.clear();
}