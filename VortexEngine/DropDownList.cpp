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


Widget* DropDownList::AddElement(string TooltipText)
{
	float Offset = GetChildCount();
	Button* ButtonElement = CreateWidget<Button>();
	ButtonElement->SetRelativeTransform(0, Offset * 15);
	ButtonElement->SetSize(150,15);
	
	
		Text* NewTooltip = CreateWidget<Text>();
		NewTooltip->SetSize(150,12);
		NewTooltip->SetText(TooltipText);
		ButtonElement->AddChildWidget(NewTooltip);
		NewTooltip->SetState(WidgetState::Disabled);
		NewTooltip->Alignment = AlignmentLeft;
		NewTooltip->SetIsMultiline(false);
	SetSize(100, (Offset+1) * 15);
	AddChildWidget(ButtonElement);
	UpdateChildTransform();

	return ButtonElement;
}
