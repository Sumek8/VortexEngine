#include "stdafx.h"
#include "Terrain.h"
#include "ResourceManager.h"

Terrain::Terrain()
{
}


StaticMesh*  Terrain::GenerateTerrain(int Resolution, float Spacing)
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


StaticMesh* Terrain::CreateTerrain(int SizeX, int SizeY,float Height)
{
	int Resolution = 120;
	
	VBitmap HeightMap = ResourceManager::ImportPNG("Content/Heightmap2.PNG");
	
	StaticMesh* Mesh = Terrain::GenerateTerrain((Resolution - 1)*SizeX, 0.1f);

	Mesh->SetName("Terrain");
	

	float scale = float(HeightMap.Height) / float(Resolution);
	int j, k;
	j = 0;
	if (scale < 1)
		return 0;
	for (size_t i = 0; i < Mesh->GetModel()->Chunks[0]->Vertices.size(); i++)
	{
		j = int(i / (Resolution));

		k = (int(i*scale) + j * int(scale - 1)*(HeightMap.Width));


		Mesh->GetModel()->Chunks[0]->Vertices[i].position.z = float(HeightMap.ColorData[k]) / 256 * Height;

	}
	size_t VertSize = Mesh->GetModel()->Chunks[0]->Vertices.size();
	for (size_t i = 0; i < VertSize; i++)
	{
		float P, PL, PR, PU, PD;
		PD = 0;
		PL = 0;
		PR = 0;
		PU = 0;

		P = Mesh->GetModel()->Chunks[0]->Vertices[i].position.z;
		if (i > 0)
			PL = Mesh->GetModel()->Chunks[0]->Vertices[i - 1].position.z;
		if (i < (VertSize - 1))
			PR = Mesh->GetModel()->Chunks[0]->Vertices[i + 1].position.z;
		if (i < VertSize - Resolution)
			PU = Mesh->GetModel()->Chunks[0]->Vertices[i + Resolution].position.z;
		if (i > Resolution)
			PD = Mesh->GetModel()->Chunks[0]->Vertices[i - Resolution].position.z;

		float hL = P - PL;
		float hR = P - PR;
		float hD = P - PD;
		float hU = P - PU;
		VVector N;
		// Compute terrain normal
		N.x = hL - hR;
		N.y = hD - hU;
		N.z = 2.0;
		N.Normalize();
		Mesh->GetModel()->Chunks[0]->Vertices[i].normal.x = N.x;
		Mesh->GetModel()->Chunks[0]->Vertices[i].normal.y = N.y;
		Mesh->GetModel()->Chunks[0]->Vertices[i].normal.z = N.z;
	}

	return Mesh;
}


Terrain::~Terrain()
{
}
