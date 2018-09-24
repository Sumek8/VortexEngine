#include "stdafx.h"
#include "WorldOutliner.h"
#include "ListBox.h"
#include "OutlinerRow.h"

WorldOutliner::WorldOutliner()
{


	SetName("World Outliner");
	SetColor(0.3f, 0.3f, 0.3f, 1);


	
	CreateList();
	
	


}


WorldOutliner::~WorldOutliner()
{


}

void WorldOutliner::CreateList()
{
	ListBox* List = new ListBox;
	
	AddChildWidget(List);

}
void WorldOutliner::ClearList()
{
	GetChildWidget(0)->Shutdown();


}
void WorldOutliner::SetSize(float newSizeX, float newSizeY)
{
	Widget::SetSize(newSizeX,newSizeY);
	GetChildWidget(0)->SetSize(newSizeX, newSizeY);

}
void WorldOutliner::AddActor(Actor* NewActor)
{
	float SizeY;
	SizeY = 18;

	ListBox* List = static_cast<ListBox*>(GetChildWidget(0));

	OutlinerRow* ActorWidget;
	ActorWidget = new OutlinerRow;
	ActorWidget->SetRelativeTransform(5.0f, SizeY * GetChildCount());
	ActorWidget->SetColor(0.5f, 0.5f, 0.5f, 1);
	ActorWidget->SetMouseOverColor(1, 1, 1, 1);
	ActorWidget->SetMouseOverEndColor(0.5f, 0.5f, 0.5f, 1);
	ActorWidget->OnWheelUpDelegate.Bind<ListBox, &ListBox::OnWheelUp>(List);
	ActorWidget->OnWheelDownDelegate.Bind<ListBox, &ListBox::OnWheelDown>(List);
	ActorWidget->SetName(string("WorldOutlinerBox").append(to_string(GetChildCount())));
	ActorWidget->ID = GetChildWidget(0)->GetChildCount();
	ActorWidget->ActorName = NewActor->GetName();


	Button* VisibilityIcon;
	VisibilityIcon = new Button;
	VisibilityIcon->SetSize(10.0f, 10.0f);
	VisibilityIcon->SetRelativeTransform(1.0f, (SizeY - 10) / 2);
	VisibilityIcon->SetColor(0.8f, 0.8f, 0.8f, 1);
	VisibilityIcon->SetMouseOverColor(1, 1, 1, 1);
	VisibilityIcon->SetMouseOverEndColor(0.8f, 0.8f, 0.8f, 1);
	VisibilityIcon->OnLeftMouseButtonDownDelegate.Bind<OutlinerRow, &OutlinerRow::HideActor>(ActorWidget);
	VisibilityIcon->SetName(string("VisibilityIcon").append(to_string(GetChildCount())));
	ActorWidget->AddChildWidget(VisibilityIcon);


	Text* IconText;
	IconText = new Text;
	IconText->SetSize(170.0f, float(SizeY - 6));
	IconText->SetRelativeTransform(15.0f, 0);
	IconText->SetColor(1, 1, 1, 1);
	IconText->Alignment = AlignmentLeft;
	IconText->SetName(string("TextIcon").append(to_string(GetChildCount())));
	IconText->SetText(NewActor->GetName());
	ActorWidget->AddChildWidget(IconText);

	static_cast<ListBox*>(ChildWidgets[0])->AddChildWidget(ActorWidget);

	UpdateChildTransform();

}
/*
void WorldOutliner::AddStaticMeshActor(StaticMeshActor* StaticMesh)
{
	
	float SizeY;
	SizeY = 18;

	ListBox* List = static_cast<ListBox*>(GetChildWidget(0));

		OutlinerRow* ActorWidget;
		ActorWidget = new OutlinerRow;
		ActorWidget->SetRelativeTransform(5.0f, SizeY * GetChildCount());
		ActorWidget->SetColor(0.5f, 0.5f, 0.5f, 1);
		ActorWidget->SetMouseOverColor(1, 1, 1, 1);
		ActorWidget->SetMouseOverEndColor(0.5f, 0.5f, 0.5f, 1);
		ActorWidget->OnWheelUpDelegate.Bind<ListBox, &ListBox::OnWheelUp>(List);
		ActorWidget->OnWheelDownDelegate.Bind<ListBox, &ListBox::OnWheelDown>(List);
		ActorWidget->SetName(string("WorldOutlinerBox").append(to_string(GetChildCount())));
		ActorWidget->ID = GetChildWidget(0)->GetChildCount();
		

		
		Button* VisibilityIcon;
		VisibilityIcon = new Button;
		VisibilityIcon->SetSize(10.0f, 10.0f);
		VisibilityIcon->SetRelativeTransform(1.0f, (SizeY - 10) / 2);
		VisibilityIcon->SetColor(0.8f, 0.8f,0.8f,1);
		VisibilityIcon->SetMouseOverColor(1, 1, 1, 1);
		VisibilityIcon->SetMouseOverEndColor(0.8f, 0.8f, 0.8f, 1);
		VisibilityIcon->OnLeftMouseButtonDownDelegate.Bind<OutlinerRow,&OutlinerRow::HideActor>(ActorWidget);
		VisibilityIcon->SetName(string("VisibilityIcon").append(to_string(GetChildCount())));
		ActorWidget->AddChildWidget(VisibilityIcon);


		Text* IconText;
		IconText = new Text;
		IconText->SetSize(170.0f, float(SizeY-6));
		IconText->SetRelativeTransform(15.0f,0);
		IconText->SetColor(1, 1,1, 1);
		IconText->Alignment = AlignmentLeft;
		IconText->SetName(string("TextIcon").append(to_string(GetChildCount())));
		IconText->SetText(StaticMesh->GetName());
		ActorWidget->AddChildWidget(IconText);
	
		static_cast<ListBox*>(ChildWidgets[0])->AddChildWidget(ActorWidget);
		
		UpdateChildTransform();
}
*/
void WorldOutliner::SetWidgetOffset(int Offset)
{
	WidgetOffset = Offset;
}

int WorldOutliner::GetWidgetOffset()
{
	return WidgetOffset;

}

void WorldOutliner::AddSkeletalMeshActor(SkeletalMeshActor* SkeletalMesh)
{


}



void WorldOutliner::CreateIcons()
{

}
