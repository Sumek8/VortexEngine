#include "stdafx.h"
#include "StaticMesh.h"



StaticMesh::StaticMesh()
{
	vertexCount = 0;
	VModel = 0;

}

string StaticMesh::GetName()
{
	
	return Name;

}



void StaticMesh::CalculateSphereBounds()
{
	BoundingBox VBoundingBox = VModel->GetBoundingBox();
	BoundSphereRadius = pow(pow(pow(VBoundingBox.MaxX,2.0f)+pow(VBoundingBox.MaxY,2.0f),0.5f)+ pow(VBoundingBox.MaxZ, 2.0f),0.5f);
}

float  StaticMesh::GetSphereBoundsRadius()
{

	return BoundSphereRadius;
}

void StaticMesh::SetModel(Model* srcModel)
{
	Name = srcModel->GetName();
	VModel = srcModel;
	CalculateSphereBounds();
	polygonCount = 0;
	for (size_t i = 0; i < srcModel->Chunks.size(); i++)
	{
		polygonCount += srcModel->Chunks.at(i)->VTriangleCount;

	}
	


	return;
}



Model* StaticMesh::GetModel()
{
	if (VModel)
	return VModel;
	else return 0;
}

StaticMesh::~StaticMesh()
{
}



int StaticMesh::GetPolycount()
{
	if (VModel)
	return VModel->GetIndexCount()/3;
	else return 0;
}



void StaticMesh::Shutdown()
{
	if(VModel)
	delete VModel;

}