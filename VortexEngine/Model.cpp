#include "stdafx.h"
#include "Model.h"


Model::Model()
{
	RenderCustomDepth = true;
}


void Model::SetBoundingBox(float MinX, float MinY, float MinZ, float MaxX, float MaxY,float MaxZ)
{

	VBoundingBox.MaxX = MaxX;
	VBoundingBox.MaxY = MaxY;
	VBoundingBox.MaxZ = MaxZ;
	VBoundingBox.MinX = MinX;
	VBoundingBox.MinY = MinY;
	VBoundingBox.MinZ = MinZ;

	CalculateGeometryCenter();

}

void Model::CalculateGeometryCenter()
{
	

	GeometryCenter.x = (VBoundingBox.MaxX + VBoundingBox.MinX) / 2;
	GeometryCenter.y = (VBoundingBox.MaxY + VBoundingBox.MinY) / 2;
	GeometryCenter.z = (VBoundingBox.MaxZ + VBoundingBox.MinZ) / 2;
	


}
void Model::CreateBoundingSphere()
{
	

	for (size_t i = 0; i < Chunks.size(); i++)
	{
		for (size_t j = 0; j < Chunks.at(i)->VIndexCount; j++)
		{
			float vectorX,vectorY,vectorZ,Radius;
			vectorX = GeometryCenter.x - Chunks.at(i)->Vertices[j].position.x;
			vectorY = GeometryCenter.y - Chunks.at(i)->Vertices[j].position.y;
			vectorZ = GeometryCenter.z - Chunks.at(i)->Vertices[j].position.z;
			 
			Radius = float((pow(pow(pow(vectorX, 2) + pow(vectorY, 2), 0.5) + pow(vectorZ, 2), 0.5)));

		}
		
	}


}


int Model::GetIndexCount()
{

	return VIndexCount;

}
void Model::UpdateIndexCount()
{
	VIndexCount = 0;
	for (size_t i = 0; i < Chunks.size(); i++)
	{
		VIndexCount += Chunks.at(i)->VIndexCount;

	}
	
}


BoundingBox Model::GetBoundingBox()
{

	return VBoundingBox;

}

void Model::SetPivotPoint(float x, float y, float z)
{

	VPivot = VVector(x, y, z);


}

VVector Model::GetPivotPoint()
{
	return VPivot;

}


Model::~Model()
{
}
void Model::Shutdown()
{
		return;
		for (size_t i = 0; i <Chunks.size(); i++)
		{
						Chunks[i]->Vertices.clear();
						
						 Chunks[i]->VIndexArray.clear();
						
						 Chunks[i]->VVertexBuffer->Release();
						 Chunks[i]->VVertexBuffer = 0;
						 Chunks[i]->VIndexBuffer->Release();
						 Chunks[i]->VIndexBuffer = 0;

		}

		Chunks.clear();
}
