#pragma once
#include "Actor.h"
#include "StaticMesh.h"

enum VGizmoState
{
	VGizmoTranslation,
	VGizmoRotation,
	VGizmoScale,
};

enum VOrientation
{
	VGlobal,
	VLocal,
};


class Gizmo :
	public Actor
{
public:
	Gizmo();
	~Gizmo();
	bool	RotationTrace(VVector Position, VVector UnprojectedVector, enum VAxis);
	VGizmoState GetState();
	void	SetState(VGizmoState NewState);
	void	Shutdown();
	void	AddMesh(StaticMesh* Mesh);
	Model*  GetModel();
	bool	TransformationTrace(VVector UnprojectedVector, VAxis Axis);
	void	SetAxis(VAxis Axis);
	VAxis	GetAxis();
	void	SetRotationAngle(float Angle);
	float	GetRotationAngle();
	void	SetOrientation(VOrientation Orientation);
	VOrientation	GetOrientation();

private:
	VOrientation OrientationMode;
	float	RotationAngle;
	VGizmoState State;
	VAxis GizmoAxis;
	float DeltaRadius;
	float GizmoScale;
	vector<StaticMesh*>   Meshes;
};

