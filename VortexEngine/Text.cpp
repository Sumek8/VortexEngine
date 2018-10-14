#include "stdafx.h"
#include "Text.h"


Text::Text()
{
	bIsEnabled = false;
	Type = TextWidget;
	Color = VColor(1, 1, 1, 1);
	isMultiline = true;
}
bool Text::GetIsMultiline()
{
	return isMultiline;

}
string Text::GetText()
{

	return VText;
}

void Text::SetIsMultiline(bool bisMultiline)
{
	isMultiline = bisMultiline;
}

void Text::SetText(string WidgetText)
{

	VText = WidgetText;

}
Text::~Text()
{
}
