#pragma once
#include "Widget.h"
class Canvas :
	public Widget
{
public:
	Canvas();
	~Canvas();
	void UpdateChildTransform();
};

