#pragma once
#include "Widget.h"
class DropDownList :
	public Widget
{
public:
	DropDownList();
	~DropDownList();
	
	Widget* AddElement(string Text);
};

