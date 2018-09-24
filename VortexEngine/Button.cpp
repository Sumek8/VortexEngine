#include "stdafx.h"
#include "Button.h"
#include "SystemClass.h"

Button::Button()
{
	SetColor(0.7f, 0.7f, 0.7f, 1.0f);
	MouseOverColor = VColor(1, 1, 1, 1);
	MouseOverEndColor = Color;
	Type = ButtonWidget;
}



Button::~Button()
{
}



void Button::Initialize()
{


	
	
	
}


void Button::OnMouseOver()
{
	SetColor(MouseOverColor);
}

void Button::OnMouseOverEnd()
{
	Color = MouseOverEndColor;
	
}

void Button::SetMouseOverColor(float r,float g,float b,float a)
{
	MouseOverColor = VColor(r, g, b, a);

}

void Button::SetMouseOverEndColor(float r, float g, float b, float a)
{
	MouseOverEndColor = VColor(r, g, b, a);

}

void Button::OnMouseButtonDown()
{

		OnLeftMouseButtonDownDelegate.Execute();

}

void Button::OnMouseButtonUp()
{
}
void Button::OnRightButtonDown()
{

	
}
