#include "stdafx.h"
#include "Widget.h"
#include "Graphics.h"
#include "SystemClass.h"



Widget::Widget()
{
	bIsClipping = false;
	bIsEnabled = true;
	bIsVisible = true;
	bIsDraggable = false;
	bInheritSize = false;
	ZOrder = 0;
	RelativeTransform.x = 0;
	RelativeTransform.y = 0;
	ParentTransform.x = 0;
	ParentTransform.y = 0;


	Type = BaseWidget;
}
WidgetType Widget::GetType()
{

	return Type;

}
void Widget::SetIsClipping(bool isClipping)
{

	bIsClipping = isClipping;
}
void Widget::SetInheritSize(bool InheritsSize)
{
	bInheritSize = InheritsSize;
}
bool Widget::GetIsClipping()
{

	return bIsClipping;

}

bool Widget::GetInheritSize()
{
	return bInheritSize;
}

void Widget::LockDragAxis(bool LockAxisX,bool LockAxisY)
{
			 bIsAxisXLocked = LockAxisX;
			 bIsAxisYLocked = LockAxisY;
}

void Widget::SetIsVisible(bool NewVisibility)
{

	bIsVisible = NewVisibility;

}

Widget::~Widget()
{
	
}


 void Widget::SetEnabled(bool Enabled)
{
	bIsEnabled = Enabled;
}

void Widget::SetDraggable(bool Draggable)
{

	bIsDraggable = Draggable;

}
 bool Widget::GetEnabled ()
{

	return bIsEnabled;

}
 bool Widget::GetIsDraggable()
{
	return bIsDraggable;
}

 void Widget::SetState(WidgetState NewState)
 {
	 CurrentState = NewState;

 }
WidgetState  Widget::GetState()
 {

	 return CurrentState;
}

void Widget::Initialize()
{
	
}



 string Widget::GetName()
{

	return VWidgetName;

}

void Widget::SetName(string name)
{

	VWidgetName = name;

}

int Widget::GetChildCount()
{

	return int(ChildWidgets.size());

}

void Widget::AddChildWidget(Widget* ChildWidget)
{
	if (ChildWidget == this)
		throw;

	ChildWidget->SetParentTransform(Transform.x, Transform.y);
	ChildWidget->UpdateTransform();
	ChildWidgets.push_back(ChildWidget);


}

void Widget::UpdateChildTransform()
{
	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		ChildWidgets[i]->SetParentTransform(GetTransform().x, GetTransform().y);
		ChildWidgets[i]->UpdateTransform();
		ChildWidgets[i]->UpdateChildTransform();
	}

}

Widget* Widget::GetChildWidget(int ID)
{

	return ChildWidgets[ID];


}



void Widget::SetRelativeTransform(float x, float y)
{
	RelativeTransform.x = x;
	RelativeTransform.y = y;
	

}


bool Widget::GetIsVisible()
{


	return bIsVisible;
}

VVector2 Widget::GetRelativeTransform()
{

	return RelativeTransform;
}

int Widget::GetZOrder()
{

	return ZOrder;
}

VColor Widget::GetOutlineColor()
{


	return OutlineColor;
}

VColor Widget::GetColor()
{

	return Color;
}

VVector2 Widget::GetSize()
{
	return Size;
}

VVector2 Widget::GetTransform()
{

	return Transform;
}

bool Widget::GetIsDragXLocked()
{
	return bIsAxisXLocked;
}
bool Widget::GetIsDragYLocked()
{
	return bIsAxisYLocked;

}


 void Widget::SetZOrder(int Order)
{

	 ZOrder = Order;

}
 void Widget::SetColor(VColor NewColor)
{

	 Color = NewColor;
	 return;

}
void Widget::SetColor(float r, float g, float b, float a)
{

	Color.r = r;
	Color.g = g;
	Color.b = b;
	Color.a = a;

	OutlineColor.r = r*0.75f;
	OutlineColor.g = g*0.75f;
	OutlineColor.b = b*0.75f;
	OutlineColor.a = a;
}

void Widget::OnWheelUp()
{

}
void Widget::OnWheelDown()
{
	
}
void Widget::SetSize(float newSizeX, float newSizeY)
{

	Size.x = newSizeX;
	Size.y = newSizeY;


}
void Widget::SetTransform(float x, float y)
{
	Transform.x = x;
	Transform.y = y;

}

void Widget::SetParentTransform(float x, float y)
{
	ParentTransform.x = x;
	ParentTransform.y = y;

}

void Widget::UpdateTransform()
{
	Transform.x = RelativeTransform.x + ParentTransform.x;
	Transform.y = RelativeTransform.y + ParentTransform.y;
	

}
vector<Widget*>Widget::GetChildren()
{

	return ChildWidgets;
}

void Widget::StartDrag()
{
	CurrentState = Dragged;

}

VVector2 Widget::GetParentTransform()
{

	return ParentTransform;

}

void Widget::OnDrag()
{
	OnDragDelegate.Execute();
}


void Widget::OnLeftButtonDoubleClick()
{
	OnLeftMouseDoubleClickDelegate.Execute();
}

void Widget::OnRightButtonDown()
{
	OnRightMouseButtonDownDelegate.Execute();
}

void Widget::EndDrag()
{
	CurrentState = InActive;

}

void Widget::OnMouseButtonDown()
{
	OnLeftMouseButtonDownDelegate.Execute();
}


void Widget::OnMouseOverEnd()
{

}
void Widget::OnMouseOver()
{

}
void Widget::OnMouseButtonUp()
{

}
void Widget::RemoveChildren()
{
	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		ChildWidgets[i]->Shutdown();
		delete	ChildWidgets[i];
		ChildWidgets[i] = 0;
	}
	ChildWidgets.clear();
}
void Widget::Shutdown()
{
	
	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		ChildWidgets[i]->Shutdown();
		delete	ChildWidgets[i];
		ChildWidgets[i] = 0;
	}
	ChildWidgets.clear();
	
}

void Widget::RemoveChild(int ID)
{
	
	GetChildWidget(ID)->Shutdown();
	ChildWidgets.erase(ChildWidgets.begin() + ID);

}


void Widget::Destroy()
{
	
	WidgetManager::GetWidgetManager()->RemoveWidget(GetName());

}