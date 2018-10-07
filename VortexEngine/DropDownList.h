#pragma once
#include "Widget.h"
class DropDownList :
	public Widget
{
public:
	DropDownList();
	~DropDownList();
	
	void AddElement(string Text);
};

