#include "stdafx.h"
#include "MaterialWidgetSlot.h"


MaterialWidgetSlot::MaterialWidgetSlot()
{
}


MaterialWidgetSlot::~MaterialWidgetSlot()
{
}


void MaterialWidgetSlot::OnMouseButtonUp()
{

	
	
}

void MaterialWidgetSlot::OnMouseButtonDown()
{
	
	OnLeftMouseButtonDownDelegate.Execute(ID);
}