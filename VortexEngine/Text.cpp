#include "stdafx.h"
#include "Text.h"


Text::Text()
{
	bIsEnabled = false;
	Type = TextWidget;
	Color = VColor(1, 1, 1, 1);
}

string Text::GetText()
{

	return VText;
}

void Text::SetText(string WidgetText)
{

	VText = WidgetText;

}
Text::~Text()
{
}
