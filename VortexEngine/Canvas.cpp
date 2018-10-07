#include "stdafx.h"
#include "Canvas.h"


Canvas::Canvas()
{
	bIsVisible = false;
}


Canvas::~Canvas()
{
}


void Canvas::UpdateChildTransform()
{
	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		ChildWidgets[i]->SetParentTransform(GetTransform().x, GetTransform().y);
		ChildWidgets[0]->SetSize(GetSize().x, GetSize().y);
		ChildWidgets[i]->UpdateChildTransform();
	}

}