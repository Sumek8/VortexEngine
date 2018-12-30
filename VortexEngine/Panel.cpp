#include "stdafx.h"
#include "Panel.h"
#include "Text.h"

Panel::Panel()
{
	TabHeight = 20.0f;
	UnderlineSize = 3.0f;


	SetColor(0.2f, 0.2f, 0.2f,1);

	Widget* Underline;
	Underline = new Widget;
	Underline->SetName("Underline");
	Underline->SetColor(0.5, 0.5, 0.5, 1);
	Widget::AddChildWidget(Underline);
	
}

void Panel::UpdateChildTransform()
{
	
	//UpdateUnderline
	ChildWidgets[0]->SetParentTransform(GetTransform().x, GetTransform().y);
	ChildWidgets[0]->SetRelativeTransform(0, TabHeight);
	ChildWidgets[0]->SetSize(Size.x, UnderlineSize);
	ChildWidgets[0]->UpdateTransform();
	

	//Tabs
		for (size_t i = 2; i < ChildWidgets.size(); i += 2)
		{
		
			ChildWidgets[i]->SetParentTransform(GetTransform().x, GetTransform().y);
			ChildWidgets[i]->SetRelativeTransform(float(((i/2-1)*150 + 5)),UnderlineSize);
			ChildWidgets[i]->UpdateTransform();
			ChildWidgets[i]->UpdateChildTransform();
		}



		for (size_t i = 1; i < ChildWidgets.size(); i += 2)
		{
			ChildWidgets[i]->SetSize(Size.x, Size.y - TabHeight - UnderlineSize);
			ChildWidgets[i]->SetParentTransform(GetTransform().x, GetTransform().y);
			ChildWidgets[i]->SetRelativeTransform(0, TabHeight + UnderlineSize);
			ChildWidgets[i]->UpdateTransform();
			ChildWidgets[i]->UpdateChildTransform();
		}
		

}


void Panel::AddChildWidget(Widget* Child)
{

	Widget::AddChildWidget(Child);

	

		Widget* TabWidget;
		TabWidget = new Widget;
		TabWidget->SetSize(140.0f, float(TabHeight));
		TabWidget->SetColor(0.5f, 0.5f, 0.5f,1);
		TabWidget->SetName(Child->GetName());
		TabWidget->SetDraggable(true);
		TabWidget->OnDragDelegate.Bind<Panel,&Panel::MoveTab>(this);
		Widget::AddChildWidget(TabWidget);
			
		Text* TabText;
		TabText = new Text;
		TabText->Alignment = AlignmentCenter;
		TabText->SetRelativeTransform(0, 2);
		TabText->SetSize(140.0f, float(TabHeight-8));
		TabText->SetText(Child->GetName());
		TabWidget->AddChildWidget(TabText);
		

}

void Panel::MoveTab()
{



	for (size_t i = 2; i < ChildWidgets.size(); i += 2)
	{
		if (ChildWidgets[i]->GetState() == Dragged)
		{
			RemoveChild(i);
			RemoveChild(i - 1);
			
		}
	}



}

Panel::~Panel()
{
}
