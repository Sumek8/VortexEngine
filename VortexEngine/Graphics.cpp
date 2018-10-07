#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "Widget.h"
#include "SystemClass.h"
#include "ResourceManager.h"
#include "WidgetShader.h"
#include "MaterialMaster.h"
#include "PostProcessShader.h"

Graphics::Graphics()
{
	VDirect3D = 0;
	VCamera = 0;
	VShaderClass = 0;
	VDepthShader = 0;
	BaseColorMap = 0;

	bApplyPostProcess = true;

	DrawCallCount = 0;
	DrawInterface = true;
}



Graphics::~Graphics()
{
}
void Graphics::SetLightRotation(VRotation Rotation)
{
	if (VDirectionalLight)
	VDirectionalLight->SetRotation(Rotation);
	
	return;
}
void Graphics::SetLightPosition(VVector Position)
{
	if (VDirectionalLight)
	VDirectionalLight->SetRotation(Position.x, Position.y, Position.z);

	return;
}
void Graphics::GetLightRotation(VRotation outRotation)
{
	if (VDirectionalLight)
	outRotation = VDirectionalLight->GetRotation();

	return;
}



bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	
	// Create the Direct3D object.
	VDirect3D = new D3DClass;
	if (!VDirect3D)
	{
		return false;
	}

	
	// Initialize the Direct3D object.
	result = VDirect3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

		
	// Create the camera object.
	VCamera = new CameraClass;
	if (!VCamera)
	{
		return false;
	}

	// Set the initial position of the camera.
	VCamera->SetPosition(0.0f, -10.0f, 4.0f);
	VCamera->SetRotation(0.0f,0.0f,0.0f);

	
	
	VDepthShader = new DepthShader;
		if(!VDepthShader)
		{

			return false;
		}
		result = VDepthShader->Initialize(VDirect3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
			return false;
		}

	// Create the color shader object.
	VShaderClass = new PBRShaderClass;
	if (!VShaderClass)
	{

		return false;
	}

	VLineShader = new LineShader;
	if (!VLineShader)
	{
		return false;

	}

	VWidgetShader = new WidgetShader;
	if (!VWidgetShader)
	{
		return false;

	}

	
		result = VLineShader->Initialize(VDirect3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	result = VWidgetShader->Initialize(VDirect3D->GetDevice(), hwnd);
	if (!result)
	{
			MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
			return false;
	}
	// Initialize the color shader object.
	result = VShaderClass->Initialize(VDirect3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	

	
	
	
	//Load BaseColor
	if (!LoadShaderResource(L"Content/Grid.PNG", &BaseColorMap))
		return false;
	
	//Load NormalMap
		if (!LoadShaderResource(L"Content/Grid_N.PNG", &NormalMap))
			return false;
	
		if (!LoadShaderResource(L"Content/Cubemap.PNG", &CubeMap))
			return false;

		
		if (!LoadShaderResource(L"Content/Font/Arial.PNG", &FontMap))
			return false;

		if (!LoadShaderResource(L"Content/Engine/GizmoTexture.PNG",&GizmoMap))
			return false;

	return true;
}


D3DClass* Graphics::GetD3DClass()
{

	return VDirect3D;

}





bool Graphics::Frame()
{
	bool result;

	for (int i = 0; i < VWidgetManager->GetWindowCount(); i++)
	{
		if (VWidgetManager->WidgetContainers[i]->GetIsBeingDestroyed())
		{
			VWidgetManager->ActiveWidget = 0;
			VWidgetManager->WidgetContainers[i]->ClearContainer();
			VWidgetManager->WidgetContainers.erase(VWidgetManager->WidgetContainers.begin() + i);
			GetD3DClass()->RemoveSwapChain(i);
			break;
			
		}
	}

	// Render the graphics scene.
	result = Render();
	if (!result)
	{

		return false;
	}

	return true;
}
bool Graphics::LoadTextureFromFile(WCHAR* filename, ID3D11Resource** out)
{
	HRESULT hr;
	
	hr = DirectX::CreateWICTextureFromFile(VDirect3D->GetDevice(), filename, out,nullptr );
	return true;
}
bool Graphics::LoadShaderResource(WCHAR* filename, ID3D11ShaderResourceView** out)
{
	HRESULT hr;
	
	hr = DirectX::CreateWICTextureFromFile(VDirect3D->GetDevice(), filename, nullptr,out);
	
	//hr = DirectX::CreateDDSTextureFromFile(device, L"Texture.DDS", nullptr, &resourceView);
	if (FAILED(hr)) {
		return false;
	
	}
	
	//VDirect3D->GetDeviceContext()->GenerateMips(BaseColorMap);
	

	return true;
}
void Graphics::UpdateFPS()
{
	
	
		VWidgetManager->GetWidgetContainer(SystemClass::GetSystem()->GetSelectedWindow())->GetViewportList()[0]->UpdateFPS(SystemClass::GetSystem()->GetFPS());

}

void Graphics::UpdateDrawCallCount()
{
	VWidgetManager->GetWidgetContainer(SystemClass::GetSystem()->GetSelectedWindow())->GetViewportList()[0]->UpdateDrawCall(DrawCallCount);

}

void Graphics::UpdatePolycount()
{
	
	int Polycount = VResourceManager->GetWorld(0)->StaticMeshPolyCount;
		if (Polycount>0)
		VWidgetManager->GetWidgetContainer(SystemClass::GetSystem()->GetSelectedWindow())->GetViewportList()[0]->UpdatePolycount(Polycount);
		else
			VWidgetManager->GetWidgetContainer(SystemClass::GetSystem()->GetSelectedWindow())->GetViewportList()[0]->UpdatePolycount(0);
	
}
void Graphics::SetWidgetManager(WidgetManager* srcWidgetManager)
{
	VWidgetManager = srcWidgetManager;
	return;

}

void Graphics::SetResourceManager(ResourceManager* SrcManager)
{
	VResourceManager = SrcManager;

}
PBRShaderClass* Graphics::GetShader()
{
	return VShaderClass;

}

void Graphics::ToggleWireframe()
{
	bRenderWireframe = !bRenderWireframe;

}

bool Graphics::Render()
{


	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightProjectionMatrix, lightViewMatrix;
	bool result;
	size_t ObjectCount;
	VColor DiffuseColor = VColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	DrawCallCount = 0;

	World* VWorld = VResourceManager->GetWorld(0);

	ObjectCount = VWorld->VActors.size();
	UpdateFPS();

	VCamera->Render();

	if (bRenderWireframe)
		VDirect3D->SetWireframeMode(true);
	if (VDirectionalLight)
	{
		VDirectionalLight->GetViewMatrix(lightViewMatrix);
		VDirectionalLight->GetProjectionMatrix(lightProjectionMatrix);
	}
	//RenderDynamicShadows
	if (ObjectCount == 0 )
		VDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	else
	{
		if (VDirectionalLight)
		{
			VDirectionalLight->GetProjectionMatrix(lightProjectionMatrix);
			VDirectionalLight->GenerateViewMatrix();
		}
		if (!bRenderWireframe)
		{
			VDirect3D->RenderShadows();

			for (size_t i = 0; i < ObjectCount; i++)
			{

				StaticMeshActor* StaticActor = dynamic_cast<StaticMeshActor*>(VWorld->VActors[i]);
				if (StaticActor)
					if (StaticActor->bCastDynamicShadows && StaticActor->bIsVisible)
					{
						
						StaticActor->CalculateMatrix();
						worldMatrix = StaticActor->WorldMatrix;

						for (size_t j = 0; j < StaticActor->Mesh->GetModel()->Chunks.size(); j++)
						{
							DrawCallCount++;
							RenderBuffers(VDirect3D->GetDeviceContext(), StaticActor->Mesh->GetModel()->Chunks[j]->VVertexBuffer, StaticActor->Mesh->GetModel()->Chunks[j]->VIndexBuffer);
							result = VDepthShader->SetShaderParameters(VDirect3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, StaticActor->Mesh->GetModel()->Chunks.at(j)->VIndexCount);
							if (!result)
								return false;
						}
				}

			}
			VDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}


	int ActiveWindow = SystemClass::GetSystem()->GetSelectedWindow();
	//RenderToViewport
	for (int i = 0; i < VWidgetManager->WidgetContainers.size(); i++)
	{
		if (ActiveWindow == i)
		{
			if (ObjectCount > 0)
			{

				VDirect3D->SetGBufferRenderTarget();


				float ViewportAspect = VWidgetManager->WidgetContainers[i]->GetViewportList()[0]->GetViewportAspect();



				VCamera->GetViewMatrix(viewMatrix);

				VDirect3D->GetProjectionMatrix(projectionMatrix, ViewportAspect);
				if (VDirectionalLight)
				{
					VDirectionalLight->GetViewMatrix(lightViewMatrix);
				}


				for (size_t i = 0; i < ObjectCount; i++)
				{
					StaticMeshActor* StaticActor = dynamic_cast<StaticMeshActor*>(VWorld->VActors[i]);
					if (StaticActor)
					{
						StaticActor->CalculateMatrix();
						worldMatrix = StaticActor->WorldMatrix;


						for (size_t j = 0; j < StaticActor->Mesh->GetModel()->Chunks.size(); j++)
						{
							if (StaticActor->bIsVisible)
							{

								DrawCallCount++;
								RenderBuffers(VDirect3D->GetDeviceContext(), StaticActor->Mesh->GetModel()->Chunks[j]->VVertexBuffer, StaticActor->Mesh->GetModel()->Chunks[j]->VIndexBuffer);
								result = VShaderClass->Render(VDirect3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, StaticActor->Mesh->GetModel()->Chunks[j]->VIndexCount,BaseColorMap, NormalMap, VDirect3D->pShadowMapSRView, CubeMap);

							}
						}
					}

				}


				if (bRenderWireframe)
					VDirect3D->SetWireframeMode(false);

			 //Rotation Correction
				VRotation CameraRot = VCamera->GetRotation();
				CameraRot.Yaw -= 90;
				
				float     LightIntensity = 0;
				VRotation LightDirection = VRotation(0, 0, 0);
				VColor    LightColor = VColor(1, 1, 1, 1);
					if (VDirectionalLight)
					{
						if (VDirectionalLight->GetIsVisible())
						{
							LightIntensity = VDirectionalLight->GetLightIntensity();
							LightDirection = VDirectionalLight->GetRotation();
							LightColor = VDirectionalLight->GetLightColor();
						}
					}
				///////RenderLightPass/////////
				VDirect3D->SetDeferredRenderTarget();
				VDirect3D->GetProjectionMatrix(projectionMatrix, ViewportAspect);
				VCamera->GetViewMatrix(viewMatrix);
				result = VShaderClass->RenderDeferredLightPass(VDirect3D->GetDeviceContext(),VDirect3D->GetGBufferResource(), VDirect3D->pShadowMapSRView, worldMatrix, viewMatrix, projectionMatrix, lightProjectionMatrix, lightViewMatrix,LightDirection,LightColor,RotationToVector(CameraRot), LightIntensity);
			
			if (!result)
				{
					return false;
				}

				if (bApplyPostProcess)
				{
					///PostProcess Pass1
					VDirect3D->SetPostProcessRenderTarget(1);
					result = VShaderClass->RenderPostProcess(VDirect3D->GetDeviceContext(), VDirect3D->GetPostProcessResourceView(0));

					//VDirect3D->SetBlurRenderTarget();
					//result = VShaderClass->RenderPostProcess(VDirect3D->GetDeviceContext(), VDirect3D->GetPostProcessResourceView(0));

					


					//PostProcess RenderToBackBuffer
					VDirect3D->SetPostProcessRenderTarget(0);
					result = VShaderClass->RenderPostProcess(VDirect3D->GetDeviceContext(), VDirect3D->GetPostProcessResourceView(1));
				   // result = VShaderClass->RenderPostProcess(VDirect3D->GetDeviceContext(), VDirect3D->GetBlurResourceView());
					
				}
				///WidgetPass

				//Gizmo
				if (VWorld->GetSelectedActorsCount() > 0)
				{

					VDirect3D->RenderGizmo();
					VVector Position;
					Position = VVector(0, 0, 0);
					for (int i = 0; i < VWorld->GetSelectedActorsCount(); i++)
					{
						Position += VWorld->GetSelectedActor(i)->GetPosition();
					}
					Position = Position / VWorld->GetSelectedActorsCount();

					VResourceManager->GetGizmo()->SetPosition(Position.x,Position.y,Position.z);

					worldMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);

					VVector CamPos;
					
					CamPos.x = VCamera->GetPosition().x;
					CamPos.y = VCamera->GetPosition().y;
					CamPos.z = VCamera->GetPosition().z;
					
					

					const double fov = PI / 4.0;  //suppose 45 degrees FOV

					VVector VectorDist = VectorSubtract(CamPos, Position);
					float Distance = VectorDist.Length();

					float worldSize = float((2 * tan(fov / 2.0f)) * Distance);
					GizmoScale = 0.15f * worldSize;
					
					VResourceManager->GetGizmo()->SetScale(GizmoScale, GizmoScale, GizmoScale);

					VResourceManager->GetGizmo()->CalculateMatrix();

					worldMatrix = VResourceManager->GetGizmo()->WorldMatrix;

				
					

					Model* VGizmo = VResourceManager->GetGizmo()->GetModel();
					
					DrawCallCount++;
					RenderBuffers(VDirect3D->GetDeviceContext(), VGizmo->Chunks[0]->VVertexBuffer, VGizmo->Chunks[0]->VIndexBuffer);
					result = VShaderClass->Render(VDirect3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,VGizmo->Chunks[0]->VIndexCount,GizmoMap,0,0,0);
					VGizmo = 0;


				}
			

			}
			////SetCoreWindowRTV
			GetD3DClass()->SetWindowRenderTarget(i);

			////	RenderViewports////////

			if (ObjectCount > 0)
			{
				if (VWidgetManager->GetWidgetContainer(i)->GetViewportList().size() > 0)
				{
			
					DrawCallCount++;
					CreateViewportBuffer(i);
					RenderWidgetBuffers(VDirect3D->GetDeviceContext(), WidgetVertexBuffer, WidgetIndexBuffer);
					VWidgetShader->RenderTexturedWidget(VDirect3D->GetDeviceContext(), int(VWidgetManager->GetWidgetContainer(i)->GetViewportList().size() * 6), GetD3DClass()->GetPostProcessResourceView(0),false);

				}
			}
			UpdatePolycount();

			if (DrawInterface)
			{
				////	RenderWidgets////////
				if (VWidgetManager->GetWidgetContainer(i)->GetWidgetList().size() > 0)
				{
					DrawCallCount++;
					CreateWidgetBuffer(i);
					RenderWidgetBuffers(VDirect3D->GetDeviceContext(), WidgetVertexBuffer, WidgetIndexBuffer);
					VWidgetShader->RenderShader(VDirect3D->GetDeviceContext(), int(VWidgetManager->GetWidgetContainer(i)->GetWidgetList().size() * 6));
				}



				/////////RenderText////////
				if (VWidgetManager->GetWidgetContainer(i)->GetTextList().size() > 0)
				{

					DrawCallCount++;
					UpdateDrawCallCount();
					CreateTextBuffers(i);

					RenderWidgetBuffers(VDirect3D->GetDeviceContext(), TextVertexBuffer, TextIndexBuffer);
					VWidgetShader->RenderTexturedWidget(VDirect3D->GetDeviceContext(), int(VWidgetManager->GetWidgetContainer(i)->LetterCount * 6), FontMap, true);

				}


				/////RenderLines///

				if (VWidgetManager->GetWidgetContainer(i)->LineList.size() > 0)
				{
					DrawCallCount++;
					CreateLineListBuffer(i);
					RenderLines(VDirect3D->GetDeviceContext(), LineVertexBuffer, LineIndexBuffer);
					VLineShader->RenderShader(VDirect3D->GetDeviceContext(), int(VWidgetManager->GetWidgetContainer(i)->LineList.size() * 2));
				}
			}

		}

	}
	VDirect3D->ClearBlendState();

	VDirect3D->EndScene();

	return true;
}

void Graphics::RenderLines(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer)
{

	unsigned int stride;
	unsigned int offset;

	stride = sizeof(LinePoint);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	return;

}

bool Graphics::CreateLineListBuffer(int WindowID)
{
	bool result;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	vector <LinePoint> LineVertices;
	vector <int>Indices;
	
	
	vector<Line*>Lines = VWidgetManager->GetWidgetContainer(WindowID)->LineList;
		
	for (size_t i = 0; i < Lines.size(); i++)
	{
		LinePoint Point;
		Point.color = Lines[i]->Color;
		
		Point.position.x = Lines[i]->Points[0].x/screenWidth;
		Point.position.y = Lines[i]->Points[0].y/screenHeight;
		Point.position.z = 0;


		LineVertices.push_back(Point);

		Point.position.x = Lines[i]->Points[1].x / screenWidth;
		Point.position.y = Lines[i]->Points[1].y / screenHeight;
		Point.position.z = 0;
		LineVertices.push_back(Point);

	
	
	}
	for (size_t i = 0; i < LineVertices.size(); i++)
		Indices.push_back(int(i));


	ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
	// Set up the description of the static vertex buffer.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(LinePoint) * UINT(LineVertices.size());
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	ZeroMemory(&VertexData, sizeof(VertexData));
	VertexData.pSysMem = &LineVertices[0];
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (LineVertexBuffer)
		LineVertexBuffer->Release();

	// Now create the vertex buffer.
	result = VDirect3D->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexData, &LineVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	LineVertices.clear();

	ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
	// Set up the description of the static index buffer.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = UINT(sizeof(unsigned long) * Indices.size());
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &Indices[0];
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (LineIndexBuffer)
		LineIndexBuffer->Release();

	// Create the index buffer.
	result = VDirect3D->GetDevice()->CreateBuffer(&IndexBufferDesc, &IndexData, &LineIndexBuffer);
	return result;
}



bool Graphics::CreateViewportBuffer(int WindowID)
{
	bool result;
	D3D11_BUFFER_DESC		    VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA		VertexData, IndexData;
	vector <WidgetVertex>		WidgetVertices;
	vector <int>Indices;


	vector<Viewport*>WidgetList = VWidgetManager->GetWidgetContainer(WindowID)->GetViewportList();

	float ScreenWidth = float(VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize().x);
	float ScreenHeight = float(VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize().y);
	float ScreenRatio = ScreenWidth / ScreenHeight;



	

	int WidgetQuantity = int(WidgetList.size());

	for (int i = 0; i < WidgetQuantity; i++)
	{
		VVector2 Size = WidgetList[i]->GetSize();
		VVector2 Transform = WidgetList[i]->GetTransform();

		

			WidgetVertex Vertex;

			Vertex.position.z = 0;
			Vertex.color = WidgetList[i]->GetColor();

			//VWidgetManager->GetWidgetContainer(0)
			Vertex.position.x = Transform.x / ScreenWidth;
			Vertex.position.y = (Transform.y + Size.y) / ScreenHeight;



				Vertex.uv.x = 0;
				Vertex.uv.y = 1;

			WidgetVertices.push_back(Vertex);


			Vertex.position.x = (Transform.x + Size.x) / ScreenWidth;
			Vertex.position.y = (Transform.y + Size.y) / ScreenHeight;


			
				Vertex.uv.x = 1;
				Vertex.uv.y = 1;


			WidgetVertices.push_back(Vertex);


			Vertex.position.x = Transform.x / ScreenWidth;
			Vertex.position.y = Transform.y / ScreenHeight;

			Vertex.uv.x = 0;
			Vertex.uv.y = 0;

			WidgetVertices.push_back(Vertex);


			Vertex.position.x = (Transform.x + Size.x) / ScreenWidth;
			Vertex.position.y = Transform.y / ScreenHeight;

			Vertex.uv.x = 1;
			Vertex.uv.y = 0;

			WidgetVertices.push_back(Vertex);



			Indices.push_back(i * 4);
			Indices.push_back(i * 4 + 2);
			Indices.push_back(i * 4 + 1);


			Indices.push_back(i * 4 + 1);
			Indices.push_back(i * 4 + 2);
			Indices.push_back(i * 4 + 3);



		
	}


	ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
	// Set up the description of the static vertex buffer.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(WidgetVertex) * UINT(WidgetVertices.size());
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	ZeroMemory(&VertexData, sizeof(VertexData));
	VertexData.pSysMem = &WidgetVertices[0];
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (WidgetVertexBuffer)
		WidgetVertexBuffer->Release();

	// Now create the vertex buffer.
	result = VDirect3D->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexData, &WidgetVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	WidgetVertices.clear();

	ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
	// Set up the description of the static index buffer.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = UINT(sizeof(unsigned long) * Indices.size());
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &Indices[0];
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (WidgetIndexBuffer)
		WidgetIndexBuffer->Release();

	// Create the index buffer.
	result = VDirect3D->GetDevice()->CreateBuffer(&IndexBufferDesc, &IndexData, &WidgetIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	Indices.clear();

	return true;


}


bool Graphics::CreateWidgetBuffer(int WindowID)
{
	bool result;
	D3D11_BUFFER_DESC		    VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA		VertexData, IndexData;
	vector <WidgetVertex>		WidgetVertices;
	vector <int>Indices;

	int ScreenWidth;
	int ScreenHeight;
	
	vector<Widget*>WidgetList = VWidgetManager->GetWidgetContainer(WindowID)->GetWidgetList();

	ScreenWidth = int( VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize().x);
	ScreenHeight = int( VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize().y);

	int WidgetQuantity = int(WidgetList.size());

	for (int i = 0; i < WidgetQuantity; i++)
	{
		if (WidgetList[i]->GetIsVisible())
		{
			VVector2 Size = WidgetList[i]->GetSize();
			VVector2 Transform = WidgetList[i]->GetTransform();


			WidgetVertex Vertex;

			Vertex.position.z = 0;
			Vertex.color = WidgetList[i]->GetColor();

			Vertex.position.x = Transform.x / ScreenWidth;
			Vertex.position.y = (Transform.y + Size.y) / ScreenHeight;
			Vertex.uv.x = 0;
			Vertex.uv.y = 0;

			WidgetVertices.push_back(Vertex);


			Vertex.position.x = (Transform.x + Size.x) / ScreenWidth;
			Vertex.position.y = (Transform.y + Size.y) / ScreenHeight;
			Vertex.uv.x = 1;
			Vertex.uv.y = 0;


			WidgetVertices.push_back(Vertex);


			Vertex.position.x = Transform.x / ScreenWidth;
			Vertex.position.y = Transform.y / ScreenHeight;
			Vertex.uv.x = 0;
			Vertex.uv.y = 1;

			WidgetVertices.push_back(Vertex);


			Vertex.position.x = (Transform.x + Size.x) / ScreenWidth;
			Vertex.position.y = Transform.y / ScreenHeight;
			Vertex.uv.x = 1;
			Vertex.uv.y = 1;

			WidgetVertices.push_back(Vertex);



			Indices.push_back(i * 4);
			Indices.push_back(i * 4 + 2);
			Indices.push_back(i * 4 + 1);


			Indices.push_back(i * 4 + 1);
			Indices.push_back(i * 4 + 2);
			Indices.push_back(i * 4 + 3);




		}
	}
	
		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
		// Set up the description of the static vertex buffer.
		VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		VertexBufferDesc.ByteWidth = sizeof(WidgetVertex) * UINT(WidgetVertices.size());
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		ZeroMemory(&VertexData, sizeof(VertexData));
		VertexData.pSysMem = &WidgetVertices[0];
		VertexData.SysMemPitch = 0;
		VertexData.SysMemSlicePitch = 0;

	if(WidgetVertexBuffer)
		WidgetVertexBuffer->Release();

		// Now create the vertex buffer.
		result = VDirect3D->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexData,&WidgetVertexBuffer);
		if (FAILED(result))
		{
			return false;
		}
		WidgetVertices.clear();

		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
		// Set up the description of the static index buffer.
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = UINT (sizeof(unsigned long) * Indices.size());
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		ZeroMemory(&IndexData, sizeof(IndexData));
		IndexData.pSysMem = &Indices[0];
		IndexData.SysMemPitch = 0;
		IndexData.SysMemSlicePitch = 0;
	
		if (WidgetIndexBuffer)
		WidgetIndexBuffer->Release();

		// Create the index buffer.
		result = VDirect3D->GetDevice()->CreateBuffer(&IndexBufferDesc, &IndexData, &WidgetIndexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		
		Indices.clear();

	return true;

	
}


bool Graphics::CreateChunkBuffers(Model* srcModel)
{

	bool result;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;

	for (size_t i = 0; i < srcModel->Chunks.size(); i++)
	{

		

		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
		// Set up the description of the static vertex buffer.
		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = sizeof(VertexStruct) * UINT(srcModel->Chunks[i]->Vertices.size());
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		ZeroMemory(&VertexData, sizeof(VertexData));
		VertexData.pSysMem = &srcModel->Chunks[i]->Vertices[0];
		VertexData.SysMemPitch = 0;
		VertexData.SysMemSlicePitch = 0;


		
		// Now create the vertex buffer.
		result = VDirect3D->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexData, &srcModel->Chunks[i]->VVertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
		// Set up the description of the static index buffer.
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = sizeof(unsigned long) * srcModel->Chunks[i]->VIndexCount;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		ZeroMemory(&IndexData, sizeof(IndexData));
		IndexData.pSysMem = &srcModel->Chunks[i]->VIndexArray[0];
		IndexData.SysMemPitch = 0;
		IndexData.SysMemSlicePitch = 0;



		// Create the index buffer.
		result = VDirect3D->GetDevice()->CreateBuffer(&IndexBufferDesc, &IndexData, &srcModel->Chunks[i]->VIndexBuffer);
		if (FAILED(result))
		{
			return false;
		}

	
	}
	
	return true;

}

void Graphics::RenderInstancedStaticMeshes(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexStruct);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void Graphics::RenderWidgetBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(WidgetVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void Graphics::UpdateInterfaceBuffers(int WindowID)
{
	CreateWidgetBuffer(WindowID);
	CreateTextBuffers(WindowID);

}

bool Graphics::CreateTextBuffers(int WindowID)
{
	bool result;
	D3D11_BUFFER_DESC VertexBufferDesc, IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData, IndexData;
	vector <WidgetVertex> WidgetVertices;
	vector <int>Indices;
	float WindowWidth;
	float WindowHeight;

		
	
	vector<Text*>TextArray = VWidgetManager->GetWidgetContainer(WindowID)->GetTextList();
	WindowWidth = VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize().x;
	WindowHeight = VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize().y;

	FontType TextFont = Arial;

	float Scale = 1;
	float FontX = float(TextFont.width);
	float FontY = float(TextFont.height);
	int   TextQuantity = int(TextArray.size());
	int	  IndexStride = 0;
	float StrideMultiplier = 1.0f;
	bool  bDivideText;

	for (int i = 0; i < TextQuantity; i++)
	{
		if (TextArray[i]->GetIsVisible())
		{
			float TextWidth = 0;


			Scale = TextArray[i]->GetSize().y / TextFont.size;

			VVector2 Size = TextArray[i]->GetSize();
			VVector2 Transform = TextArray[i]->GetTransform();

			bDivideText = false;
			WidgetVertex Vertex;

			float Stride = 0;

			Vertex.position.z = 0;
			Vertex.color = TextArray[i]->GetColor();

			string Text = TextArray[i]->GetText();
			int UpperIndex = 0;

			vector<int>ASCII;
			for (size_t j = 0; j < Text.size(); j++)
			{

				ASCII.push_back(Text.at(j));
				if (isupper(Text[j]))
					UpperIndex = j;

			}

			for (int k = 0; k < ASCII.size(); k++)
			{
				TextWidth += GetCharacter(ASCII[k]).width;
			}
			if (TextWidth > Size.x)
				bDivideText = true;

			if (TextArray[i]->Alignment == AlignmentCenter)
			{

				Transform.x = Transform.x + (Size.x - TextWidth * Scale*StrideMultiplier) / 2;
			}

			float LineOffset = 0;
			for (int k = 0; k < ASCII.size(); k++)
			{
				if (k == UpperIndex && k > 0 && bDivideText)
				{
					Stride = 0;
					LineOffset += TextFont.size;
				}
				


				Character Letter = GetCharacter(ASCII[k]);
				
				float DeltaY = (TextFont.size - Letter.originY)*Scale + LineOffset;



				Vertex.position.x = float(Transform.x + Stride) / WindowWidth;
				Vertex.position.y = float(Transform.y + Letter.height*Scale + DeltaY) / WindowHeight;


				Vertex.uv.x = float(Letter.x) / FontX;
				Vertex.uv.y = float(Letter.y + Letter.height) / FontY;

				WidgetVertices.push_back(Vertex);


				Vertex.position.x = float(Transform.x + Stride + Letter.width*Scale) / WindowWidth;
				Vertex.position.y = float(Transform.y + Letter.height*Scale + DeltaY) / WindowHeight;


				Vertex.uv.x = float(Letter.x + Letter.width) / FontX;
				Vertex.uv.y = float(Letter.y + Letter.height) / FontY;


				WidgetVertices.push_back(Vertex);


				Vertex.position.x = (Transform.x + Stride) / WindowWidth;
				Vertex.position.y = (Transform.y + DeltaY) / WindowHeight;

				Vertex.uv.x = float(Letter.x) / FontX;
				Vertex.uv.y = float(Letter.y) / FontY;

				WidgetVertices.push_back(Vertex);


				Vertex.position.x = (Transform.x + Stride + float(Letter.width)*Scale) / WindowWidth;
				Vertex.position.y = (Transform.y + DeltaY) / WindowHeight;



				Vertex.uv.x = float(Letter.x + Letter.width) / FontX;
				Vertex.uv.y = float(Letter.y) / FontY;

				WidgetVertices.push_back(Vertex);



				Indices.push_back((k + IndexStride) * 4);
				Indices.push_back((k + IndexStride) * 4 + 2);
				Indices.push_back((k + IndexStride) * 4 + 1);


				Indices.push_back((k + IndexStride) * 4 + 1);
				Indices.push_back((k + IndexStride) * 4 + 2);
				Indices.push_back((k + IndexStride) * 4 + 3);

				Stride += float(Letter.width)*Scale*StrideMultiplier;


			}
			//LetterStride
			IndexStride += int(ASCII.size());
			ASCII.clear();
		}
	}
	

	ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
	// Set up the description of the static vertex buffer.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(WidgetVertex) * UINT(WidgetVertices.size());
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	ZeroMemory(&VertexData, sizeof(VertexData));
	VertexData.pSysMem = &WidgetVertices[0];
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	if (TextVertexBuffer)
		TextVertexBuffer->Release();
	
	// Now create the vertex buffer.
	result = VDirect3D->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexData, &TextVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	WidgetVertices.clear();

	ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
	// Set up the description of the static index buffer.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = UINT(sizeof(unsigned long) * Indices.size());
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	ZeroMemory(&IndexData, sizeof(IndexData));
	IndexData.pSysMem = &Indices[0];
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	if (TextIndexBuffer)
		TextIndexBuffer->Release();

	// Create the index buffer.
	result = VDirect3D->GetDevice()->CreateBuffer(&IndexBufferDesc, &IndexData, &TextIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	Indices.clear();
	
	return true;
	
	


}

void Graphics::RenderBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexStruct);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	return;
}

void Graphics::SetDirectionalLight(DirectionalLight* Light)
{
	VDirectionalLight = Light;
	VDirectionalLight->GenerateViewMatrix();
}

void Graphics::Shutdown()
{
	// Release the color shader object.
	if (VShaderClass)
	{
		VShaderClass->Shutdown();
		delete VShaderClass;
		VShaderClass = 0;
	}
	if (VLineShader)
	{

		VLineShader->ShutdownShader();
		delete VLineShader;
		VLineShader = 0;

	}
	if (VWidgetShader)
	{

		VWidgetShader->ShutdownShader();
		delete VWidgetShader;
		VWidgetShader = 0;

	}
	if (VDepthShader)
	{

		VDepthShader->ShutdownShader();
		delete VDepthShader;
		VDepthShader = 0;

	}
	

	if (VCamera)
	{
		delete VCamera;
		VCamera = 0;
	}
	// Release the Direct3D object.
	if (VDirect3D)
	{
		VDirect3D->Shutdown();
		delete VDirect3D;
		VDirect3D = 0;
	}
	// Release the Direct2D object.
	


	if (BaseColorMap)

	{
		BaseColorMap->Release();

		BaseColorMap = 0;
	}
	if (NormalMap)

	{
		NormalMap->Release();

		NormalMap = 0;
	}
	if (CubeMap)

	{
		CubeMap->Release();

		CubeMap = 0;
	}

	if (GizmoMap)

	{
		GizmoMap->Release();

		GizmoMap = 0;
	}

	if (TextVertexBuffer)
		TextVertexBuffer->Release();

	if (TextIndexBuffer)
		TextIndexBuffer->Release();

	if (WidgetVertexBuffer)
		WidgetVertexBuffer->Release();

	if (WidgetIndexBuffer)
		WidgetIndexBuffer->Release();
	if (LineIndexBuffer)
		LineIndexBuffer->Release();
	
	return;

}