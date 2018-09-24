#include "stdafx.h"
#include <iostream>
#include <string> 
#include "ModelClass.h"

using namespace std;

ModelClass::ModelClass()
{
	
	m_vertexCount = 0;
	m_indexCount = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	
	

}
int ModelClass::GetPolycount()
{
	return PolygonCount;
}

 void ModelClass::InitializeFBXManager(char* ModelPath,ID3D11Device* device)
{
	 FbxManager* pManager = FbxManager::Create();
	 FbxGeometryConverter* FbxGConv = new FbxGeometryConverter(pManager);
	 FbxImporter* lImporter = FbxImporter::Create(pManager, "");
	 FbxScene* pScene = FbxScene::Create(pManager,"");

	FbxIOSettings * ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	//Load name from source
	bool lImportStatus = lImporter->Initialize(ModelPath, -1, pManager->GetIOSettings());
	if (!lImportStatus)
		return;

	lImporter->Import(pScene);
	// Destroy the importer.
	lImporter->Destroy();
	
	
	FbxGConv->Triangulate(pScene, true,true);
	
	
	//Load FBXNode
	FbxNode* FBXRootNode = pScene->GetRootNode();
	if (FBXRootNode) {
		for (int i = 0; i < FBXRootNode->GetChildCount(); i++)
		{

			if (FBXRootNode->GetChild(i))
				{
			
			FbxNode* childNode = FBXRootNode->GetChild(i);
			if (childNode)
			{
			
			FbxStringList Flags = childNode->GetTypeFlags();
		FbxUInt ObjFlag = childNode->GetAllObjectFlags();
				FbxNodeAttribute* Attribute = childNode->GetNodeAttribute();
				FbxNodeAttribute::EType Type = Attribute->GetAttributeType();
				//SKELETON
		//	FbxSkeleton* skeleton = childNode->GetSkeleton();
			//if (skeleton)
			//	OutputDebugStringA("Works");
				int count = childNode->GetChildCount();
				if (count > 0)
				{
					FbxNode* Node = childNode->GetChild(0);
					
					OutputDebugStringA(Node->GetName());
				}
				wchar_t *result = new wchar_t[10];
				swprintf(result, 10, L"  %d ",count);

				OutputDebugStringW(result);
				
				//MESH
				if (Type == FbxNodeAttribute::EType::eMesh)
						{
					FbxMesh *mesh = childNode->GetMesh();
						if (mesh)
							{

						childNode->GetName();
						ObjMesh = mesh;
						childNode->GetTransform();
						InitializeBuffers(device);
						pManager->Destroy();
						return;
							}
						}
					}
				}
			}
		}
	
	

		
}

 bool ModelClass::CreateSkeletonHierarchy()
 {
	 return true;
 }


ModelClass::~ModelClass()
{
}



bool ModelClass::Initialize(ID3D11Device* device, char* FilePath)
{
	bool result;

	InitializeFBXManager(FilePath,device);

	return true;
}
void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}
//GetIndexCount returns the number of indexes in the model.The color shader will need this information to draw this model.

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	VertexType* vertices;

	HRESULT result;


	if (!ObjMesh)
		return false;
	int Polycount = ObjMesh->GetPolygonCount();
	PolygonCount = Polycount;
	m_indexCount = ObjMesh->GetPolygonVertexCount();
	

	float ImportScale = 1;

	int* indices = new int[m_indexCount];
	

	int* indicesIndex = indices;

	indices = ObjMesh->GetPolygonVertices();



	vertices = new VertexType[m_indexCount];
	ObjMesh->GenerateNormals(true, true, false);
	


	int* vertIndex = new int[m_indexCount];
	for (int i = 0; i < m_indexCount; i++)
		vertIndex[i] = i;
	


	FbxStringList UvNames;

	int materialCount = ObjMesh->GetElementMaterialCount();
	ObjMesh->GetUVSetNames(UvNames);
	const char* lUVSetName = UvNames.GetStringAt(0);
	int UVSetCount = UvNames.GetCount();

	FbxGeometryElementNormal* lNormalElement = ObjMesh->GetElementNormal();
	FbxGeometryElementUV* UVelement = ObjMesh->GetElementUV(lUVSetName);

	FbxGeometryElementBinormal* BinormalElement = ObjMesh->GetElementBinormal();
	FbxGeometryElementMaterial* MaterialElement = ObjMesh->GetElementMaterial();
	FbxGeometryElementTangent* TangentElement = ObjMesh->GetElementTangent();
	
	XMFLOAT3 BoundsSize;
	FbxAMatrix Pivot;


	ObjMesh->ComputeBBox();
	//ImportScale 
	BoundsSize.x = ObjMesh->BBoxMax.Get().mData[0] - ObjMesh->BBoxMin.Get().mData[0];
	BoundsSize.y = ObjMesh->BBoxMax.Get().mData[1] - ObjMesh->BBoxMin.Get().mData[1];
	BoundsSize.z = ObjMesh->BBoxMax.Get().mData[2] - ObjMesh->BBoxMin.Get().mData[2];
	
	ObjMesh->GetPivot(Pivot);
	ImportScale = 10/(pow(pow(pow(BoundsSize.x, 2) + pow(BoundsSize.y, 2), 0.5) + pow(BoundsSize.z, 2), 0.5));
	
	for (int j = 0; j < Polycount; j++)
	{
		for (int i = 0; i < 3; i++)
		{

			FbxVector4 vert = ObjMesh->GetControlPointAt(indices[j * 3 + i]);
			
			vertices[3 * j + i].position.x = (float)vert.mData[0] * ImportScale;
			vertices[3 * j + i].position.y = (float)vert.mData[1] * ImportScale;
			vertices[3 * j + i].position.z = (float)vert.mData[2] * ImportScale;

			FbxVector4 normal = lNormalElement->GetDirectArray().GetAt(indices[j * 3 + i]);

			vertices[3 * j + i].normal.x = (float)normal[0];
			vertices[3 * j + i].normal.y = (float)normal[1];
			vertices[3 * j + i].normal.z = (float)normal[2];





			if (BinormalElement != NULL)

				if (BinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (BinormalElement->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						BinormalElement->GetDirectArray().GetAt(indices[j * 3 + i]);
						FbxVector4 binormal = BinormalElement->GetDirectArray().GetAt(indices[j * 3 + i]);
						vertices[3 * j + i].binormal.x = binormal[0];
						vertices[3 * j + i].binormal.x = binormal[1];
						vertices[3 * j + i].binormal.x = binormal[2];
						break;
					}

					case FbxGeometryElement::eIndexToDirect:
					{
						int id = BinormalElement->GetIndexArray().GetAt(i);
						FbxVector4 binormal = BinormalElement->GetDirectArray().GetAt(indices[j * 3 + i]);
						vertices[3 * j + i].binormal.x = binormal[0];
						vertices[3 * j + i].binormal.x = binormal[1];
						vertices[3 * j + i].binormal.x = binormal[2];
						break;
					}

					break; // other reference modes not shown here!
					}
				}

			if (TangentElement != NULL)

				if (TangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (TangentElement->GetReferenceMode())
					{
						{
					case FbxGeometryElement::eDirect:

						FbxVector4 tangent = TangentElement->GetDirectArray().GetAt(indices[j * 3 + i]);
						vertices[3 * j + i].tangent.x = tangent[0];
						vertices[3 * j + i].tangent.x = tangent[1];
						vertices[3 * j + i].tangent.x = tangent[2];
						break;
						}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = TangentElement->GetIndexArray().GetAt(indices[j * 3 + i]);
						FbxVector4 tangent = TangentElement->GetDirectArray().GetAt(id);
						vertices[3 * j + i].tangent.x = tangent[0];
						vertices[3 * j + i].tangent.x = tangent[1];
						vertices[3 * j + i].tangent.x = tangent[2];
						break;
					}

					default:
						break; 
					}
				}

			
			if (MaterialElement != NULL)
				if (MaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)

				{
				}
				

		}
	}


	if (UVelement != NULL) //There are no UV's
	{
		const bool lUseIndex = UVelement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? UVelement->GetIndexArray().GetCount() : 0;


		if (UVSetCount > 0)
		{
			if (UVelement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
				UVelement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			{
				return false;
			}
			if (UVelement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				for (int lPolyIndex = 0; lPolyIndex < Polycount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = ObjMesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						FbxVector2 UV;
						//get the index of the current vertex in control points array
						int lPolyVertIndex = ObjMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? UVelement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

						UV = UVelement->GetDirectArray().GetAt(lUVIndex);


						vertices[lPolyIndex * 3 + lVertIndex].uv.x = (float)UV[0];
						vertices[lPolyIndex * 3 + lVertIndex].uv.y = (float)UV[1];

					}
				}
			}
			else if (UVelement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{

				int lPolyIndexCounter = 0;
				for (int lPolyIndex = 0; lPolyIndex < Polycount; ++lPolyIndex)
				{

					// build the max index array that we need to pass into MakePoly
					for (int lVertIndex = 0; lVertIndex < 3; ++lVertIndex)
					{



						bool unMapped;

						unMapped = false;
						bool result = true;
						FbxVector2 UV;


						result = ObjMesh->GetPolygonVertexUV(lPolyIndex, lVertIndex, lUVSetName, UV, unMapped);

						vertices[lPolyIndex * 3 + lVertIndex].uv.x = (float)UV[0];
						vertices[lPolyIndex * 3 + lVertIndex].uv.y = -(float)UV[1];


					}
				}
			}
		}

	}
	//Print string to deb
	/*
	char buffer[200];
	sprintf_s(buffer, size_t(100), "PolyCount  %d\n", ObjMesh->GetPolygonCount());
	OutputDebugStringA(buffer);
	*/
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_indexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = vertIndex;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ObjMesh->Destroy();
	delete[] vertices;
	vertices = 0;

	delete[] indicesIndex;
	indicesIndex = 0;
	
	delete[] vertIndex;
	vertIndex = 0;

	return true;
}



void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

