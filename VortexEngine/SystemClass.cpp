#pragma warning(disable : 4244)



#pragma once


#include "stdafx.h"

#include "SystemClass.h"
#include "Widget.h"
#include "ResourceManager.h"
#include "FBXImportManager.h"
#include "Physics/Physics.h"
#include "InputBox.h"

using namespace std;

SystemClass::SystemClass()
{
	bShowCursor = true;
	mouseWheelRate = 0.1f;
	WindowX = 0;
	WindowY = 0;
	bIsBeingClosed = false;
	bPhysicsEnabled = false;
}

SystemClass::~SystemClass()
{
}



void SystemClass::HideActor(int ID)
{
	if (VResourceManager->GetWorld(0)->GetActor(ID)->bIsVisible == true)
		VResourceManager->GetWorld(0)->GetActor(ID)->bIsVisible = false;
	else
		VResourceManager->GetWorld(0)->GetActor(ID)->bIsVisible = true;

}

bool SystemClass::Initialize()
{
	int ScreenWidth, ScreenHeight;
	bool result;


	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CXSCREEN);

	CreateProjectFiles();

	if (ProjectDirectory != "")
		
	InitializeWindows(ScreenWidth, ScreenHeight);
	
	VInput = new Input;
	if (!VInput)
	{
		return false;
	}

	VInput->Initialize();

	
	VDebugger = new SystemDebugger;
	if (!VDebugger)
	{
		return false;
	}

	VResourceManager = new ResourceManager;
	if(!VResourceManager)
	{
		return false;
	
	}


	VGraphics = new Graphics;
	if (!VGraphics)
	{
		return false;
	}
	

	VResourceManager->CreateWorld();

	result = VGraphics->Initialize(ScreenWidth, ScreenHeight, Vhwnd);
	if (!result)
	{
		
		return false;
	}
	
	VGraphics->SetResourceManager(VResourceManager);

	ScreenResX = ScreenWidth;
	ScreenResY = ScreenHeight;

	CreateWidgetManager();

	
	VWidgetManager->GetWidgetContainer(0)->SetWindowSize(VVector2(ScreenWidth, ScreenHeight));
	VWidgetManager->GetWidgetContainer(0)->SetWindowTransform(VVector2(0,0));
	
	
	VGraphics->UpdateInterfaceBuffers(0);

	VMaterialMaster = new MaterialMaster;
	if (!VMaterialMaster)
	{
		return false;
	}
	
	VGraphics->VMaterialMaster = VMaterialMaster;
	
	
	CreateDirectionalLight();
	//CreateViewport();
	//OpenMaterialEditor();
	

	//CreateGrid();
	CreateGizmo();
	//GenerateTerrain();

	LoadFilesFromDirectory(ProjectDirectory.c_str());

	ResizeViewport();
	

	return true;
}

void  SystemClass::CloseWindow()
{
	
	int WindowID = GetSelectedWindow();
	VWidgetManager->GetWidgetContainer(WindowID)->SetIsBeingDestroyed(true);
	DestroyWindow(VWindows[WindowID]);
	VWindows.erase(VWindows.begin() + WindowID);
	
}

SystemClass* SystemClass::GetSystem()
{

	return ApplicationHandle;

}

void SystemClass::CreateNewProject()
{


}
ResourceManager* SystemClass::GetResoruceManager()
{
	if (VResourceManager)
		return VResourceManager;
	else return 0;

}
void SystemClass::OpenMaterialEditor()
{
	int WindowWidth = GetSystemMetrics(SM_CXSCREEN);
	int WindowHeight = GetSystemMetrics(SM_CXSCREEN);
	WindowHeight = 600;
	WindowWidth = 800;
	



	int posX = (GetSystemMetrics(SM_CXSCREEN) - WindowWidth) / 2;
	int posY = (GetSystemMetrics(SM_CXSCREEN) - WindowHeight) / 2;

HWND VMaterialHwnd = CreateWindowEx(WS_EX_APPWINDOW, L"Vortex Engine", L"Material Master",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, WindowWidth, WindowHeight,NULL, NULL, Vhinstance, NULL);

	// Bring the window up on the Screen and set it as main focus.
	ShowWindow(VMaterialHwnd, SW_SHOW);
	SetForegroundWindow(VMaterialHwnd);
	SetFocus(VMaterialHwnd);

	VWindows.push_back(VMaterialHwnd);

	VWidgetManager->CreateWidgetContainer("MaterialEditor");
	bool bCreated =  VGraphics->GetD3DClass()->CreateSwapChain(VMaterialHwnd);
	VWidgetManager->CreateMaterialEditor(GetWindowID(VMaterialHwnd));
	VWidgetManager->GetWidgetContainer(GetWindowID(VMaterialHwnd))->SetWindowSize(VVector2(WindowWidth, WindowHeight));
	VMaterialMaster->Initialize(VWidgetManager,VInput);

	UpdateInterface();
	
	return;
	
}



int SystemClass::GetWindowID(HWND Window)
{

		for (int i = 0; i < VWindows.size(); i++)
			if (Window == VWindows[i])
				return i;
		return 0;

}

int SystemClass::GetSelectedWindow()
{
	HWND Window = GetActiveWindow();
	for (int i = 0; i < VWindows.size(); i++)
		if (Window == VWindows[i])
			return i;
	return 0;
}

void SystemClass::ResizeViewport()
{
	
	RECT TaskBar = GetWindowsTaskBar();
	
	int WindowX = GetSystemMetrics(0);
	int WindowY = GetSystemMetrics(1);
	SetWindowPos(GetActiveWindow(),HWND_TOP,0,0, WindowX, WindowY - (TaskBar.bottom - TaskBar.top),NULL);
	VWidgetManager->GetWidgetContainer(GetSelectedWindow())->SetWindowSize(VVector2(WindowX,WindowY - (TaskBar.bottom - TaskBar.top)));
	VWidgetManager->ResizeWindow(GetSelectedWindow());
	bIsFullScreen = true;
	VGraphics->UpdateInterfaceBuffers(GetSelectedWindow());
	return;
}
void SystemClass::Play()
{
	CreateViewport();

}

RECT SystemClass::GetWindowsTaskBar()
{
	RECT rect;
	HWND taskBar = FindWindow(L"Shell_traywnd", NULL);
	if (taskBar && GetWindowRect(taskBar, &rect)) {
		return rect;
	}
	else return rect;
}


void SystemClass::DragViewport()
{

	
	LPPOINT mousePoint;
	mousePoint = nullptr;
	mousePoint = VInput->GetMouseInput();
	
	int X = mousePoint->x;
	int Y = mousePoint->y;
			
	
		MoveWindow(GetActiveWindow(), X - WindowX, Y - WindowY, ScreenResX, ScreenResY, false);
		VWidgetManager->ResizeWindow(GetSelectedWindow());
		
	}


void SystemClass::CreateGizmo()
{

	VResourceManager->CreateGizmo();

	FBXImportManager FBXImportM;

	Model* VModel;

	VModel = FBXImportM.InitializeFBXManager("Content/Engine/GizmoTransform.fbx");
	if (VModel)
	{
		VModel->SetName("GizmoTranslation");
		StaticMesh* VStaticMesh = new StaticMesh;
		VStaticMesh->SetModel(VModel);
		VGraphics->CreateChunkBuffers(VModel);
		VResourceManager->GetGizmo()->AddMesh(VStaticMesh);
	}
	VModel = 0;

	VModel = FBXImportM.InitializeFBXManager("Content/Engine/GizmoRotator.fbx");
	if (VModel)
	{
		VModel->SetName("GizmoRotation");
		StaticMesh* VStaticMesh = new StaticMesh;
		VStaticMesh->SetModel(VModel);
		VGraphics->CreateChunkBuffers(VModel);
		VResourceManager->GetGizmo()->AddMesh(VStaticMesh);
	}

	VModel = 0;
	

	VModel = FBXImportM.InitializeFBXManager("Content/Engine/GizmoScale.fbx");
	if (VModel)
	{
		VModel->SetName("GizmoScale");
		StaticMesh* VStaticMesh = new StaticMesh;
		VStaticMesh->SetModel(VModel);
		VGraphics->CreateChunkBuffers(VModel);
		VResourceManager->GetGizmo()->AddMesh(VStaticMesh);
	}

	VModel = 0;


}

void SystemClass::CreateGrid()
{
	VResourceManager->CreateGizmo();

	FBXImportManager FBXImportM;

	Model* VModel;


	VModel = FBXImportM.InitializeFBXManager("Content/Engine/Grid.fbx");
	if (VModel)
	{
		VModel->SetName("Grid");
		StaticMesh* VStaticMesh = new StaticMesh;
		VStaticMesh->SetModel(VModel);
		VGraphics->CreateChunkBuffers(VModel);
		VResourceManager->GetWorld(0)->CreateStaticMeshActor(VStaticMesh);
	}
	
	VModel = 0;


}

void SystemClass::CreateWidgetManager()
{
	
	VWidgetManager = new WidgetManager;
	VWidgetManager->Initialize();
	VGraphics->SetWidgetManager(VWidgetManager);
	return;
}
void SystemClass::GenerateTerrain()
{
	

	Terrain VTerrain;
	StaticMesh* TerrainMesh = VTerrain.CreateTerrain(1,1,1);
	

	VGraphics->CreateChunkBuffers(TerrainMesh->GetModel());
	VResourceManager->AddStaticMesh(TerrainMesh);
	VResourceManager->GetWorld(0)->CreateStaticMeshActor(VResourceManager->GetStaticMeshByName(TerrainMesh->GetName()));
	VResourceManager->GetWorld(0)->UpdateStaticMeshPolycount();
	
	UpdateOutliner();
	
	UpdateInterface();
	
	return;

	
}

void SystemClass::Shutdown()
{
	if (VWidgetManager)
	{
		VWidgetManager->Shutdown();
		delete VWidgetManager;
		VWidgetManager = 0;
		

	}

	// Release the input object.
	if (VInput)
	{
		delete VInput;
		VInput = 0;
	}

	if (VDebugger)
	{
		delete VDebugger;
		VDebugger = 0;
	}
	if (VResourceManager)
	{
		VResourceManager->Shutdown();
		delete VResourceManager;
		VResourceManager = 0;
	}
	
	if (VGraphics)
	{
		VGraphics->Shutdown();
		delete VGraphics;
		VGraphics = 0;
	}

	if (VMaterialMaster)
	{
		delete VMaterialMaster;
		VMaterialMaster = 0;
	}

	for (size_t i = 0; i < VWindows.size(); i++)
	{
		DestroyWindow(VWindows[i]);
		VWindows[i] = NULL;
	}
	VWindows.clear();
	
		ShutdownWindows();



	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			if (VWindows.size() == 0)
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}

void SystemClass::AddObjectToWorld()
{
	string AssetName;
	

	int mouseX, mouseY;
	LPPOINT mousePoint;
	mousePoint = nullptr;

	mousePoint = VInput->GetMouseInput();
	if (mousePoint)
	{
		ScreenToClient(GetActiveWindow(), mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;
	}

	Widget* OverWidget;
	OverWidget = 0;

	OverWidget = (VWidgetManager->GetClickedWidget(mouseX, mouseY, GetSelectedWindow()));
	if (OverWidget)
	{
		AssetName = OverWidget->GetName();
		StaticMesh* Mesh = VResourceManager->GetStaticMeshByName(AssetName);
		if (Mesh)
		{
			VResourceManager->GetWorld(0)->CreateStaticMeshActor(Mesh);
			VResourceManager->GetWorld(0)->UpdateStaticMeshPolycount();
		
		UpdateOutliner();
		}
	}
	return;

}



 POINT  SystemClass::GetCursorPosition()
{
	 
	 POINT Point;

	 GetCursorPos(&Point);
	
	return Point;
}


void SystemClass::SelectActor(int ID)
{
	
	VResourceManager->GetWorld(0)->SelectActor(VResourceManager->GetWorld(0)->VActors[ID],!isLeftCtrDown);
	for (size_t i = 0; i < VResourceManager->GetWorld(0)->VActors.size(); i++)
	{
		if (VResourceManager->GetWorld(0)->VActors[i]->GetName() == "")
		{
		}
	}
}



bool SystemClass::Frame()
{
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if (VInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	if (bIsBeingClosed)
	{
		return false;
	}
	
	int mouseX, mouseY;
	LPPOINT mousePoint;

	
	VVector   Vector,MoveVector;
	Vector = VVector(0, 0, 0);


	mousePoint = nullptr;

	mousePoint = VInput->GetMouseInput();
	if (mousePoint)
	{
		
		ScreenToClient(GetActiveWindow(), mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;
	}
	else
	{
		mouseX = 0;
		mouseY = 0;
	}

	if (isDragging || isMiddleDown || isLeftDown || isRightDown)
		if (!isDraggingViewport)
		{
			VVector2 WindowSize = VWidgetManager->GetWidgetContainer(GetSelectedWindow())->GetWindowSize();
			if (mouseY > WindowSize.y || mouseY < 0 || mouseX > WindowSize.x || mouseX < 0)
				ReleaseInput();
		}
	 
	if (VMaterialMaster->IsBinding && VMaterialMaster->ActiveLink)
	{
		VMaterialMaster->ActiveLink->Points[1] = VVector(mouseX, mouseY, 0);
	}

	if (isDragging)
	{	
		VVector2 ParentTransform = VWidgetManager->DragWidget->GetParentTransform();

		if (VWidgetManager->DragWidget->GetIsDragXLocked())
			VWidgetManager->DragWidget->SetRelativeTransform(VWidgetManager->DragWidget->GetRelativeTransform().x, mouseY - ParentTransform.y - VWidgetManager->DragWidget->GetSize().y / 2);
		else if (VWidgetManager->DragWidget->GetIsDragYLocked())
			VWidgetManager->DragWidget->SetRelativeTransform(mouseX - ParentTransform.x - VWidgetManager->DragWidget->GetSize().x / 2, VWidgetManager->DragWidget->GetRelativeTransform().y);
		else
			VWidgetManager->DragWidget->SetRelativeTransform(mouseX - ParentTransform.x - VWidgetManager->DragWidget->GetSize().x / 2, mouseY - ParentTransform.y - VWidgetManager->DragWidget->GetSize().y / 2);
	
		VWidgetManager->DragWidget->OnDrag();
		
		
		VWidgetManager->ResizeWindow(GetSelectedWindow());
		UpdateInterface();
		VMaterialMaster->UpdateLinks();
		
		
		/*if (mouseY < ScreenResY - 100 && GetSelectedWindow()==0)
		{
			
			AddObjectToWorld();
			//ClearDrag();
			
		}*/
		
	}
	else
	{
		Widget* OverWidget;
		OverWidget = 0;

		OverWidget = (VWidgetManager->GetClickedWidget(mouseX, mouseY,GetSelectedWindow()));
		if (VWidgetManager->ActiveWidget != OverWidget)
		{
			if (VWidgetManager->ActiveWidget)
				VWidgetManager->ActiveWidget->OnMouseOverEnd();
			if (OverWidget)
			{
				VWidgetManager->ActiveWidget = OverWidget;
				OverWidget->OnMouseOver();
			}
			else VWidgetManager->ActiveWidget = 0;

		}
	
	}


	if (isMiddleDown)
	{
		float deltaX = cursorPos.x - mouseX;
		if (deltaX != 0)
			{
			float speed = 0.03f*deltaX;

			MoveVector = VGraphics->VCamera->GetLeftVector();
			Vector.x -= MoveVector.x*speed;
			Vector.y -= MoveVector.y*speed;
			Vector.z -= MoveVector.z*speed;
			cursorPos.x = mouseX;

			VVector Pos = VGraphics->VCamera->GetPosition();

			VGraphics->VCamera->SetPosition(Pos.x + Vector.x, Pos.y + Vector.y, Pos.z + Vector.z);
			return true;
			}
			float deltaY = cursorPos.y - mouseY;
		if (deltaY != 0)
		{
			float speed = 0.03f*deltaY;

			MoveVector = VGraphics->VCamera->GetUpVector();
			Vector.x -= MoveVector.x*speed;
			Vector.y -= MoveVector.y*speed;
			Vector.z -= MoveVector.z*speed;
			cursorPos.y = mouseY;

			VVector Pos = VGraphics->VCamera->GetPosition();

			VGraphics->VCamera->SetPosition(Pos.x + Vector.x, Pos.y + Vector.y, Pos.z + Vector.z);
			
		}

	}
		 
	
	


	
	float degree;
	float time;
	time = clock();

	degree = time / 100;
	
	if(isAKey|| isDKey|| isSKey|| isWKey)
	{ 
		POINT Point;
		float mouseX, mouseY;

		GetCursorPos(&Point);
		mouseX = cursorPos.x - Point.x;
		mouseY = cursorPos.y - Point.y;

		VVector Vector, MoveVector;
		Vector = VVector(0, 0, 0);

		if (isAKey)
		{
			MoveVector = VGraphics->VCamera->GetLeftVector();
			Vector.x += MoveVector.x;
			Vector.y += MoveVector.y;
			Vector.z += MoveVector.z;

		}
		else if (isDKey)
		{
			MoveVector = VGraphics->VCamera->GetRightVector();
			Vector.x += MoveVector.x;
			Vector.y += MoveVector.y;
			Vector.z += MoveVector.z;
		}
		else if (isWKey)
		{
			MoveVector = VGraphics->VCamera->GetForwardVector();
			Vector.x += MoveVector.x;
			Vector.y += MoveVector.y;
			Vector.z += MoveVector.z;
		}
		else if (isSKey)
		{
			MoveVector = VGraphics->VCamera->GetForwardVector();
			Vector.x -= MoveVector.x;
			Vector.y -= MoveVector.y;
			Vector.z -= MoveVector.z;

		}
		VVector Pos = VGraphics->VCamera->GetPosition();
		

		VGraphics->VCamera->SetPosition(Pos.x+ Vector.x, Pos.y + Vector.y, Pos.z + Vector.z);
		cursorPos = Point;
	}



	if (isLeftDown)
	{
		if (isDraggingViewport)
		{

			DragViewport();
			
		}
		if (VResourceManager->GetGizmo())
		{
			VAxis ActiveAxis = VResourceManager->GetGizmo()->GetAxis();
			VGizmoState GizmoState = VResourceManager->GetGizmo()->GetState();
			if (bIsGizmoActive)
			{
				//VResourceManager->GetGizmo()->SetRotation(VResourceManager->GetWorld(0)->GetSelectedActors()[0]->GetRotation());


				if (GizmoState == VGizmoTranslation)
				{
					if (ActiveAxis == VAxisX)
					{


						float deltaX = cursorPos.x - mouseX;


						if (deltaX != 0)
						{

							float speed = 0.01f*deltaX * VGraphics->GizmoScale;

							MoveVector = VGraphics->VCamera->GetLeftVector();

							cursorPos.x = mouseX;


							VVector WorldVector;
							WorldVector.x = 1;
							WorldVector.y = 0;
							WorldVector.z = 0;
							VVector CameraVector;
							CameraVector.x = MoveVector.x;
							CameraVector.y = MoveVector.y;
							CameraVector.z = MoveVector.z;

							float Scale = VectorDot(CameraVector, WorldVector);
							for (int i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActorsCount(); i++)
							{
								VVector ActorPos = VResourceManager->GetWorld(0)->GetSelectedActor(i)->GetPosition();
								VResourceManager->GetWorld(0)->GetSelectedActor(i)->SetPosition(ActorPos.x + (Scale*speed), ActorPos.y, ActorPos.z);

							}

						}
					}
					if (ActiveAxis == VAxisY)
					{
						float deltaX = cursorPos.x - mouseX;
						if (deltaX != 0)
						{
							float speed = 0.01f*deltaX* VGraphics->GizmoScale;

							MoveVector = VGraphics->VCamera->GetLeftVector();

							cursorPos.x = mouseX;



							VVector WorldVector;
							WorldVector.x = 0;
							WorldVector.y = 1;
							WorldVector.z = 0;
							VVector CameraVector;
							CameraVector.x = MoveVector.x;
							CameraVector.y = MoveVector.y;
							CameraVector.z = MoveVector.z;
							float Scale = VectorDot(CameraVector, WorldVector);




							for (int i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActorsCount(); i++)
							{
								VVector ActorPos = VResourceManager->GetWorld(0)->GetSelectedActor(i)->GetPosition();
								VResourceManager->GetWorld(0)->GetSelectedActor(i)->SetPosition(ActorPos.x, ActorPos.y + (Scale*speed), ActorPos.z);

							}
						}

					}
					if (ActiveAxis == VAxisZ)
					{
						float deltaY = cursorPos.y - mouseY;
						if (deltaY != 0)
						{
							float speed = 0.01f*deltaY*VGraphics->GizmoScale;

							MoveVector = VGraphics->VCamera->GetUpVector();

							cursorPos.y = mouseY;


							VVector WorldVector;
							WorldVector.x = 0;
							WorldVector.y = 0;
							WorldVector.z = 1;

							VVector CameraVector;
							CameraVector.x = MoveVector.x;
							CameraVector.y = MoveVector.y;
							CameraVector.z = MoveVector.z;

							float Scale = VectorDot(CameraVector, WorldVector);
							for (int i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActorsCount(); i++)
							{
								VVector ActorPos = VResourceManager->GetWorld(0)->GetSelectedActor(i)->GetPosition();
								VResourceManager->GetWorld(0)->GetSelectedActor(i)->SetPosition(ActorPos.x, ActorPos.y, ActorPos.z + (Scale*speed));
							}
						}

					}
				}
				if (GizmoState == VGizmoRotation)
				{

					if (ActiveAxis == VAxisX)
					{
						VVector GizmoPivot = VResourceManager->GetGizmo()->GetPosition();
						float GizmoAngle = VResourceManager->GetGizmo()->GetRotationAngle();
						VVector Intersection = LinePlaneTrace(GetUnprojectedVector(), VGraphics->VCamera->GetPosition(), GizmoPivot, VAxisX);


						VVector VectorCB = Intersection - GizmoPivot;

						//float AB = atan2(VectorCB.z, VectorCB.y);
						float CB = atan2(VectorCB.z, VectorCB.y);

						int Angle = (CB * 360 / 6.283185482) + GizmoAngle;


						for (size_t i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActors().size(); i++)
						{
							VRotation Rotation = VResourceManager->GetWorld(0)->GetSelectedActors()[i]->GetRotation();
							VResourceManager->GetWorld(0)->GetSelectedActors()[i]->SetRotation(Rotation.Pitch, Angle, Rotation.Yaw);
						}
						//float DotProduct = VectorDot(LaneAB, LaneCB);
						//float Cross = VectorCross(LaneAB, LaneCB);
						//float Alpha = atan2(Cross, DotProduct);
					}


					if (ActiveAxis == VAxisY)
					{
						VVector GizmoPivot = VResourceManager->GetGizmo()->GetPosition();
						float GizmoAngle = VResourceManager->GetGizmo()->GetRotationAngle();
						VVector Intersection = LinePlaneTrace(GetUnprojectedVector(), VGraphics->VCamera->GetPosition(), GizmoPivot, VAxisY);


						VVector VectorCB = Intersection - GizmoPivot;


						float CB = atan2(VectorCB.z, VectorCB.x);

						int Angle = (CB * (-360) / 6.283185482) + GizmoAngle;

						for (size_t i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActors().size(); i++)
						{
							VRotation Rotation = VResourceManager->GetWorld(0)->GetSelectedActors()[i]->GetRotation();
							VResourceManager->GetWorld(0)->GetSelectedActors()[i]->SetRotation(Angle, Rotation.Roll, Rotation.Yaw);
						}


					}
					if (ActiveAxis == VAxisZ)
					{
						VVector GizmoPivot = VResourceManager->GetGizmo()->GetPosition();
						float GizmoAngle = VResourceManager->GetGizmo()->GetRotationAngle();
						VVector Intersection = LinePlaneTrace(GetUnprojectedVector(), VGraphics->VCamera->GetPosition(), GizmoPivot, VAxisZ);


						VVector VectorCB = Intersection - GizmoPivot;


						float CB = atan2(VectorCB.y, VectorCB.x);

						int Angle = (CB * (360) / 6.283185482) + GizmoAngle;

						for (size_t i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActors().size(); i++)
						{
							VRotation Rotation = VResourceManager->GetWorld(0)->GetSelectedActors()[i]->GetRotation();
							VResourceManager->GetWorld(0)->GetSelectedActors()[i]->SetRotation(Rotation.Pitch, Rotation.Roll, Angle);
						}
					}

				}
				if (GizmoState == VGizmoScale)
				{
					if (ActiveAxis == VAxisX)
					{


						float deltaX = cursorPos.x - mouseX;


						if (deltaX != 0)
						{

							float speed = 0.01f*deltaX * VGraphics->GizmoScale;

							MoveVector = VGraphics->VCamera->GetLeftVector();

							cursorPos.x = mouseX;


							VVector WorldVector;
							WorldVector.x = 1;
							WorldVector.y = 0;
							WorldVector.z = 0;
							VVector CameraVector;
							CameraVector.x = MoveVector.x;
							CameraVector.y = MoveVector.y;
							CameraVector.z = MoveVector.z;

							float Scale = VectorDot(CameraVector, WorldVector);
							for (int i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActorsCount(); i++)
							{
								VVector ActorScale = VResourceManager->GetWorld(0)->GetSelectedActor(i)->GetScale();
								VResourceManager->GetWorld(0)->GetSelectedActor(i)->SetScale(ActorScale.x + (Scale*speed), ActorScale.y,ActorScale.z);

							}

						}
					}
					if (ActiveAxis == VAxisY)
					{
						float deltaX = cursorPos.x - mouseX;
						if (deltaX != 0)
						{
							float speed = 0.01f*deltaX* VGraphics->GizmoScale;

							MoveVector = VGraphics->VCamera->GetLeftVector();

							cursorPos.x = mouseX;



							VVector WorldVector;
							WorldVector.x = 0;
							WorldVector.y = 1;
							WorldVector.z = 0;
							VVector CameraVector;
							CameraVector.x = MoveVector.x;
							CameraVector.y = MoveVector.y;
							CameraVector.z = MoveVector.z;
							float Scale = VectorDot(CameraVector, WorldVector);




							for (int i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActorsCount(); i++)
							{
								VVector ActorScale = VResourceManager->GetWorld(0)->GetSelectedActor(i)->GetScale();
								VResourceManager->GetWorld(0)->GetSelectedActor(i)->SetScale(ActorScale.x, ActorScale.y - (Scale*speed), ActorScale.z);

							}
						}

					}
					if (ActiveAxis == VAxisZ)
					{
						float deltaY = cursorPos.y - mouseY;
						if (deltaY != 0)
						{
							float speed = 0.01f*deltaY*VGraphics->GizmoScale;

							MoveVector = VGraphics->VCamera->GetUpVector();

							cursorPos.y = mouseY;


							VVector WorldVector;
							WorldVector.x = 0;
							WorldVector.y = 0;
							WorldVector.z = 1;

							VVector CameraVector;
							CameraVector.x = MoveVector.x;
							CameraVector.y = MoveVector.y;
							CameraVector.z = MoveVector.z;

							float Scale = VectorDot(CameraVector, WorldVector);
							for (int i = 0; i < VResourceManager->GetWorld(0)->GetSelectedActorsCount(); i++)
							{
								VVector ActorScale = VResourceManager->GetWorld(0)->GetSelectedActor(i)->GetScale();
								VResourceManager->GetWorld(0)->GetSelectedActor(i)->SetScale(ActorScale.x, ActorScale.y, ActorScale.z + (Scale*speed));
							}
						}

					}
				}



			}
		}
	}

	
	if (isRightDown)
	{
		//CameraOrbit
		/*
		POINT Point;
		float x;
		float y;
		float Multiplier;
		Multiplier = 0.5f;

		GetCursorPos(&Point);
		x = cursorPos.x - Point.x;
		y = cursorPos.y - Point.y;
		XMFLOAT3 rot = VGraphics->VCamera->GetRotation();
		rot.z = -x* Multiplier + rot.z;
		rot.x = y* Multiplier + rot.x;
		VGraphics->VCamera->SetRotation(0,0,rot.z);
		VGraphics->VCamera->UpdateTransforms();
		cursorPos = Point;
		*/

		POINT Point;
		float x;
		float y;
		float Multiplier;
		Multiplier = 0.075f;

		GetCursorPos(&Point);
		x = cursorPos.x - Point.x;
		y = cursorPos.y - Point.y;
		VRotation rot = VGraphics->VCamera->GetRotation();


		rot.Yaw = -x* Multiplier + rot.Yaw;
		rot.Pitch = y* Multiplier + rot.Pitch;

		VGraphics->VCamera->SetRotation(rot.Pitch, 0, rot.Yaw);

		cursorPos = Point;
		//SetConsoleCursorPosition(output, pos);
		//SetCursorPos(cursorPos.x,cursorPos.y);

	}

	

	result = VGraphics->Frame();
	if (!result)
	{
		return false;
	}

	for each (Actor* Actor in VResourceManager->GetWorld(0)->VActors)
	{
		Actor->Update();
	}

	if (bPhysicsEnabled)
	UpdatePhysics();



	return true;
}

void SystemClass::UpdatePhysics()
{
	for (int i = 0; i < VResourceManager->GetWorld(0)->VActors.size(); i++)
	{
		bool DoesCollide;

		StaticMeshActor* CollisionActor = dynamic_cast<StaticMeshActor*>(VResourceManager->GetWorld(0)->GetActor(i));
		if (CollisionActor)

			

			for (size_t y = 0; y < VResourceManager->GetWorld(0)->VActors.size(); y++)
			{
				DoesCollide = false;
				
				StaticMeshActor* OtherCollisionActor = dynamic_cast<StaticMeshActor*>(VResourceManager->GetWorld(0)->GetActor(y));
				if (OtherCollisionActor)
					if (OtherCollisionActor != CollisionActor)
					{
						VVector BodyPositon = CollisionActor->GetPosition() + CollisionActor->Mesh->GetModel()->GeometryCenter;
						BoundingBox CollisionBox = CollisionActor->Mesh->GetModel()->GetBoundingBox();



						VVector OtherBodyPositon = OtherCollisionActor->GetPosition() + CollisionActor->Mesh->GetModel()->GeometryCenter;
						BoundingBox OtherCollisionBox = OtherCollisionActor->Mesh->GetModel()->GetBoundingBox();
						float ColisionBias = 0.01f;
						
						//
						if (!(
							BodyPositon.x + CollisionBox.MinX - ColisionBias > OtherBodyPositon.x + OtherCollisionBox.MaxX ||
							BodyPositon.x + CollisionBox.MaxX + ColisionBias < OtherBodyPositon.x + OtherCollisionBox.MinX ||

							BodyPositon.y + CollisionBox.MinY - ColisionBias > OtherBodyPositon.y + OtherCollisionBox.MaxY ||
							BodyPositon.y + CollisionBox.MaxY + ColisionBias < OtherBodyPositon.y + OtherCollisionBox.MinY ||

							BodyPositon.z + CollisionBox.MinZ - ColisionBias > OtherBodyPositon.z + OtherCollisionBox.MaxZ ||
							BodyPositon.z + CollisionBox.MaxZ + ColisionBias < OtherBodyPositon.z + OtherCollisionBox.MinZ
							))
						{
							DoesCollide = true;
							break;
						}
						
						
						
					}
					
				
			}
		if (DoesCollide == false)
			CollisionActor->UpdatePhysics();
		else
			CollisionActor->SetVelocity(VVector(0, 0, 0));


	}
	
	
}


void SystemClass::UpdateOutliner()
{
	VWidgetManager->UpdateWorldOutliner(VResourceManager->GetWorld(0));
	
}


void SystemClass::MouseIntersection()
{
	VVector UnprojectedVector = GetUnprojectedVector();
	VVector ViewPosition = VVector(VGraphics->VCamera->GetPosition().x, VGraphics->VCamera->GetPosition().y, VGraphics->VCamera->GetPosition().z);


		if (VResourceManager->GetWorld(0)->GetSelectedActorsCount()>0)
			{
				float Radius = 0.4f;
				
			
				VGizmoState State = VResourceManager->GetGizmo()->GetState();
				if (State == VGizmoRotation)
				{
					VRotation GizmoRotation = VResourceManager->GetGizmo()->GetRotation();

					if (VResourceManager->GetGizmo()->RotationTrace(ViewPosition, GetUnprojectedVector(), VAxisX))
					{
						VVector GizmoPivot = VResourceManager->GetGizmo()->GetPosition();
						
						VVector GizmoIntersectionPoint = LinePlaneTrace(GetUnprojectedVector(), VGraphics->VCamera->GetPosition(), GizmoPivot, VAxisX);
						
						
						float GizmoAngle = 0;
						VVector VectorAB = GizmoIntersectionPoint - GizmoPivot;
						GizmoAngle = atan2(VectorAB.y, VectorAB.z) * 360 / (2 * PI);
						VResourceManager->GetGizmo()->SetRotationAngle(GizmoAngle);

						VResourceManager->GetGizmo()->SetAxis(VAxisX);
						bIsGizmoActive = true;
						OutputDebugStringA("X Axis");
						return;
					}
					if (VResourceManager->GetGizmo()->RotationTrace(ViewPosition, GetUnprojectedVector(), VAxisY))
					{
						float GizmoAngle = 0;
						VVector GizmoPivot = VResourceManager->GetGizmo()->GetPosition();
						VVector GizmoIntersectionPoint = LinePlaneTrace(GetUnprojectedVector(), VGraphics->VCamera->GetPosition(), GizmoPivot, VAxisY);

						VVector VectorAB = GizmoIntersectionPoint - GizmoPivot;
						GizmoAngle = atan2(VectorAB.z, VectorAB.x) * 360 / (2 * PI);

						VResourceManager->GetGizmo()->SetRotationAngle(GizmoAngle);

						VResourceManager->GetGizmo()->SetAxis(VAxisY);
						bIsGizmoActive = true;
						OutputDebugStringA("Y Axis");
						return;
					}
					if (VResourceManager->GetGizmo()->RotationTrace(ViewPosition, GetUnprojectedVector(), VAxisZ))
					{
						float GizmoAngle = 0;
						VVector GizmoPivot = VResourceManager->GetGizmo()->GetPosition();

							VVector GizmoIntersectionPoint = LinePlaneTrace(GetUnprojectedVector(), VGraphics->VCamera->GetPosition(), GizmoPivot, VAxisZ);
					
							VVector VectorAB = GizmoIntersectionPoint - GizmoPivot;
							GizmoAngle = atan2(VectorAB.x, VectorAB.y)*360/(2*PI);
							VResourceManager->GetGizmo()->SetRotationAngle(GizmoAngle);

						VResourceManager->GetGizmo()->SetAxis(VAxisZ);
						bIsGizmoActive = true;
						OutputDebugStringA("Z Axis");
						return;
					}
				}
				VVector ObjectPivot;
				VVector Position = VResourceManager->GetGizmo()->GetPosition();
				ObjectPivot.x = Position.x + 1*VGraphics->GizmoScale;
				ObjectPivot.y = Position.y;
				ObjectPivot.z = Position.z;

				if (LineSphereTrace(UnprojectedVector, ViewPosition, Radius* VGraphics->GizmoScale, ObjectPivot))
				{
					VResourceManager->GetGizmo()->SetAxis(VAxisX);
					bIsGizmoActive = true;
	
					return;
				}

				ObjectPivot.x = Position.x;
				ObjectPivot.y = Position.y - 1 * VGraphics->GizmoScale;
				ObjectPivot.z = Position.z;

				if (LineSphereTrace(UnprojectedVector, ViewPosition, Radius * VGraphics->GizmoScale, ObjectPivot))
				{
					VResourceManager->GetGizmo()->SetAxis(VAxisY);
					bIsGizmoActive = true;
					return;
				}
			
				ObjectPivot.x = Position.x;
				ObjectPivot.y = Position.y;
				ObjectPivot.z = Position.z + 1 * VGraphics->GizmoScale;

				
				if (LineSphereTrace(UnprojectedVector, ViewPosition, Radius* VGraphics->GizmoScale, ObjectPivot))
				{
					VResourceManager->GetGizmo()->SetAxis(VAxisZ);
					bIsGizmoActive = true;
					return;
				}

				


			}
			
			
			for (size_t i = 0; i < VResourceManager->GetWorld(0)->VActors.size(); i++)
		{
				if (VResourceManager->GetWorld(0)->VActors[i]->bIsVisible)
				{
					float Radius;
					VVector ObjectPivot;

					if (dynamic_cast<StaticMeshActor*>(VResourceManager->GetWorld(0)->VActors[i]) != nullptr)
					{
						Radius = dynamic_cast<StaticMeshActor*>(VResourceManager->GetWorld(0)->VActors[i])->Mesh->GetSphereBoundsRadius();
					
					
		
					ObjectPivot = dynamic_cast<StaticMeshActor*>(VResourceManager->GetWorld(0)->VActors[i])->Mesh->GetModel()->GeometryCenter;

					ObjectPivot += VResourceManager->GetWorld(0)->VActors[i]->GetPosition();

					}
					else
					{
						Radius = 0.1;
					    ObjectPivot = VResourceManager->GetWorld(0)->VActors[i]->GetPosition();
					}

					if (LineSphereTrace(UnprojectedVector, ViewPosition, Radius, ObjectPivot))
					{
						if (isLeftCtrDown)
						{
							VResourceManager->GetWorld(0)->SelectActor(VResourceManager->GetWorld(0)->VActors[i], false);
							OutputDebugStringA("MultiSelect");
							return;
						}
						else
						{
							VResourceManager->GetWorld(0)->SelectActor(VResourceManager->GetWorld(0)->VActors[i], true);
							return;
						}
					}
				
				int BufferX = VGraphics->GetD3DClass()->GetBackBufferSize(0);
				int BufferY = VGraphics->GetD3DClass()->GetBackBufferSize(1);
				
				POINT Point;
				GetCursorPos(&Point);
				ScreenToClient(GetActiveWindow(), &Point);
				
				VVector2 WindowTransform = VWidgetManager->GetWidgetContainer((GetSelectedWindow()))->GetViewportList()[0]->GetTransform();
				VVector2 WindowSize = VWidgetManager->GetWidgetContainer((GetSelectedWindow()))->GetViewportList()[0]->GetSize();
				Point.x = (Point.x - WindowTransform.x) / (WindowSize.x/ BufferX);
				Point.y = (Point.y - WindowTransform.y) / (WindowSize.y / BufferY);


				float LinearDepth;
				LinearDepth = VGraphics->GetD3DClass()->GetPixelDepth(Point.x, Point.y);
				LinearDepth += 256;
				
				//LinearDepth = (pow(1 * Screen_DEPTH + 1, LinearDepth) - 1) / 1;
				//LinearDepth = (LinearDepth - Screen_NEAR) / (Screen_DEPTH - Screen_NEAR);
				float ProjectionA = SCREEN_DEPTH / (SCREEN_DEPTH - SCREEN_NEAR);
				float ProjectionB = (-SCREEN_DEPTH * SCREEN_NEAR) / (SCREEN_DEPTH - SCREEN_NEAR);
				LinearDepth = ProjectionB / (LinearDepth - ProjectionA);
			
				char buffer[200];
				sprintf_s(buffer, size_t(100), "Depth  %f\n", LinearDepth);
				OutputDebugStringA(buffer);
					
				
				
			}
		}
		
			VResourceManager->GetWorld(0)->ClearSelectedActors();

}


VVector SystemClass::GetUnprojectedVector()
{


	VVector TracePoint;
	VVector Position;
	XMMATRIX Projection, ViewMatrix;
	XMVECTOR TraceVector, UnprojectedVector;
	XMMATRIX CameraPos;

	POINT Point;
	GetCursorPos(&Point);
	ScreenToClient(GetActiveWindow(), &Point);

	int WindowID = GetSelectedWindow();
	VVector2 ViewportSize = VWidgetManager->GetWidgetContainer(WindowID)->GetViewportList()[0]->GetSize();
	VVector2 ViewportTransform = VWidgetManager->GetWidgetContainer(WindowID)->GetViewportList()[0]->GetTransform();
	VVector2 WindowSize = VWidgetManager->GetWidgetContainer(WindowID)->GetWindowSize();

	float Aspect = VWidgetManager->GetWidgetContainer(WindowID)->GetViewportList()[0]->GetViewportAspect();


	TracePoint = VVector(Point.x, Point.y, SCREEN_DEPTH);
	Position = VVector(VGraphics->VCamera->GetPosition().x, VGraphics->VCamera->GetPosition().y, VGraphics->VCamera->GetPosition().z);
	TraceVector = XMLoadFloat3(&XMFLOAT3(TracePoint.x, TracePoint.y, TracePoint.z));
	VGraphics->GetD3DClass()->GetProjectionMatrix(Projection, Aspect);
	VGraphics->VCamera->GetViewMatrix(ViewMatrix);
	CameraPos = XMMatrixTranslation(Position.x, Position.y, Position.z);
	UnprojectedVector = XMVector3Unproject(TraceVector, ViewportTransform.x, ViewportTransform.y, ViewportSize.x, ViewportSize.y, SCREEN_NEAR, SCREEN_DEPTH, Projection, ViewMatrix, CameraPos);

	VVector UnprojectedV(UnprojectedVector.m128_f32[0], UnprojectedVector.m128_f32[1], UnprojectedVector.m128_f32[2]);


	return UnprojectedV;
}




void SystemClass::Exit()
{
	if (GetSelectedWindow() == 0)
		bIsBeingClosed = true;
	else
		CloseWindow();	
}

void SystemClass::BeginDragViewport()
{
	LPPOINT mousePoint;

	mousePoint = nullptr;

	mousePoint = VInput->GetMouseInput();
	RECT rect;
	GetWindowRect(GetActiveWindow(), &rect);
	WindowX = mousePoint->x - rect.left;
	WindowY = mousePoint->y - rect.top;
	isDraggingViewport = true;
	
	VWidgetManager->GetWidgetContainer(GetWindowID(GetActiveWindow()))->SetWindowSize(VVector2(ScreenResX, ScreenResY));
	

	MoveWindow(GetActiveWindow(),WindowX,WindowY,ScreenResX, ScreenResY, false);
	VWidgetManager->ResizeWindow(GetSelectedWindow());
	VGraphics->UpdateInterfaceBuffers(GetWindowID(GetActiveWindow()));

	return;

}

void SystemClass::TogglePhysics()
{

	bPhysicsEnabled = !bPhysicsEnabled;

}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		if (VWidgetManager->ActiveWidget != 0)
		{
			if (dynamic_cast<InputBox*>(VWidgetManager->ActiveWidget))
			{
				throw;

			}
		}
		// If a key is pressed send it to the input object so it can record that state.
		VInput->KeyDown((unsigned int)wparam);
		switch(wparam)
		{
			
		case 46:  //Delete
		{
			VWidgetManager->ActiveWidget = 0;
			VResourceManager->GetWorld(0)->DeleteSelectedActors();
			
			UpdateOutliner();
			VGraphics->UpdateInterfaceBuffers(GetSelectedWindow());
			
			return 0;
		}

		case 65:
			{
			if (isLeftCtrDown)
			{
				VResourceManager->GetWorld(0)->SelectAll();
				return 0;
			}

			isAKey = true;
		
			}
		case 68:
			{
				isDKey = true;

			}

		case 69: //E Key
			{
			VResourceManager->GetGizmo()->SetState(VGizmoRotation);
			break;
			//isWKey = true;

			}

		case 9: //Tab
			{
				ShowWindow(GetActiveWindow(), SW_MINIMIZE);		
			}

		case 87: //W Key
		{
			if (isRightDown)
			{
				isWKey = true;
				break;
			}
			VResourceManager->GetGizmo()->SetState(VGizmoTranslation);
			//isWKey = true;
			break;

		}
		case 82: //R Key
		{
			VResourceManager->GetGizmo()->SetState(VGizmoScale);
			//isWKey = true;
			break;

		}
		case 83:
		{
			isSKey = true;
			break;
		
		}
		case 17:
		{
			
			isLeftCtrDown = true;
		
			break;
		}
		case 113: //F2 
		{		//DrawWireframe
	
			VGraphics->ToggleWireframe();
			break;
		}
		case 114:
		{
			VGraphics->ToggleDrawInterface();

		}

		case 70: //F
			{
				VVector OriginLocation;
				OriginLocation = VVector(0,0,0);
				for each (Actor* ActiveActor in VResourceManager->GetWorld(0)->GetSelectedActors())
				{
					OriginLocation += ActiveActor->GetPosition();
				}
			
				OriginLocation = OriginLocation / VResourceManager->GetWorld(0)->GetSelectedActors().size();
				//To be fixed
		
				VVector DistanceBias =  VGraphics->VCamera->GetForwardVector() * -10;
				VGraphics->VCamera->SetPosition(OriginLocation + DistanceBias);
				break;
			}

		case 18:
			{
			isAltDown = true;
			break;
			}
		case 72:
		{

			
					for each (Actor* TargetActor in VResourceManager->GetWorld(0)->GetSelectedActors())
			{
						if (isAltDown == true)
							TargetActor->bIsVisible = true;
						else
							TargetActor->bIsVisible = false;
			}
					break;
			

		}

		case 13: //Enter
			{	
		
			//VWidgetManager->GetWidgetContainer((GetSelectedWindow()))->GetViewportList()[0]->ToggleDebugInfo();
			
		
			
			TogglePhysics();

			
			/*
			SpawnActor<PointLight>(VVector(0, 0, 0));
			UpdateOutliner();
			UpdateInterface();
			*/

			//ToggleVisibility
			/*
			Actor* SelectedActor;
			SelectedActor = VResourceManager->GetWorld(0)->GetSelectedActor(0);
			SelectedActor->bIsVisible = !(SelectedActor->bIsVisible);
			*/
			}
		}
		break;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		case 18:
		{
		isAltDown = false;
		}

		// If a key is released then send it to the input object so it can unset the state for that key.
		VInput->KeyUp((unsigned int)wparam);
		isLeftCtrDown = false;
		ReleaseInput();
		return 0;
	}
	
	case WM_MBUTTONUP:
	{
		isMiddleDown = false;
		//ShowCursor(true);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		ReleaseInput();
		POINT Point;
		GetCursorPos(&Point);
		ScreenToClient(hwnd,&Point);
		cursorPos = Point;
		isMiddleDown = true;
		//ShowCursor(false);
		//OutputDebugStringA("ShowCursor");
		return 0;
	
	}


	case WM_MOUSEWHEEL:
	{
		int mouseX, mouseY;
		LPPOINT mousePoint;

		mousePoint = 0;
		mousePoint = VInput->GetMouseInput();

		ScreenToClient(hwnd, mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;

		Widget* ClickedWidget;
		ClickedWidget = 0;
		ClickedWidget = (VWidgetManager->GetClickedWidget(mouseX, mouseY, GetSelectedWindow()));
		if (ClickedWidget->GetType() != ViewportWidget)
			{
				if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
					ClickedWidget->OnWheelUp();
				else
					ClickedWidget->OnWheelDown();
				return 0;
			}
		
		/////////////////////////////////////////
		ReleaseInput();
		int wheelInput = GET_WHEEL_DELTA_WPARAM(wparam)/10* mouseWheelRate;
		
		

			float speed = 0.4f*wheelInput;
			VVector MoveVector,Vector;
			Vector = VVector(0, 0, 0);
			MoveVector = VGraphics->VCamera->GetForwardVector();
			Vector.x += MoveVector.x*speed;
			Vector.y += MoveVector.y*speed;
			Vector.z += MoveVector.z*speed;

			VVector Pos = VGraphics->VCamera->GetPosition();

			VGraphics->VCamera->SetPosition(Pos.x + Vector.x, Pos.y + Vector.y, Pos.z + Vector.z);
			
		return 0;

	}
	
	case WM_RBUTTONDBLCLK:
	{

		OutputDebugStringA("DBlClick");
	}
	
	
	case WM_LBUTTONUP:
	{

		int mouseX, mouseY;
		LPPOINT mousePoint;
		mousePoint = 0;
		mousePoint = VInput->GetMouseInput();

		ScreenToClient(hwnd, mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;

		VWidgetManager->RemoveDropdownList();
		VWidgetManager->ActiveWidget = 0;
		

		if (isDragging)
		{
			ClearDrag();
			return 0;
		}
		
		if (isDraggingViewport)
		{
			isDraggingViewport = false;

		}
		Widget* ClickedWidget;
		ClickedWidget = 0;
		ClickedWidget = (VWidgetManager->GetClickedWidget(mouseX, mouseY, GetSelectedWindow()));
		if(ClickedWidget)
		ClickedWidget->OnMouseButtonUp();
	
		


		ReleaseInput();

		return 0;
	}
	case WM_LBUTTONDBLCLK:
	{	
		int mouseX, mouseY;
		LPPOINT mousePoint;
		mousePoint = 0;
		mousePoint = VInput->GetMouseInput();

		ScreenToClient(hwnd, mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;


		Widget* ClickedWidget;
		ClickedWidget = 0;
		ClickedWidget = (VWidgetManager->GetClickedWidget(mouseX, mouseY, GetSelectedWindow()));
		OutputDebugStringA("LeftDoubleClick");
		if (ClickedWidget != 0)
			ClickedWidget->OnLeftButtonDoubleClick();
		break;

	}

	case WM_LBUTTONDOWN:
	{
		

		POINT Point;
		GetCursorPos(&Point);
		ScreenToClient(hwnd, &Point);
		cursorPos = Point;

		ReleaseInput();
				
		HWND ActiveWindow = GetFocus();
		SetForegroundWindow(ActiveWindow);
		SetActiveWindow(ActiveWindow);
		ShowWindow(ActiveWindow,SW_SHOW);
		isRightDown = false;
		isLeftDown = true;

		int mouseX, mouseY;
		LPPOINT mousePoint;
		mousePoint = 0;
		mousePoint = VInput->GetMouseInput();

		ScreenToClient(hwnd, mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;

	

		Widget* ClickedWidget;
		ClickedWidget = 0;
		ClickedWidget =  (VWidgetManager->GetClickedWidget(mouseX, mouseY, GetSelectedWindow()));
	
		if (ClickedWidget)
		{
			
			if (ClickedWidget->GetType() != ViewportWidget)
			{
				if (ClickedWidget->GetIsDraggable())
				{	
					ClickedWidget->SetState(Dragged);
								isDragging = true;

				VWidgetManager->DragWidget = ClickedWidget;
				return 0;


				}
				else
				{
				
					
					ClickedWidget->OnMouseButtonDown();
					
					VWidgetManager->ActiveWidget = 0;
					ClickedWidget = 0;
				}
			}
			else
				MouseIntersection();
		}
		
		

	
	
		return 0;


	}
	case WM_RBUTTONUP:
	{
	
		

		isRightDown = false;
		//ShowCursor(true);
		POINT Cursor = GetCursorPosition();

		Widget* ClickedWidget;
		ClickedWidget = 0;
		ClickedWidget = VWidgetManager->GetClickedWidget(Cursor.x, Cursor.y, GetSelectedWindow());
		if (ClickedWidget)
		{
			if (ClickedWidget->GetType() == ViewportWidget)
				return 0;
			ClickedWidget->OnRightButtonUp();
			ReleaseInput();
		}

		VWidgetManager->ActiveWidget = 0;
		VWidgetManager->RemoveDropdownList();

		return 0;

	}

	case WM_RBUTTONDOWN:
	{

		ReleaseInput();
		POINT Point;
		GetCursorPos(&Point);
		cursorPos = Point;
		
		int mouseX, mouseY;
		LPPOINT mousePoint;
	
		isRightDown = true;
		mousePoint = nullptr;
		//ShowCursor(false);

		mousePoint = VInput->GetMouseInput();
		
		ScreenToClient(GetActiveWindow(), mousePoint);

		mouseX = mousePoint->x;
		mouseY = mousePoint->y;

		Widget* ClickedWidget;
		ClickedWidget = 0;
		ClickedWidget = VWidgetManager->GetClickedWidget(mouseX, mouseY, GetSelectedWindow());
		if (ClickedWidget)
		{ 
			if (ClickedWidget->GetType() == ViewportWidget)
				return 0;
			ClickedWidget->OnRightButtonDown();
			ReleaseInput();
		}
		return 0;
		
	}

	

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
	return 0;
}



WidgetManager* SystemClass::GetWidgetManager()
{

	return VWidgetManager;

}
void SystemClass::ClearDrag()
{
	
	VWidgetManager->DragWidget->SetState(InActive);
	VWidgetManager->DragWidget = 0;
	isDragging = false;
	
}

Graphics* SystemClass::GetRenderer()
{
	return VGraphics;

}


void SystemClass::ReleaseInput()
{
	isLeftDown = false;
	isMiddleDown = false;
	isRightDown = false;
	isAKey = false;
	isSKey = false;
	isDKey = false;
	isWKey = false;
	bIsGizmoActive  = false;
	
	if (VMaterialMaster->IsBinding)
	{
		VMaterialMaster->EndBinding();
	}

	if (isDragging == true)
	{
		isDragging = false;
		ClearDrag();
	}
}
void SystemClass::CreateViewport()
{
	int WindowWidth = GetSystemMetrics(SM_CXSCREEN);
	int WindowHeight = GetSystemMetrics(SM_CXSCREEN);
	//WindowHeight = 600;
	//WindowWidth = 800;



	int posX = (GetSystemMetrics(SM_CXSCREEN) - WindowWidth) / 2;
	int posY = (GetSystemMetrics(SM_CXSCREEN) - WindowHeight) / 2;

	HWND VGameViewport = CreateWindowEx(WS_EX_APPWINDOW, L"Vortex Engine", L"GameViewport",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, WindowWidth, WindowHeight, NULL, NULL, Vhinstance, NULL);

	// Bring the window up on the Screen and set it as main focus.
	ShowWindow(VGameViewport, SW_SHOW);
	SetForegroundWindow(VGameViewport);
	SetFocus(VGameViewport);

	VWindows.push_back(VGameViewport);
	VWidgetManager->CreateGameViewport();

	bool bCreated = VGraphics->GetD3DClass()->CreateSwapChain(VGameViewport);
	
	
		return;


}
void SystemClass::OpenFileBrowser()

{

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;


		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		pFileOpen->SetOkButtonLabel(L"Import");
		pFileOpen->SetTitle(L"Content Import");

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{

					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{


					

						size_t outputSize = 150;

						size_t buffersize = 150;

						wchar_t* source = pszFilePath;

						char *Path = new char[outputSize];
						size_t *outputLength = &outputSize;
						

						wcstombs_s(outputLength, Path, buffersize, source, outputSize);

						CoTaskMemFree(pszFilePath);
						
						ImportFileFromPath(Path);

					


					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
		
	}
	
}
void SystemClass::CreateProjectFiles()
{
	string FolderDirectory;
	char User[UNLEN + 1];
	LPSECURITY_ATTRIBUTES Attributes;
	Attributes = 0;
	DWORD NameLenght = UNLEN + 1;
	GetUserNameA(User, &NameLenght);
	FolderDirectory.append("C:\\Users\\"+  string(User) + "\\Documents\\Vortex Projects");
	CreateDirectoryA(FolderDirectory.c_str(), Attributes);
	ProjectDirectory = FolderDirectory.c_str();
}

bool SystemClass::CopyImportedFile(const char *SRC, const char* DEST)
{	
	return std::experimental::filesystem::copy_file(SRC, DEST, std::experimental::filesystem::copy_options::overwrite_existing);
}

void SystemClass::LoadFilesFromDirectory(const char* ProjectDirectory)
{

	for (auto Filepath : std::experimental::filesystem::directory_iterator(ProjectDirectory))  /*get directory */
	{
		bool Result = ImportFileFromPath(Filepath.path().string().c_str());
	
	}

}
inline bool SystemClass::FileExist(const char*  FilePath)
{
	FILE *file;
	errno_t err;

	if ((err = fopen_s(&file, FilePath, "r"))==0) {

		fclose(file);
		return true;
	}
	else
	{
		return false;
	}
	
}
bool SystemClass::ImportFileFromPath(const char*  FilePath)
{
	bool Result;

	string FileName = FilePath;
	string Extension = FileName.substr(FileName.find_last_of(".") + 1);
	size_t EndSize = Extension.size();

	

	if (FileName.find("\\") != 0)
	{
		FileName = FileName.substr(FileName.find_last_of("\\") + 1);
		FileName.resize(FileName.size() - EndSize - 1);

	}

	if (FileName.find("/") != 0)
	{
		FileName = FileName.substr(FileName.find_last_of("/") + 1);
	}
	
	if (!FileExist((ProjectDirectory + "\\" + FileName + "." + Extension).c_str()))
		Result = CopyImportedFile(FilePath, (ProjectDirectory + "\\" + FileName + "." + Extension).c_str());

	


	if (Extension == "fbx")
	{
	
		FBXImportManager FBXImportM;
		
		Model* VModel;

		VModel = FBXImportM.InitializeFBXManager(FilePath);
		
		if (VModel)
		{
			VModel->SetName(FileName);
			StaticMesh* VStaticMesh;
			VStaticMesh = new StaticMesh;
			VModel->CreateBoundingSphere();
			VGraphics->CreateChunkBuffers(VModel);
			
			VModel->UpdateIndexCount();
			VStaticMesh->SetModel(VModel);
			VResourceManager->AddStaticMesh(VStaticMesh);
			
			UpdateOutliner();
			UpdateInterface();
				
			
		}
		
		
	}

	else if (Extension == "png" || Extension == "tiff" || Extension == "jpg")
	{
		VResourceManager->CreateTexture();
		size_t newsize = strlen(FilePath) + 1;
		wchar_t * wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, FilePath, _TRUNCATE);


		if (FileName.find("_N") != string::npos || FileName.find("NRM") != string::npos || FileName.find("_normal") != string::npos)
		{
			if (VGraphics->NormalMap != nullptr)
				VGraphics->NormalMap->Release();
			VGraphics->LoadShaderResource(wcstring, &(VGraphics->NormalMap));
			//VResourceManager->AddTexture()
		}
		else  if (FileName.find("_OcclusionRoughnessMetallic") != string::npos)
		{
			if (VGraphics->RoughnessMetalness)
				VGraphics->RoughnessMetalness->Release();
			VGraphics->LoadShaderResource(wcstring, &(VGraphics->RoughnessMetalness));
		}
		else
		{
			if  (VGraphics->BaseColorMap)
			VGraphics->BaseColorMap->Release();
			VGraphics->LoadShaderResource(wcstring, &(VGraphics->BaseColorMap));
		}
		
		
		delete[] wcstring;
	}
	else return false;
	
	UpdateContentBrowser();

	VWidgetManager->UpdateWidgets(VResourceManager);
	return true;
}


void SystemClass::UpdateInterface()
{
	VWidgetManager->GetWidgetContainer(GetWindowID(GetActiveWindow()))->CalculateContainer();
	VGraphics->UpdateInterfaceBuffers(GetWindowID(GetActiveWindow()));
}
void SystemClass::CreateMaterial()
{
	VResourceManager->CreateMaterial();
	UpdateContentBrowser();
	return;

}
void SystemClass::UpdateContentBrowser()
{
	VWidgetManager->UpdateContentBrowser();

	return;
}
void SystemClass::CreateDirectionalLight()
{
	DirectionalLight* Light = SpawnActor<DirectionalLight>(VVector(0,0,0));
	VGraphics->SetDirectionalLight(Light);
	Light->SetName("DirectionalLight");
};


template<class C>
C* SystemClass::SpawnActor(VVector Location)
{

    int 	ActorIndex = 0;
    Actor*	SpawnedActor = new C();
	SpawnedActor->SetPosition(Location);
	string DefaultActorName = "Actor";
	string ActorName = DefaultActorName;

	

	
	if (static_cast<PointLight*>(SpawnedActor))
	{
		ActorName = "PointLight";
		DefaultActorName = ActorName;
		while (VResourceManager->GetWorld(0)->DoesActorNameExists(ActorName))
		{
			ActorIndex += 1;
			ActorName = DefaultActorName + to_string(ActorIndex);

		}
	}

		SpawnedActor->SetName(ActorName);

	VResourceManager->AddActor(SpawnedActor);

	UpdateOutliner();
	if(SpawnedActor)
	return static_cast<C*>(SpawnedActor);
	else return 0;

}

void SystemClass::GetResolution(int &horizontal,int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


void SystemClass::InitializeWindows(int& ScreenWidth, int& ScreenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	Vhinstance = GetModuleHandle(NULL);

	// Give the application a name.
	VapplicationName = L"Vortex Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC| CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = Vhinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = VapplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	// Register the window class.
	RegisterClassEx(&wc);

	

	// Determine the resolution of the clients desktop Screen.
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CXSCREEN);
	
	// Setup the Screen settings depending on whether it is running in full Screen or in windowed mode.
	if (FULL_SCREEN)
	{
		//GetWindowsTaskBar().top
		// If full Screen set the Screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)ScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)ScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full Screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		//ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		//ScreenHeight = GetSystemMetrics(SM_CXSCREEN)-(GetWindowsTaskBar().bottom - GetWindowsTaskBar().top);
		
		// If windowed then set it to 800x600 resolution.
		ScreenWidth = 800;
		ScreenHeight = 600;

		// Place the window in the middle of the Screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) / 2;
		posY = (GetSystemMetrics(SM_CXSCREEN) - ScreenHeight) / 2;
	}

	// Create the window with the Screen settings and get the handle to it.
	Vhwnd = CreateWindowEx(WS_EX_APPWINDOW, VapplicationName, VapplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, ScreenWidth, ScreenHeight, NULL, NULL, Vhinstance, NULL);
	
	// Bring the window up on the Screen and set it as main focus.
	ShowWindow(Vhwnd, SW_SHOW);
	SetForegroundWindow(Vhwnd);
	SetFocus(Vhwnd);

	VWindows.push_back(Vhwnd);

	WindowX = posX;
	WindowY = posY;

	
	
	ShowCursor(bShowCursor);
	
	
	return;
}

int	SystemClass::GetFPS()
{

	return VDebugger->GetFPS();
}
/*
void SystemClass::DestroyWindow(HWND window)
{
	

}
*/
void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full Screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(Vhwnd);
	Vhwnd = NULL;

	// Remove the application instance.
	UnregisterClass(VapplicationName, Vhinstance);
	Vhinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}
void SystemClass:: Quit()
{
	Shutdown();

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}
