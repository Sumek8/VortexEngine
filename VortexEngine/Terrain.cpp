#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


StaticMesh* Terrain::GenerateTerrain(int Resolution, float Spacing)
{


	Chunk*	VChunk;
	VChunk = new Chunk;

	VertexStruct Vertex;

	Vertex.position.z = 0;

	Vertex.normal.x = 0;
	Vertex.normal.y = 0;
	Vertex.normal.z = 1;

	Vertex.binormal.x = 0;
	Vertex.binormal.y = 1;
	Vertex.binormal.z = 0;

	Vertex.tangent.x = 1;
	Vertex.tangent.y = 0;
	Vertex.tangent.z = 0;


	for (size_t i = 0; i < Resolution+1; i++)
	{
		for (size_t j = 0; j < Resolution+1; j++)
		{
			Vertex.position.x = j* Spacing;
			Vertex.position.z = 0;
			Vertex.position.y = i* Spacing;
			Vertex.uv.x = float((1.0f / Resolution)*j)*20;
			Vertex.uv.y = float((1.0f / Resolution)*i)*20;
			VChunk->Vertices.push_back(Vertex);
			
		}
		
	}

	for (int j = 0; j < Resolution; j++)
	{

		for (int i = 0; i < Resolution ; i++)
		{


			VChunk->VIndexArray.push_back(i  + (Resolution+1) * j); //0
			VChunk->VIndexArray.push_back(i + 1 + (Resolution+1) * j); //1
			VChunk->VIndexArray.push_back(i + (Resolution+1) * (j+1)); //3		
			VChunk->VIndexArray.push_back(i + 1 + (Resolution + 1) * j); //2
			VChunk->VIndexArray.push_back(i + 1 + (Resolution + 1) * (j + 1));	//3
			VChunk->VIndexArray.push_back(i  + (Resolution + 1) * (j + 1)); //1
		}
	}



	VChunk->VIndexCount = int(VChunk->VIndexArray.size());
	VChunk->VTriangleCount = (Resolution*Resolution)*2;
	Model* VModel;
	VModel = new Model;
	VModel->Chunks.push_back(VChunk);
	VModel->SetName("Terrain");
	VModel->SetBoundingBox(0,0,0,Resolution*Spacing,Resolution*Spacing,1);
	VModel->SetPivotPoint(0, 0, 0);
	

	StaticMesh* VMesh;
	VMesh = new StaticMesh;
	VMesh->SetModel(VModel);
	
	return VMesh;

}

Terrain::~Terrain()
{
}
