#pragma once

#include <vector>
#include <memory>
#include "Widget.h"
#include "Button.h"
#include "WorldOutliner.h"
#include "Text.h"
#include "Object.h"
#include "World.h"
#include "Math.h"
#include "Viewport.h"
#include "DetailPanel.h"

using namespace std;

class ResourceManager;


enum WindowType
{
	Editor,
	MaterialEditor,
	StaticMeshEditor,

};

struct LinePoint
{
	VVector position;
	VColor color;
};


struct WidgetVertex
{
	VVector position;
	VVector2 uv;
	VColor color;
};

struct Line
{
	VVector Points[2];
	VColor Color;
};

struct WidgetContainer
{
	

	bool		 AddWidget(Widget* newWidget, string parentName);
	bool		 ClearContainer();
	int			 GetLineCount();
	string		 GetName();
	VVector2	 GetWindowSize();
	VVector2	 GetWindowTransform();
	void		 SetName(string SourceName);
	void		 SetWindowID(int ID);
	void		 SetIsBeingDestroyed(bool Destroyed);

	vector<Line*>		LineList;
	vector<Text*>		TextList;
	vector<Viewport*>	ViewportList;
	vector <DetailPanel*> DetailPanelList;

	Widget*			  GetWidgetRoot();
	vector<Viewport*> GetViewportList();
	vector<Widget*>   GetWidgetList();
	vector<Line*>     GetLineList();
	vector<Text*>	  GetTextList();
	//vector<DetailPanel*> GetDetailPanelList();
	
	bool GetIsBeingDestroyed();
	void GetLetterCount();
	void CalculateWidgetArray(Widget* InWidget);
	void CalculateContainer();
	void AddLine(Line* NewLine);
	void SetWindowSize(VVector2 Size);
	void UpdateDetailPanel();
	void  SetWindowTransform(VVector2 Transform);
	
	int				 LetterCount;
	Widget*			 WidgetRootNode;
	vector<Widget*>  WidgetList;
private:


	int				 WindowID;
	bool			 bIsBeingDestroyed;
	string		     Name;
	VVector2		 WindowTransform;
	VVector2		 WindowSize;
};

class WidgetManager :public Object
{
public:
	WidgetManager();
	~WidgetManager();

	WidgetContainer* CreateWidgetContainer(string Name);
	WidgetContainer* GetWidgetContainer(int i);
	WidgetContainer* GetWidgetContainer(string name);
	
	int		 GetWindowCount();
	void     CreateGameViewport();
	void	 CreateOutliner();
	void	 CreateContentBrowser();
	void	 CreateMaterialEditor(int WindowID);

	
	void     UpdateContentBrowser(string AssetName);
	void     UpdateWidgets(ResourceManager* VResourceManager);
	void	 UpdateWorldOutliner(World* VWorld);
	void	 ResizeWindow(int ID);
	void	 Shutdown();
	void	 Initialize();
	void     AddViewport(int ContainerID);

	
	int		 GetWidgetCount	();
	void	 CloseContentBrowser();
	void	 RemoveWidget(string);
	void	 ClearContainersArray();
	static WidgetManager* GetWidgetManager();

	void	 CreateInterface();

	Widget* GetParentWidgetFromHierarchy(Widget* ChildWidget,string name);
	Widget* CheckIntersection(Widget* InWidget, int mouseX, int mouseY);
	Widget* GetWidgetByName(string Name);
	Widget* GetClickedWidget(float inMouseX, float inMouseY,int WindowID);
	Widget* DragWidget;
	Widget* ActiveWidget;

	vector<WidgetContainer*>WidgetContainers;
private:
	

	
};

static WidgetManager* VWidgetManager = 0;
