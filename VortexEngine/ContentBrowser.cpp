#include "stdafx.h"
#include "ContentBrowser.h"
#include "Button.h"
#include "Text.h"
#include "SystemClass.h"
#include "Composer.h"

ContentBrowser::ContentBrowser()
{
	
	SetName("Content Browser");
	//bIsVisible = false;
	CurrentState = Hidden;
	

	Composer* VBottomComposer;
	VBottomComposer = new Composer;
	VBottomComposer->SetIsHorizontal(true);
	VBottomComposer->DistributionArray = { 0.3f,0.9f };
	this->AddChildWidget(VBottomComposer);
	VBottomComposer->SetInheritSize(true);

	
	FolderViewer = new Widget;
	FolderViewer->SetColor(0.3f, 0.3f, 0.3f,1);
	
	
	VBottomComposer->AddChildWidget(FolderViewer);

	AssetViewer = new Widget;
	AssetViewer->SetColor(0.3f, 0.3f, 0.3f,1);
	AssetViewer->OnRightMouseButtonDownDelegate.Bind<SystemClass, &SystemClass::OpenFileBrowser>(SystemClass::GetSystem());


	VBottomComposer->AddChildWidget(AssetViewer);
	

}

void ContentBrowser::SetSize(float SizeX, float SizeY)
{
	Widget::SetSize(SizeX,SizeY);
	GetChildWidget(0)->SetSize(SizeX,SizeY);


}

void ContentBrowser::LoadItem()
{
	
	SystemClass::GetSystem()->AddObjectToWorld();

}

void ContentBrowser::AddItem(string AssetName)
{

	Button* Item = new Button;
	int Id = AssetViewer->GetChildCount();

	Item->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
	Item->SetMouseOverEndColor(0.5f, 0.5f, 0.5f, 1.0f);
	Item->SetRelativeTransform(float(10 + (Id) * 65),5.0f);
	Item->SetSize(65.0f,100.0f);
	Item->SetMouseOverColor(0.8f, 0.8f,0.8f, 1);
	Item->SetDraggable(false);
	Item->SetName(AssetName);

	Item->OnLeftMouseButtonDownDelegate.Bind<ContentBrowser,&ContentBrowser::LoadItem>(this);
	AssetViewer->AddChildWidget(Item);


	Widget* ItemIcon = new Widget;
	ItemIcon->SetSize(Item->GetSize().x-10, Item->GetSize().x - 10);
	ItemIcon->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	ItemIcon->SetRelativeTransform((Item->GetSize().x-ItemIcon->GetSize().x)/2, 7.5);
	ItemIcon->SetEnabled(false);
	ItemIcon->SetName(Item->VWidgetName + "_Icon");
	Item->AddChildWidget(ItemIcon);

	Text* ItemName = new Text;
	ItemName->SetSize(ItemIcon->GetSize().x, 10.0f);
	ItemName->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	ItemName->SetRelativeTransform(ItemIcon->GetRelativeTransform().x, ItemIcon->GetSize().y+ ItemIcon->GetRelativeTransform().y+5);
	ItemName->SetEnabled(false);
	ItemName->Alignment = AlignmentCenter;
	ItemName->SetName(Item->VWidgetName + "_Name");
	ItemName->SetText(AssetName);
	Item->AddChildWidget(ItemName);

	


	return;


}



ContentBrowser::~ContentBrowser()
{
}

