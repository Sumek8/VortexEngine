
//#pragma warning(disable : 4244)


#include "stdafx.h"
#include "FBXImportManager.h"
#include "ResourceManager.h"
#include "Model.h"

#include "StaticMesh.h"
#include "SkeletalMesh.h"
#include "Skeleton.h"
#include <vector>
#include "SystemClass.h"



FBXImportManager::FBXImportManager()
{
	bShouldImportSkeleton = true;
}


FBXImportManager::~FBXImportManager()
{
}



SkeletalMesh* FBXImportManager::ImportSkeletalMesh(FbxMesh & SkeletalMeshNode,Skeleton* VSkeleton)
{
	SkeletalMesh* VSkeletalMesh = new SkeletalMesh;
	
	FbxMesh* ImportMesh = &SkeletalMeshNode;

	if (VSkeleton != 0)
	{

		int DeformerCount = ImportMesh->GetDeformerCount();
		FbxDeformer* Deformer = ImportMesh->GetDeformer(0, FbxDeformer::eSkin);
		FbxSkin* Skin = reinterpret_cast<FbxSkin*>(Deformer);
		int ClusterCount = Skin->GetClusterCount();
		int* ControlIndices = 0;
		double* IndicesWeights = 0;
		for (int i = 0; i < ClusterCount; i++)
		{
			ControlIndices = Skin->GetCluster(i)->GetControlPointIndices();
			IndicesWeights = Skin->GetCluster(i)->GetControlPointWeights();
			
		}
	}




	return VSkeletalMesh;

}



Model* FBXImportManager::InitializeFBXManager(const char* ModelPath)
{
	FbxManager* pManager = FbxManager::Create();
	FbxGeometryConverter* FbxGConv = new FbxGeometryConverter(pManager);
	FbxImporter*		  lImporter = FbxImporter::Create(pManager, "");
	
	FbxIOSettings * ios = FbxIOSettings::Create(pManager, IOSROOT);

	Scene = FbxScene::Create(pManager, "");

	pManager->SetIOSettings(ios);

	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	
	bool lImportStatus = lImporter->Initialize(ModelPath, -1, pManager->GetIOSettings());
	if (lImportStatus)

	lImporter->Import(Scene);
	
	lImporter->Destroy();
	
	string ObjectName;
	int NodeCount = Scene->GetNodeCount();
	int GeoCount = Scene->GetGeometryCount();

	
	
	
	FbxDocumentInfo* Info = Scene->GetSceneInfo();

	FbxGeometry* Geometry = Scene->GetGeometry(0);
	
	if (!Geometry)
		return 0;
	FbxNode* GeometryNode = Geometry->GetNode();


	Skeleton* VSkeleton = 0;

	
	

	FbxTransform Transform;


	
	

	FbxGConv->Triangulate(Scene, true, true);
	
	FbxMesh*ImportMesh;
	ImportMesh = 0;
	
	FbxNode* FBXRootNode = Scene->GetRootNode();
	if (FBXRootNode) {
		for (int i = 0; i < FBXRootNode->GetChildCount(); i++)
		{
			if (FBXRootNode->GetChild(i))
			{


				FbxNode* Node = FBXRootNode->GetChild(i);
				if (Node)
					if (Node->GetChild(0))
					{
						FbxNodeAttribute::EType AttributeType = Node->GetChild(0)->GetNodeAttribute()->GetAttributeType();
						if (AttributeType == FbxNodeAttribute::eSkeleton && bShouldImportSkeleton)
						{
							
							VSkeleton = CreateSkeleton(Node);

							ImportAnimation();
							break;
						}

					}

			}
		}
		for (int i = 0; i < FBXRootNode->GetChildCount(); i++)
		{
			if (FBXRootNode->GetChild(i))
			{

					FbxNode* Node = FBXRootNode->GetChild(i);
			
					//Load Mesh Model From Impported File
					FbxMesh *mesh = Node->GetMesh();
				if (mesh)
				{
					ObjectName = Node->GetName();
					ImportMesh = mesh;
					Transform = Node->GetTransform();

				return 	ImportModel(mesh, GeometryNode);


				}
			}
		}

	}

	pManager->Destroy();
	return 0;
}
Animation* FBXImportManager::ImportAnimation()
{
	
	vector<string>AnimNames;
	vector<FbxAnimStack*>AnimStacks;

	//int Posecount = Scene->GetPoseCount();

	int AnimationStacksCount = Scene->GetSrcObjectCount<FbxAnimStack>();

	if (AnimationStacksCount > 0)
	{


		for (size_t AnimationID = 0; AnimationID <AnimationStacksCount; AnimationID++)
		{
			AnimStacks.push_back(Scene->GetSrcObject<FbxAnimStack>(int(AnimationID)));
			AnimNames.push_back(Scene->GetSrcObject<FbxAnimStack>(int(AnimationID))->GetName());
	


	
			int AnimLayersCount = AnimStacks[AnimationID]->GetMemberCount<FbxAnimLayer>();


			FbxAnimLayer* AnimLayer = AnimStacks[AnimationID]->GetMember<FbxAnimLayer>(0);
		

			int CurveNodeCount = AnimLayer->GetMemberCount<FbxAnimCurveNode>();
		
			FbxAnimCurveNode* CurveNode = AnimLayer->GetMember<FbxAnimCurveNode>(0);
		
			int ChannelCount = CurveNode->GetChannelsCount();
			for (int ChannelID = 0; ChannelID <ChannelCount; ChannelID++)
			{

			
			string ChannelName = CurveNode->GetChannelName(int(ChannelID));
			string CurveName = CurveNode->GetName();

			FbxAnimCurve* AnimCurve = CurveNode->GetCurve(ChannelID, 0);
		
			int KeyCount = AnimCurve->KeyGetCount();

				for (int KeyID = 0; KeyID < KeyCount; KeyID++)
				{
					FbxTime KeyTime = AnimCurve->KeyGetTime(KeyID);
					FbxAnimCurveDef::EInterpolationType CurveInterp = AnimCurve->KeyGetInterpolation(KeyID);

				}
			

			}
		}
	}
	//* AnimCurve = 
	//	FbxAnimCurve* = Scene.get


	AnimNames.clear();
	AnimStacks.clear();

	return 0;
}

Model* FBXImportManager::ImportModel(FbxMesh* ImportMesh,FbxNode* Geometry)
{
	
	int MaterialCount = Geometry->GetMaterialCount();
	vector<const char*>MaterialNames;

	for (int i = 0; i < MaterialCount; i++)
	{
		MaterialNames.push_back(Geometry->GetMaterial(i)->GetName());
	}


	if (!ImportMesh)
		return nullptr;

	//if (bSkeletalMesh)
	VertexStruct* vertices;
	
	Model* VMesh;
	VMesh = new Model;
	int VIndexCount;
	int Polycount = ImportMesh->GetPolygonCount();
	if (Polycount == 0)
		return 0;
	VIndexCount = ImportMesh->GetPolygonVertexCount();

	float ImportScale = 1.0f;

	int* indices = new int[VIndexCount];


	int* indicesIndex = indices;

	indices = ImportMesh->GetPolygonVertices();

	FbxStringList UvNames;

	

	ImportMesh->GetUVSetNames(UvNames);
	const char* lUVSetName = UvNames.GetStringAt(0);
	int UVSetCount = UvNames.GetCount();
	bool result;
	
	vertices = new VertexStruct[VIndexCount];
	//result = ImportMesh->GenerateNormals(true,true, false);
	result = ImportMesh->GenerateTangentsData(UvNames.GetStringAt(0),true);
		


	FbxGeometryElementNormal* NormalElement = ImportMesh->GetElementNormal();
	FbxGeometryElementUV* UVelement = ImportMesh->GetElementUV(lUVSetName);

	FbxGeometryElementBinormal* BinormalElement = ImportMesh->GetElementBinormal();
	FbxGeometryElementMaterial* MaterialElement = ImportMesh->GetElementMaterial();
	FbxGeometryElementTangent* TangentElement = ImportMesh->GetElementTangent();
	XMFLOAT3 BoundsSize;
	FbxAMatrix Pivot;


	ImportMesh->ComputeBBox();
	//ImportScale 
	BoundsSize.x = float(ImportMesh->BBoxMax.Get().mData[0] - ImportMesh->BBoxMin.Get().mData[0]);
	BoundsSize.y = float(ImportMesh->BBoxMax.Get().mData[1] - ImportMesh->BBoxMin.Get().mData[1]);
	BoundsSize.z = float(ImportMesh->BBoxMax.Get().mData[2] - ImportMesh->BBoxMin.Get().mData[2]);


	ImportMesh->GetPivot(Pivot);
	VMesh->SetBoundingBox(ImportMesh->BBoxMin.Get().mData[0], ImportMesh->BBoxMin.Get().mData[1], ImportMesh->BBoxMin.Get().mData[2], ImportMesh->BBoxMax.Get().mData[0], ImportMesh->BBoxMax.Get().mData[1],ImportMesh->BBoxMax.Get().mData[2]);
	
	
	VMesh->SetPivotPoint(Pivot.GetT()[0], Pivot.GetT()[1], Pivot.GetT()[2]);

	
	

//	ImportScale = float(10 / (pow(pow(pow(BoundsSize.x, 2) + pow(BoundsSize.y, 2), 0.5) + pow(BoundsSize.z, 2), 0.5)));
	for (int j = 0; j < Polycount; j++)
	{
		for (int i = 0; i < 3; i++)
		{

		
			FbxVector4 vert = ImportMesh->GetControlPointAt(indices[j * 3 + i]);

			vertices[3 * j + i].position.x = (float)vert.mData[0] * ImportScale;
			vertices[3 * j + i].position.y = (float)vert.mData[1] * ImportScale;
			vertices[3 * j + i].position.z = (float)vert.mData[2] * ImportScale;

			if (NormalElement != NULL)
			{
				if (NormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					FbxLayerElement::EMappingMode Mapping = NormalElement->GetMappingMode();
					FbxGeometryElement::EReferenceMode Reference = NormalElement->GetReferenceMode();

					FbxVector4 normal = NormalElement->GetDirectArray().GetAt(indices[j * 3 + i]);

					vertices[3 * j + i].normal.x = (float)normal[0];
					vertices[3 * j + i].normal.y = (float)normal[1];
					vertices[3 * j + i].normal.z = (float)normal[2];
				}
				else if (NormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{

					FbxVector4 normal = NormalElement->GetDirectArray().GetAt(3 * j + i);
					vertices[3 * j + i].normal.x = (float)normal[0];
					vertices[3 * j + i].normal.y = (float)normal[1];
					vertices[3 * j + i].normal.z = (float)normal[2];

				}

			}

			if (BinormalElement != NULL)
			{
				FbxLayerElement::EMappingMode Mapping = BinormalElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode Reference = BinormalElement->GetReferenceMode();
				
				if (BinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (BinormalElement->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{

						FbxVector4 binormal = BinormalElement->GetDirectArray().GetAt(j * 3 + i);
						vertices[3 * j + i].binormal.x = float(binormal[0]);
						vertices[3 * j + i].binormal.y = float(binormal[1]);
						vertices[3 * j + i].binormal.z = float(binormal[2]);
						break;
					}

					case FbxGeometryElement::eIndexToDirect:
					{
						int id = BinormalElement->GetIndexArray().GetAt(i);
						FbxVector4 binormal = BinormalElement->GetDirectArray().GetAt(indices[j * 3 + i]);
						vertices[3 * j + i].binormal.x = float(binormal[0]);
						vertices[3 * j + i].binormal.y = float(binormal[1]);
						vertices[3 * j + i].binormal.z = float(binormal[2]);
						break;
					}

					break; // other reference modes not shown here!
					}
				}
			}
			
			if (TangentElement != NULL)
			{

				FbxLayerElement::EMappingMode Mapping = TangentElement->GetMappingMode();
				FbxGeometryElement::EReferenceMode Reference = TangentElement->GetReferenceMode();



				if (TangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (TangentElement->GetReferenceMode())
					{
						{
					case FbxGeometryElement::eDirect:

						FbxVector4 tangent = TangentElement->GetDirectArray().GetAt(j * 3 + i);
						vertices[3 * j + i].tangent.x = float(tangent[0]);
						vertices[3 * j + i].tangent.y = float(tangent[1]);
						vertices[3 * j + i].tangent.z = float(tangent[2]);
						break;
						}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = TangentElement->GetIndexArray().GetAt(indices[j * 3 + i]);
						FbxVector4 tangent = TangentElement->GetDirectArray().GetAt(id);
						vertices[3 * j + i].tangent.x = float(tangent[0]);
						vertices[3 * j + i].tangent.y = float(tangent[1]);
						vertices[3 * j + i].tangent.z = float(tangent[2]);
						break;
					}

					default:
						break;
					}
				}




			}
		}
	}
			if (UVelement != NULL) //There are no UV's
	{
		FbxLayerElement::EMappingMode Mapping = UVelement->GetMappingMode();
		FbxGeometryElement::EReferenceMode Reference = UVelement->GetReferenceMode();

		const bool lUseIndex = UVelement->GetReferenceMode() != FbxGeometryElement::eDirect;

		const int lIndexCount = (lUseIndex) ? UVelement->GetIndexArray().GetCount() : 0;

		if (UVSetCount > 0)
		{
			if (UVelement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
				UVelement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			{
				for (int lPolyIndex = 0; lPolyIndex < Polycount; ++lPolyIndex)
				{
					const int lPolySize = ImportMesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						vertices[lPolyIndex * 3 + lVertIndex].uv.x = 0;
						vertices[lPolyIndex * 3 + lVertIndex].uv.y = -0;
					}
							
				}
			}
			if (UVelement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				for (int lPolyIndex = 0; lPolyIndex < Polycount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = ImportMesh->GetPolygonSize(lPolyIndex);
				
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						FbxVector2 UV;
						//get the index of the current vertex in control points array
						int lPolyVertIndex = ImportMesh->GetPolygonVertex(lPolyIndex, lVertIndex);
						int lUVIndex = lUseIndex ? UVelement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

						//the UV index depends on the reference mode

						UV = UVelement->GetDirectArray().GetAt(lUVIndex);


						vertices[lPolyIndex * 3 + lVertIndex].uv.x = (float)UV[0];
						vertices[lPolyIndex * 3 + lVertIndex].uv.y = -(float)UV[1];

					}
				}
			}
			else if (UVelement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				

				for (int lPolyIndex = 0; lPolyIndex < Polycount; ++lPolyIndex)
				{

					
					for (int lVertIndex = 0; lVertIndex < 3; ++lVertIndex)
					{


						bool unMapped;

						unMapped = false;
						bool result = true;
						FbxVector2 UV;


						result = ImportMesh->GetPolygonVertexUV(lPolyIndex, lVertIndex, lUVSetName, UV, unMapped);
						vertices[lPolyIndex * 3 + lVertIndex].uv.x = (float)UV[0];
						vertices[lPolyIndex * 3 + lVertIndex].uv.y = -(float)UV[1];


					}
				}
			}
		}
	}


	//CREATE CHUNKS

	

	if (MaterialElement != NULL && MaterialCount > 1)
	{
		FbxLayerElement::EMappingMode Mapping = MaterialElement->GetMappingMode();
		FbxGeometryElement::EReferenceMode Reference = MaterialElement->GetReferenceMode();

		
		vector<int>ChunkIndices;
		
		int	MaterialIndex;

		for (int i = 0; i < MaterialCount; i++)
		{
		
			ChunkIndices.clear();

			for (int j = 0; j < Polycount; j++)
			{
				if (MaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
				{
		
					break;
				}
				if (MaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
				{


					switch (MaterialElement->GetReferenceMode())
					{

					case FbxGeometryElement::eIndexToDirect:
						MaterialIndex = MaterialElement->GetIndexArray().GetAt(j);

						if (MaterialIndex == i)
						{
							for (int k = 0; k < 3; k++)
							{
								
								
								ChunkIndices.push_back(j*3 + k);
							}
						

						}


					}
				}
			}

			Chunk* ModelChunk;
			ModelChunk = new Chunk;

			ModelChunk->VIndexCount = int(ChunkIndices.size());
			ModelChunk->VTriangleCount = int(ChunkIndices.size()) / 3;
			ModelChunk->MaterialName = MaterialNames[i];
			

			
			int ChunkSize = int(ChunkIndices.size());

			for (size_t f = 0; f < ChunkSize; f++)
			{
				ModelChunk->Vertices.push_back(vertices[ChunkIndices[f]]);
			}

			ChunkIndices.clear();

			for (int f = 0; f < ChunkSize; f++)
			{
				ChunkIndices.push_back(f);
			}
			
			ModelChunk->VIndexArray = ChunkIndices;
			VMesh->Chunks.push_back(ModelChunk);
			ChunkIndices.clear();

		}
		
	}
	else
	{
		int* VerticesIndices = new int[VIndexCount];
		for (int i = 0; i < VIndexCount; i++)
			VerticesIndices[i] = i;



		Chunk* ModelChunk;
		ModelChunk = new Chunk;

		ModelChunk->VIndexCount = int(VIndexCount);
		ModelChunk->VTriangleCount = int(VIndexCount) / 3;
		
		if(MaterialElement != NULL)
		 	ModelChunk->MaterialName = MaterialNames[0];



		for (size_t f = 0; f <VIndexCount; f++)
		{
			ModelChunk->VIndexArray.push_back(VerticesIndices[f]);
			ModelChunk->Vertices.push_back(vertices[f]);
		}

		VMesh->Chunks.push_back(ModelChunk);


	}
	

	ImportMesh->Destroy();

	return  VMesh;


}




Skeleton* FBXImportManager::CreateSkeleton(FbxNode* SkeletonRoot)
{

	

	Skeleton* VSkeleton;
	VSkeleton = new Skeleton;

	Bone RootBone;
	RootBone.Name = SkeletonRoot->GetName();
	RootBone.Length = 0;
	RootBone.Position = VVector(0,0,0);
	RootBone.Rotation = VQuaternion(0,0,0,0);
	VSkeleton->BoneList.push_back(RootBone);

	FbxSkeleton::EType Type =  SkeletonRoot->GetChild(0)->GetSkeleton()->GetSkeletonType();

	GenerateSkeletonNodes(SkeletonRoot, VSkeleton,0);
	
	return VSkeleton;
	
}
void FBXImportManager::GenerateSkeletonNodes(FbxNode* SkeletonNode,Skeleton* VSkeleton,int ParentID)
{
	for (int i = 0; i < SkeletonNode->GetChildCount(); i++)
	{
		
		Bone ChildBone;


		FbxDouble3 Transform = SkeletonNode->GetChild(i)->LclTranslation.Get();
		FbxDouble3 Rotation = SkeletonNode->GetChild(i)->LclRotation.Get();
		FbxDouble3 Scale = SkeletonNode->GetChild(i)->LclScaling.Get();

		SkeletonNode->GetChild(i)->GetTransform();
		ChildBone.Name = SkeletonNode->GetChild(i)->GetName();
		ChildBone.Position = VVector(Transform.mData[0], Transform.mData[1], Transform.mData[2]);
		ChildBone.Rotation = VQuaternion(Rotation.mData[0], Rotation.mData[1], Rotation.mData[2],1);
		ChildBone.Scale = VVector(Scale.mData[0], Scale.mData[1], Scale[2]);
		VSkeleton->BoneList.push_back(ChildBone);
		
		
		VSkeleton->BoneList[ParentID].ChildBonesID.push_back(int(VSkeleton->BoneList.size()) - 1);

		GenerateSkeletonNodes(SkeletonNode->GetChild(i), VSkeleton, int(VSkeleton->BoneList.size()-1));
	}
	

}