#include "stdafx.h"
#include "ListBox.h"
#include "SystemClass.h"

ListBox::ListBox()
{
	SetName("ListBox");
	bIsVisible = false;
	bIsEnabled = true;
	Color = VColor(1,0, 0, 1);
	ChildrenHeight = 15;
}



ListBox::~ListBox()
{
}

void ListBox::CalculateChildrenVisibility()
{

	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{	
	
		if ((ChildWidgets[i]->GetRelativeTransform().y + ChildWidgets[i]->GetSize().y) > Size.y || ChildWidgets[i]->GetRelativeTransform().y < 0)
		{
			ChildWidgets[i]->SetIsVisible(false);	
			for (size_t j = 0; j <ChildWidgets[i]->GetChildren().size(); j++)
			{
				ChildWidgets[i]->GetChildren()[j]->SetIsVisible(false);
			}
			
		}
		else
		{
			ChildWidgets[i]->SetIsVisible(true);
			for (size_t j = 0; j <ChildWidgets[i]->GetChildren().size(); j++)
			{
				ChildWidgets[i]->GetChildren()[j]->SetIsVisible(true);
			}
		}

	}



}

void ListBox::OnWheelUp()
{

	if (ChildWidgets.size() > 0)
		if (ChildWidgets[0]->GetRelativeTransform().y >= 0)
			return;

	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		VVector2 NewTransform = ChildWidgets[i]->GetRelativeTransform();
		ChildWidgets[i]->SetRelativeTransform(NewTransform.x, NewTransform.y + DeltaPosition);
	}
	UpdateChildTransform();
	CalculateChildrenVisibility();
	
	SystemClass::GetSystem()->UpdateInterface();
	
}
void ListBox::OnWheelDown()
{

	if (ChildWidgets.size() > 0)
		if (ChildWidgets[ChildWidgets.size()-1]->GetRelativeTransform().y + ChildWidgets[ChildWidgets.size() - 1]->GetSize().y <= Size.y)
			return;

	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		VVector2 NewTransform = ChildWidgets[i]->GetRelativeTransform();
		ChildWidgets[i]->SetRelativeTransform(NewTransform.x, NewTransform.y - DeltaPosition);
	}
	UpdateChildTransform();
	CalculateChildrenVisibility();
	
	SystemClass::GetSystem()->UpdateInterface();
}

void ListBox::AddChildWidget(Widget* ChildWidget)
{
	
	ChildWidget->SetRelativeTransform(0,GetChildCount() * ChildrenHeight);
	ChildWidget->SetParentTransform(Transform.x, Transform.y);
	ChildWidget->SetSize(Size.x,ChildrenHeight);
	ChildWidget->UpdateTransform();
	ChildWidgets.push_back(ChildWidget);

	if (GetChildCount()*ChildrenHeight > Size.y)
	{
		ChildWidget->SetIsVisible(false);
		for (size_t j = 0; j <ChildWidget->GetChildCount(); j++)
		{
			ChildWidget->GetChildren()[j]->SetIsVisible(false);
		}

	}
	
	
	
}
