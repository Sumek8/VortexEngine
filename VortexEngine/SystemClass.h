#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN
#define DIRECTX = true;

#include <windows.h>
#include <vector>
#include <string>
#include "Math.h"
#include "Input.h"
#include "resource.h"
#include "Graphics.h"
#include "WidgetManager.h"
#include "SystemDebugger.h"
#include "ResourceManager.h"
#include "World.h"
#include "MaterialMaster.h"
#include "Delegate.h"



class SystemClass
{
public:
	SystemClass();
	~SystemClass();

	bool		 Initialize();
	void		 Run();
	void		 Shutdown();
	void		 BeginDragViewport();
	void		 TogglePhysics();
	void	     GetResolution(int &horizontal, int &vertical);
	void		 ReleaseInput();
	

	Graphics*	 GetRenderer();
	LRESULT		 CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void		 GenerateTerrain();
	void		 ResizeViewport();
	void	     Play();
	void		 CloseWindow();
	void		 OpenFileBrowser();
	void		 HideActor(int ID);
	void		 SelectActor(int ID);
	void		 CreateGizmo();
	void		 CreateGrid();
	void		 Exit();
	static	     SystemClass* GetSystem();
	int			 GetSelectedWindow();
	int			 GetFPS();
	void		 UpdateInterface();
	VVector		 GetUnprojectedVector();
	void		 AddObjectToWorld();

private:
	RECT		GetWindowsTaskBar();
	bool		Frame();
	void		UpdatePhysics();
	void		InitializeWindows(int&, int&);
	void		ShutdownWindows();
	void		CreateWidgetManager();
	bool		ImportFileFromPath(const char* FilePath);
	void		MouseIntersection();
	void		UpdateContentBrowser(string AssetName);
	
	template<class C>
	void SpawnActor(VVector Location);
	
	void		ClearDrag();
	void		Quit();

	
	void		OpenMaterialEditor();
	void		CreateViewport();
	void		UpdateOutliner();
	void		DragViewport();
	
	
	int			GetWindowID(HWND Window);
	void		CreateProjectFiles();

	bool		CopyImportedFile(const char *SRC, const char* DEST);
	bool		FileExist(const char * FilePath);

	void		LoadFilesFromDirectory(const char*  DirectoryPath);



	
	

	
	LPCWSTR			 VapplicationName;
	HINSTANCE		 Vhinstance;
	HWND			 Vhwnd;
	vector<HWND>	 VWindows;
	ResourceManager* VResourceManager;
	Graphics*		 VGraphics;
	Input*			 VInput;
	SystemDebugger*  VDebugger;
	WidgetManager*	 VWidgetManager;
	
	MaterialMaster*  VMaterialMaster;
	POINT		     cursorPos;
	string           ProjectDirectory;


	bool			 bIsBeingClosed;
	bool		     bShowCursor;
	float			 mouseWheelRate;
	bool			 isAltDown;
	bool			 isMiddleDown;
	bool			 isRightDown;
	bool			 isDragging;
	bool			 isLeftDown;
	bool			 isLeftCtrDown;
	bool			 isDraggingViewport;
	bool			 isAKey, isDKey, isWKey, isSKey;
	int				 ScreenResX, ScreenResY;
	bool			 bIsGizmoActive;
	int			   	 WindowX, WindowY;
	bool			 bIsFullScreen;
	bool			 bPhysicsEnabled;
	
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;
#endif