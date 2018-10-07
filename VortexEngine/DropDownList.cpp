#include "stdafx.h"
#include "DropDownList.h"
#include "Button.h"
#include "Text.h"
#include "SystemClass.h"
DropDownList::DropDownList()
{
	SetIsVisible(false);
	SetName("DropDownList");
}


DropDownList::~DropDownList()
{

}


void DropDownList::AddElement(string TooltipText)
{
	float Offset = GetChildCount();
	Button* ButtonElement = CreateWidget<Button>();
	ButtonElement->SetRelativeTransform(0, Offset * 15);
	ButtonElement->SetSize(100,15);
	ButtonElement->SetColor(0.1, 0.1, 0.1, 1);
	
		Text* NewTooltip = CreateWidget<Text>();
		NewTooltip->SetSize(100,12);
		NewTooltip->SetText(TooltipText);
		ButtonElement->AddChildWidget(NewTooltip);
		NewTooltip->SetState(WidgetState::Disabled);

	SetSize(100, (Offset+1) * 15);
	AddChildWidget(ButtonElement);
	UpdateChildTransform();
}
