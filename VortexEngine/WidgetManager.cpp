#pragma once

#include "stdafx.h"
#include "Widget.h"
#include "WidgetManager.h"
#include "WorldOutliner.h"
#include "ResourceManager.h"
#include "ContentBrowser.h"
#include "Viewport.h"
#include "SystemClass.h"
#include "Composer.h"
#include "DetailPanel.h"
#include "DropDownList.h"



WidgetManager::WidgetManager()
{

	VWidgetManager = this;
	
}


WidgetManager::~WidgetManager()
{
}

WidgetManager* WidgetManager::GetWidgetManager()
{

	return VWidgetManager;

}

void WidgetManager::Initialize()
{
	CreateWidgetContainer("CoreWindow");
	
	CreateInterface();

}



string  WidgetContainer::GetName()
{

	return Name;

}

vector<Text*> WidgetContainer::GetTextList()
{
	if (TextList.size()>0)
	return TextList;
	else return vector<Text*>();
}

void WidgetContainer::SetName(string SourceName)
{
	Name = SourceName;

}
void WidgetContainer::SetWindowID(int ID)
{
	WindowID = ID;
	return;

}
void WidgetManager::RemoveDropdownList()
{
	

	if (GetWidgetContainer(0)->GetWidgetRoot()->GetChildCount() > 1)
	{
		for (int i = 1;i<GetWidgetContainer(0)->GetWidgetRoot()->GetChildCount();i++)
			GetWidgetContainer(0)->GetWidgetRoot()->RemoveChild(i);
	}
			SystemClass::GetSystem()->UpdateInterface();
			
		
	
	
}





void WidgetContainer::SetIsBeingDestroyed(bool Destroyed)
{
	bIsBeingDestroyed = Destroyed;
}

void WidgetContainer::AddRootNode()
{
	WidgetRootNode = CreateWidget<Widget>();
}

WidgetContainer* WidgetManager::CreateWidgetContainer(string SourceName)
{
	WidgetContainer* Container;
	Container = new WidgetContainer;
	Container->SetIsBeingDestroyed(false);
	Container->SetName(SourceName);
	Container->SetWindowSize(VVector2(800,600));
	Container->SetWindowTransform(VVector2(0, 0));
	Container->SetWindowID(int(WidgetContainers.size()));
	WidgetContainers.push_back(Container);

	
	//Container->AddRootNode();

	return Container;
}



Widget* WidgetManager::GetWidgetByName(string Name)
{
	for (size_t i = 0; i < WidgetContainers.size(); i++)
	{

		for (size_t j = 0; j < WidgetContainers[i]->GetWidgetList().size(); j++)
		{
			if (WidgetContainers[i]->GetWidgetList()[j]->GetName() == Name)
				return WidgetContainers[i]->GetWidgetList()[j];
		}
	}
	return 0;
}

WidgetContainer* WidgetManager::GetWidgetContainer(string name)
{
	for (size_t i = 0; i <WidgetContainers.size(); i++)
	{
		if (WidgetContainers[i]->GetName() == name)
			return WidgetContainers[i];
	}
	return WidgetContainers[0];
}

WidgetContainer* WidgetManager::GetWidgetContainer(int i)
{

	return WidgetContainers[i];

}
void WidgetContainer::CalculateContainer()
{
	WidgetList.clear();
	TextList.clear();
	ViewportList.clear();
	
	
	CalculateWidgetArray(WidgetRootNode);
	
	GetLetterCount();
	

}
void WidgetContainer::GetLetterCount()
{
	LetterCount = 0;
	for (size_t i = 0; i < TextList.size(); i++)
	{
		LetterCount += int(TextList[i]->GetText().size());
	}
	return;
}


void WidgetContainer::CalculateWidgetArray(Widget* InWidget)
{
	if (InWidget->GetIsVisible())
	{
		if (InWidget->GetType() == ViewportWidget)
			ViewportList.push_back(static_cast<Viewport*>(InWidget));
		else if (InWidget->GetType() == TextWidget)
			TextList.push_back(static_cast<Text*>(InWidget));
		else
			WidgetList.push_back(InWidget);
		}

	for (int i = 0; i < InWidget->GetChildCount(); i++)
	{
		
			//if (InWidget->GetChildWidget(i)->GetInheritSize())
			//	InWidget->GetChildWidget(i)->SetSize(InWidget->GetSize().x, InWidget->GetSize().y);
			CalculateWidgetArray(InWidget->GetChildWidget(i));
			
	}

	return;
}

void WidgetManager::ClearContainersArray()
{

	for (size_t i = 0; i <WidgetContainers.size(); i++)
	{
		WidgetContainers[i]->ClearContainer();
		delete WidgetContainers[i];

	}

	WidgetContainers.clear();
}


void WidgetContainer::AddLine(Line* NewLine)
{
	
	LineList.push_back(NewLine);
	return;
}

bool WidgetContainer::AddWidget(Widget* newWidget, string parentName = "")
{


	if (!newWidget)
		return false;

	bool hasParent = false;


	//CreateRootNode//
	if (WidgetList.size() == 0)
	{
		WidgetRootNode = newWidget;
		newWidget->UpdateTransform();		
		newWidget->SetName("RootNode");
		return true;
	}
	if (parentName != "")
	{
		for (size_t i = 0; i < WidgetList.size(); i++)
		{
			if (WidgetList.at(i)->GetName() == parentName)
			{
				hasParent = true;
				Widget* Parent = WidgetList.at(i);
				newWidget->SetParentTransform(Parent->GetTransform().x, Parent->GetTransform().y);
				newWidget->UpdateTransform();
				Parent->AddChildWidget(newWidget);

				int ZOrder = 0;
				ZOrder = Parent->GetZOrder() + 1;
				newWidget->SetZOrder(ZOrder);


				break;
			}


		}
	}
	if (!hasParent)
	{
		
		WidgetRootNode->AddChildWidget(newWidget);
		newWidget->UpdateTransform();
	}
	

	CalculateContainer();

	return true;


}

void  WidgetManager::CreateGameViewport()
{
	WidgetContainer* GameViewport = CreateWidgetContainer("Game Viewport");

	VVector2 WindowSize = GameViewport->GetWindowSize();

	Canvas* Canva = CreateWidget<Canvas>();
	GameViewport->AddWidget(Canva);

	Composer* VComposer = CreateWidget<Composer>();
	VComposer->SetEnabled(false);
	VComposer->SetIsHorizontal(false);
	VComposer->SetSize(WindowSize.x, WindowSize.y);
	Canva->AddChildWidget(VComposer);


	Widget* InfoPanelWidget = CreateWidget<Widget>();
	InfoPanelWidget->SetName(string("InfoPanel"));
	InfoPanelWidget->SetColor(0.3f, 0.3f, 0.3f, 1);
	InfoPanelWidget->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::BeginDragViewport>(SystemClass::GetSystem());
	InfoPanelWidget->OnLeftMouseDoubleClickDelegate.Bind<SystemClass, &SystemClass::ResizeViewport>(SystemClass::GetSystem());
	VComposer->AddChildWidget(InfoPanelWidget);

	Button* ExitButton = CreateWidget<Button>();
	ExitButton->SetColor(0.8f, 0.8f, 0.8f, 1);
	ExitButton->SetRelativeTransform(float(WindowSize.x - 25), 2.0f);
	ExitButton->SetSize(25, 25);
	ExitButton->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::CloseWindow>(SystemClass::GetSystem());
	ExitButton->SetName(string("Exit"));

	InfoPanelWidget->AddChildWidget(ExitButton);


	Viewport* VViewport = CreateWidget<Viewport>();
	VViewport->SetName("GameViewport");
	VComposer->AddChildWidget(VViewport);


	///Container
	GameViewport->CalculateContainer();
	int i = 0;

}

bool WidgetContainer::GetIsBeingDestroyed()
{
	return bIsBeingDestroyed;

}

int WidgetManager::GetWindowCount()
{
	
	return int(WidgetContainers.size());

}


bool WidgetContainer::ClearContainer()
{



	if (WidgetRootNode)
	{
		WidgetRootNode->Shutdown();
	delete WidgetRootNode;
	}

	
	for (size_t i = 0; i < LineList.size(); i++)
	{
		delete LineList[i];
	}
	ViewportList.clear();
	LineList.clear();
	WidgetList.clear();
	TextList.clear();
	return true;
}
 int WidgetContainer::GetLineCount()
{

	return int(LineList.size());

}

 void WidgetManager::CloseContentBrowser()
 {
	
		 RemoveWidget("Content Browser");

 }


 void WidgetManager::UpdateWorldOutliner(World* VWorld)
 {
	 WorldOutliner* VWorldOutliner = 0;
	 VWorldOutliner = static_cast<WorldOutliner*>(GetWidgetByName("World Outliner"));
	 if (!VWorldOutliner)
		 return;
	 VWorldOutliner->ClearList();
	
	
	 if (VWorld->VActors.size() > 25)
	 {
		 for (int i = 0; i < 25; i++)
			 VWorldOutliner->AddActor(VWorld->VActors[i]);

	 }
	 else
		 for (int i = 0; i <VWorld->VActors.size(); i++)
			 VWorldOutliner->AddActor(VWorld->VActors[i]);

	 WidgetContainers[0]->CalculateContainer();
	 
 }



 void WidgetManager::RemoveWidget(string WidgetName)
 {
	 ActiveWidget = 0;
 for (size_t j = 0; j < WidgetContainers.size(); j++)
 {
	 Widget* ParentWidget;

	 ParentWidget = 0;			 

			 ParentWidget = GetParentWidgetFromHierarchy(WidgetContainers[j]->GetWidgetRoot(),WidgetName);
			 if (ParentWidget != 0) 
				 break;
		 
		 if (ParentWidget != 0)
		 {
		
			 for (int i = 0; i < ParentWidget->GetChildCount(); i++)
			 {

				 if (ParentWidget->GetChildWidget(i)->GetName() == WidgetName)
				 {
								
					 ParentWidget->GetChildWidget(i)->Shutdown();
					 delete ParentWidget->GetChildWidget(i);		
					 ParentWidget->ChildWidgets.erase(ParentWidget->ChildWidgets.begin() + i);
				 }
				
			 }
			
			
			 WidgetContainers[j]->CalculateContainer();
		 }
	 }
 }
 
 void WidgetManager::AddViewport(int ContainerID)
 {
	 Viewport* VViewport;
	 VViewport = new  Viewport;
	 VViewport->SetSize(500,475);
	 VViewport->SetRelativeTransform(100, 25);
	 VViewport->SetColor(1, 1, 1, 1);
	
	
	 WidgetContainers[ContainerID]->GetWidgetRoot()->GetChildWidget(1)->AddChildWidget(VViewport);

	 return;


 }
 


 Widget* WidgetManager::GetParentWidgetFromHierarchy(Widget* CheckWidget,string ChildWidgetName)
 {

	 Widget* ParentWidget;
	 ParentWidget = 0;

	 if (CheckWidget->GetChildCount() > 0)
	 {
		 for (int i = 0; i <CheckWidget->GetChildCount(); i++)
		 {
			 if (CheckWidget->GetChildren()[i]->GetName() == ChildWidgetName)
				 return CheckWidget;

			 ParentWidget = GetParentWidgetFromHierarchy(CheckWidget->GetChildren()[i],ChildWidgetName);
			 return ParentWidget;
		 }
		 return 0;
	 }


	 else return 0;
 }

 void WidgetManager::ResizeWindow(int ID)
 {
	 GetWidgetContainer(ID)->GetWidgetRoot()->SetSize(GetWidgetContainer(ID)->GetWindowSize().x, GetWidgetContainer(ID)->GetWindowSize().y);
	 GetWidgetContainer(ID)->GetWidgetRoot()->UpdateChildTransform();
 
 }

 void WidgetManager::UpdateContentBrowser()
 {
	 ContentBrowser* Browser = 0;
	 Browser = static_cast<ContentBrowser*>(GetWidgetByName("Content Browser"));

	 Browser->Update();
	 WidgetContainers[0]->CalculateContainer();
 }

 void WidgetManager::CreateOutliner()
 {
	 WorldOutliner* VWorldOutliner;
	 VWorldOutliner = new WorldOutliner;
	 WidgetContainers[0]->AddWidget(VWorldOutliner);
	 return;
 }

 void WidgetManager::CreateTabDropdownList()
 {
	 POINT Cursor = SystemClass::GetCursorPosition();


	 DropDownList* WidgetList = CreateWidget<DropDownList>();
	 GetWidgetContainer(0)->AddWidget(WidgetList);
	 WidgetList->SetTransform(Cursor.x, Cursor.y - WidgetList->GetSize().y);
	 WidgetList->AddElement("ContentBrowser")->OnRightMouseButtonUpDelegate.Bind<WidgetManager,&WidgetManager::CreateContentBrowser>(this);
	 WidgetList->AddElement("DetailPanel");
	 WidgetList->AddElement("MaterialPanel");
	 WidgetList->AddElement("Viewport");


	 GetWidgetContainer(0)->CalculateContainer();


 }
 void WidgetManager::CreateFileDropdownList()
 {

	 POINT Cursor = SystemClass::GetCursorPosition();

	
	 DropDownList* WidgetList = CreateWidget<DropDownList>();
	 GetWidgetContainer(0)->AddWidget(WidgetList);
	 WidgetList->SetTransform(Cursor.x, Cursor.y - WidgetList->GetSize().y);
	 WidgetList->AddElement("NewProject");
	 WidgetList->AddElement("SaveProject");
	 WidgetList->AddElement("OpenFile");
	 WidgetList->AddElement("Preferences");

	 
	 GetWidgetContainer(0)->CalculateContainer();

 }

 void WidgetManager::CreateBrowserDropdownList()
 {
	 POINT Cursor = SystemClass::GetCursorPosition();
	 
	 DropDownList* WidgetList = CreateWidget<DropDownList>();
	 GetWidgetContainer(0)->AddWidget(WidgetList);
	
	 WidgetList->SetTransform(Cursor.x, Cursor.y - WidgetList->GetSize().y);
	 WidgetList->AddElement("Import File")->OnRightMouseButtonUpDelegate.Bind<SystemClass,&SystemClass::OpenFileBrowser>(SystemClass::GetSystem());
	 WidgetList->AddElement("Create Material")->OnRightMouseButtonUpDelegate.Bind<SystemClass, &SystemClass::CreateMaterial>(SystemClass::GetSystem());
	 WidgetList->AddElement("Create Particle System");
	 WidgetList->AddElement("Properties");

	
	 
	 GetWidgetContainer(0)->CalculateContainer();
	 
 }

 void WidgetManager::CreateContentBrowser()
 {
	 ContentBrowser* VContentBrowser;
	 VContentBrowser = CreateWidget<ContentBrowser>();
 
	
	
	 VContentBrowser->OnRightMouseButtonDownDelegate.Bind<WidgetManager, &WidgetManager::CreateBrowserDropdownList>(this);

	
	static_cast<Panel*>(ActiveWidget)->AddChildWidget(VContentBrowser);


	 GetWidgetContainer(0)->CalculateContainer();
	 SystemClass::GetSystem()->UpdateInterface();

	 return;
 }


 vector<Line*> WidgetContainer::GetLineList()

 {
	 return LineList;

 }

vector<Widget*> WidgetContainer::GetWidgetList()
{
	return WidgetList;

}

vector<Viewport*> WidgetContainer::GetViewportList()
{
	return ViewportList;

}



Widget* WidgetContainer::GetWidgetRoot()
{
	return WidgetRootNode;

}


 VVector2 WidgetContainer::GetWindowSize()
 {
	return  WindowSize;

 }

 void WidgetContainer::SetWindowSize(VVector2 Size)
 {
	 WindowSize = Size;

 }


 void WidgetContainer::SetWindowTransform(VVector2 SourceTransform)
 {
	 WindowTransform = SourceTransform;
 
 }

 VVector2 WidgetContainer::GetWindowTransform()
 {
	return WindowTransform;

 }



 void WidgetManager::CreateInterface()
 {
	
	 VVector2 WindowTransform = GetWidgetContainer(0)->GetWindowTransform();
	 VVector2 WindowSize = GetWidgetContainer(0)->GetWindowSize();


	 Canvas* BaseCanvas = CreateWidget<Canvas>();
	 GetWidgetContainer(0)->AddWidget(BaseCanvas);
	


	 Composer* VComposer;
	 VComposer = new Composer;
	 VComposer->SetIsHorizontal(false);
	 VComposer->SetSize(WindowSize.x, WindowSize.y);
	 BaseCanvas->AddChildWidget(VComposer);

		 Widget* InfoPanelWidget;
		 InfoPanelWidget = new Widget;
		 InfoPanelWidget->SetName(string("InfoPanel"));
		 InfoPanelWidget->SetColor(0.3f, 0.3f, 0.3f, 1);
		 InfoPanelWidget->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::BeginDragViewport>(SystemClass::GetSystem());
		 InfoPanelWidget->OnLeftMouseDoubleClickDelegate.Bind<SystemClass, &SystemClass::ResizeViewport>(SystemClass::GetSystem());
		 VComposer->AddChildWidget(InfoPanelWidget);

		 Button* ExitButton;
		 ExitButton = new Button;
		 ExitButton->SetColor(0.5f, 0.5f, 0.5f, 1);
		 ExitButton->SetRelativeTransform(float(WindowSize.x - 25), 0.0f);
		 ExitButton->SetSize(25, 25);		 
		 ExitButton->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::Exit>(SystemClass::GetSystem());
		 ExitButton->SetName(string("Exit"));

		 InfoPanelWidget->AddChildWidget(ExitButton);

		 VVector2 Size = InfoPanelWidget->GetSize();


		 Button* PlayButton;
		 PlayButton = new Button;
		 PlayButton->SetColor(0.5f, 0.5f, 0.5f, 1);
		 PlayButton->SetRelativeTransform(float(WindowSize.x/2), 0.0f);
		 PlayButton->SetSize(25, 25);

		 PlayButton->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::Play>(SystemClass::GetSystem());
		 PlayButton->SetName(string("Play"));
		 InfoPanelWidget->AddChildWidget(PlayButton);


		 Button* PanelButton;
		 PanelButton = new Button;
		 PanelButton->SetColor(0.5f, 0.5f, 0.5f, 1);
		 PanelButton->SetRelativeTransform(2.0f, 0);
		 PanelButton->SetSize(40, 25);
		 PanelButton->SetName(string("File"));
		 InfoPanelWidget->AddChildWidget(PanelButton);
		 PanelButton->OnLeftMouseButtonDownDelegate.Bind<WidgetManager, &WidgetManager::CreateFileDropdownList>(this);
		 

		 Text* PanelText;
		 PanelText = new Text;
		 PanelText->SetColor(1, 1, 1, 1);
		 PanelText->SetRelativeTransform(5.0f, 3.0f);
		 PanelText->SetSize(20, 15);
		 PanelText->SetName(string("File"));
		 PanelText->SetText("File");
		 PanelButton->AddChildWidget(PanelText);





	
	 Composer* VCenterComposer = new Composer;
	 VCenterComposer->SetIsHorizontal(true);
	 VComposer->AddChildWidget(VCenterComposer);

	
	 Panel*  VPanel = new Panel;
	 VPanel->SetName("Panel1");
	 VComposer->AddChildWidget(VPanel);
	// VPanel->OnRightMouseButtonDownDelegate.Bind<WidgetManager, &WidgetManager::CreateTabDropdownList>(this);
	 VPanel->OnRightMouseButtonDownDelegate.Bind<WidgetManager, &WidgetManager::CreateContentBrowser>(this);

		 ContentBrowser* VContentBrowser;
		 VContentBrowser = new  ContentBrowser;
		 VPanel->AddChildWidget(VContentBrowser);
		

	 Panel*  VLeftPanel = new Panel;
	 VCenterComposer->AddChildWidget(VLeftPanel);
	
		DetailPanel* VLeftDetailPanel;
		VLeftDetailPanel = new DetailPanel;
		VLeftDetailPanel->SetColor(0.3f, 0.3f, 0.3f, 1);
		VLeftPanel->AddChildWidget(VLeftDetailPanel);
		
	  	

		 Viewport* VViewport;
		 VViewport = new  Viewport;
		 VCenterComposer->AddChildWidget(VViewport);
		 VViewport->SetColor(1, 1, 1, 1);
		 //VisualDebugger


		 Composer* VRightComposer;
		 VRightComposer = new Composer;
		 VRightComposer->SetIsHorizontal(false);
		 VRightComposer->DistributionArray = { 0.6f,0.4f };
		 VCenterComposer->AddChildWidget(VRightComposer);


		 Panel*  VTabPanel = new Panel;
		 VRightComposer->AddChildWidget(VTabPanel);

			
				 WorldOutliner*  VWorldOutliner = new WorldOutliner;
				 VTabPanel->AddChildWidget(VWorldOutliner);
		 

		 Panel*  VLowerTabPanel = new Panel;
		 VRightComposer->AddChildWidget(VLowerTabPanel);

			DetailPanel* VDetailPanel = new DetailPanel;
			VDetailPanel->SetColor(0.3f, 0.3f, 0.3f, 1);
			VLowerTabPanel->AddChildWidget(VDetailPanel);
			 
			

		
	GetWidgetContainer(0)->CalculateContainer();
	


	
	
 }

 void WidgetManager::CreateMaterialEditor(int WindowID)
 {
	 VVector2 WindowTransform = GetWidgetContainer(WindowID)->GetWindowTransform();
	 VVector2 WindowSize = GetWidgetContainer(WindowID)->GetWindowSize();

	 ///Container
	 Composer* VComposer;
	 VComposer = new Composer;
	 VComposer->SetIsHorizontal(false);
	 VComposer->SetSize(WindowSize.x, WindowSize.y);
	 VComposer->DistributionArray = { 0.05f,0.7f};
	 GetWidgetContainer(WindowID)->AddWidget(VComposer);

	

	    Widget* InfoPanelWidget;
	    InfoPanelWidget = new Widget;
	    InfoPanelWidget->SetName(string("InfoPanel"));
	    InfoPanelWidget->SetColor(0.3f, 0.3f, 0.3f, 1);
	    InfoPanelWidget->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::BeginDragViewport>(SystemClass::GetSystem());
	    InfoPanelWidget->OnLeftMouseDoubleClickDelegate.Bind<SystemClass, &SystemClass::ResizeViewport>(SystemClass::GetSystem());
	    VComposer->AddChildWidget(InfoPanelWidget);

	    Composer* VCenterComposer;
	    VCenterComposer = new Composer;
		VCenterComposer->SetIsHorizontal(true);
	    VCenterComposer->DistributionArray = { 0.7f,0.3f};
	    VComposer->AddChildWidget(VCenterComposer);


			Composer* VCenterLeftComposer;
			VCenterLeftComposer = new Composer;
			VCenterLeftComposer->SetIsHorizontal(false);
			VCenterLeftComposer->DistributionArray = { 0.8f,0.2f };
			VCenterComposer->AddChildWidget(VCenterLeftComposer);

			Panel*  VNodePanel = new Panel;
			VCenterLeftComposer->AddChildWidget(VNodePanel);

				Widget* NodeGraph;
				NodeGraph = new Widget;
				NodeGraph->SetName("MaterialGraph");	
				NodeGraph->SetColor(0.1f, 0.1f, 0.1f, 1);
				VNodePanel->AddChildWidget(NodeGraph);

				Widget* NodeBrowser;
				NodeBrowser = new Widget;
				NodeBrowser->SetColor(0.3f, 0.3f, 0.3f, 1);
				VCenterLeftComposer->AddChildWidget(NodeBrowser);
				
			Composer* VCenterRightComposer;
			VCenterRightComposer = new Composer;
			VCenterRightComposer->SetIsHorizontal(false);
			VCenterRightComposer->DistributionArray = { 0.5f,0.5f };
			VCenterComposer->AddChildWidget(VCenterRightComposer);


				Viewport* VViewport;
				VViewport = new  Viewport;
				VCenterRightComposer->AddChildWidget(VViewport);

				Widget* MaterialDetails;
				MaterialDetails = new Widget;
				MaterialDetails->SetColor(0.5f, 0.5f, 0.5f, 1);
				VCenterRightComposer->AddChildWidget(MaterialDetails);

	

	 	 Button* ExitButton;
		 ExitButton = new Button;
		 ExitButton->SetColor(0.5f, 0.5f, 0.5f, 1);
		 ExitButton->SetRelativeTransform(float(WindowSize.x - 25), 0.0f);
		 ExitButton->SetSize(25, 25);
		 ExitButton->OnLeftMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::CloseWindow>(SystemClass::GetSystem());
		 ExitButton->SetName(string("Exit"));

		InfoPanelWidget->AddChildWidget(ExitButton);

		

		 Button* PanelButton;
		 PanelButton = new Button;
		 PanelButton->SetColor(0.5f, 0.5f, 0.5f, 1);
		 PanelButton->SetRelativeTransform(2.0f, 0);
		 PanelButton->SetSize(40, 25);
		 PanelButton->SetName(string("File"));
		 InfoPanelWidget->AddChildWidget(PanelButton);

			 Text* PanelText;
			PanelText = new Text;
			PanelText->SetColor(1, 1, 1, 1);
			PanelText->SetRelativeTransform(5.0f, 3.0f);
			PanelText->SetSize(20, 15);
			 PanelText->SetName(string("File"));
			PanelText->SetText("File");
	 		PanelButton->AddChildWidget(PanelText);

	 GetWidgetContainer(WindowID)->CalculateContainer();
	
 }



Widget* WidgetManager::CheckIntersection(Widget* InWidget,int mouseX,int mouseY)
{
	Widget* ClickWidget = 0;
	VVector2 Transforms = InWidget->GetTransform();
	VVector2 Size = InWidget->GetSize();
	
	if (
			mouseX > Transforms.x &&
			mouseX < Transforms.x + Size.x &&
			mouseY > Transforms.y &&
			mouseY < Transforms.y + Size.y
		)
			{
			if (InWidget->GetChildCount() > 0)
			{
				for (int i = 0; i < InWidget->GetChildCount(); i++)
				{
					ClickWidget = CheckIntersection(InWidget->GetChildWidget(i), mouseX, mouseY);
					if (ClickWidget != 0)
						return ClickWidget;
				}
				
			}
			if (InWidget->GetEnabled())
			{
				return InWidget;
			}
			else return 0;	
			}
		else return 0;
		
	
	
}

void WidgetContainer::UpdateDetailPanel()
{
	for (size_t i = 0; i < DetailPanelList.size(); i++)
	{
		DetailPanelList[i]->Update();
	}


}

Widget* WidgetManager::GetClickedWidget(float inMouseX, float inMouseY,int WindowID)
{

	float mouseX, mouseY;

	mouseX = inMouseX;
	mouseY = inMouseY;
	

	if (GetWidgetCount() > 0)
	{
		Widget* ClickWidget;
		ClickWidget = 0;


		int RootChildCount = WidgetContainers[WindowID]->GetWidgetRoot()->GetChildCount();
		if(RootChildCount > 0)
			for (int i = RootChildCount-1; i > -1; i--)
			{
				ClickWidget = CheckIntersection(WidgetContainers[WindowID]->GetWidgetRoot()->GetChildWidget(i), int(mouseX), int(mouseY));
				if (ClickWidget != 0)
					return ClickWidget;
			}


		
		/*

		

			ClickWidget = CheckIntersection(WidgetContainers[WindowID]->GetWidgetRoot(),int(mouseX), int(mouseY));
			if (ClickWidget != 0)
				return ClickWidget;
				*/
		
		return 0;
	}


	else return 0;
}



int WidgetManager::GetWidgetCount()
{

	return int(WidgetContainers.size());

}

void WidgetManager::UpdateWidgets(ResourceManager* VManager)
{



}

void WidgetManager::Shutdown()
{
	
	ClearContainersArray();

}