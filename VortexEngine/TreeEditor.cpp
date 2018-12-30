#include "stdafx.h"
#include "TreeEditor.h"


TreeEditor::TreeEditor()
{
}
StaticMesh* TreeEditor::GenerateTree()
{
	
	float Height;
	float Radius;
	float Shape;
	int SegmentCount;
	float Randomize;
	int TrunkCount;
	int BranchesCount;
	float Bend;
	Bend = 0;
	Height = 5;
	Radius = 0.3;
	Shape = 0.5;
	SegmentCount = 10;

	int LoopCount = 15;

	TrunkCount = 3;
	BranchesCount = 5;
	Chunk*	Trunk;
	Trunk = new Chunk;

	float TrunkOffset;

	float segmentHeight = Height / SegmentCount;
	for (size_t i = 0; i < TrunkCount; i++)
	{

	
		for (size_t SegmentIndex = 0; SegmentIndex < SegmentCount+1; SegmentIndex++)
		{


		for (size_t i = 0; i < LoopCount+1; i++)
		{

			float RadiusModifier = 1 - ((1-Shape)/ SegmentCount * SegmentIndex);
			float BendAmount = Bend;


			VertexStruct Vertex;

			Vertex.position.z = SegmentIndex * segmentHeight;

			Vertex.position.x = sin(PI * 2 / LoopCount*i)*Radius*RadiusModifier;
			Vertex.position.y = cos(PI * 2 / LoopCount*i)*Radius*RadiusModifier;
			
			Vertex.normal.x = sin(PI * 2 / LoopCount * i);
			Vertex.normal.y = cos(PI * 2 / LoopCount * i);
			Vertex.normal.z = 0;

			
			Vertex.uv.x = float(1.0f / float(LoopCount) * i);
			Vertex.uv.y = SegmentIndex*(1/ (PI * 2 * Radius));			
			Trunk->Vertices.push_back(Vertex);
		}

	
	

		}

		for (size_t SegmentIndex = 0; SegmentIndex < SegmentCount; SegmentIndex++)
		{


			for (size_t i = 0; i < LoopCount; i++)
			{

				{
				Trunk->VIndexArray.push_back(0 + (LoopCount+1) * SegmentIndex + i); //0
				Trunk->VIndexArray.push_back(0 + (LoopCount+1) * (SegmentIndex + 1) + i); //3 
				Trunk->VIndexArray.push_back(1 + (LoopCount+1) * SegmentIndex + i);   //1
			
				Trunk->VIndexArray.push_back(1 + (LoopCount+1) * SegmentIndex + i);   //1
				Trunk->VIndexArray.push_back(0 + (LoopCount+1) * (SegmentIndex + 1) + i); //3
				Trunk->VIndexArray.push_back(1 + (LoopCount+1) * (SegmentIndex + 1) + i); //4
				
			}
				

		}
	}
}

	for (size_t i = 0; i < BranchesCount; i++)
	{

	}


	Trunk->VIndexCount = Trunk->VIndexArray.size();
	
	Model* TreeModel = new Model;
	TreeModel->Chunks.push_back(Trunk);

	
	TreeModel->CalculateGeometryCenter();
	TreeModel->SetName("Tree");




	//Trunk->Vertices.push_back(Vertex);

	StaticMesh* TreeMesh = new StaticMesh;
	TreeMesh->SetName("Tree");
	TreeMesh->SetModel(TreeModel);

	//TreeMesh

	return TreeMesh;

}

TreeEditor::~TreeEditor()
{
}
