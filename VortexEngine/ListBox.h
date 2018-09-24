#pragma once
#include "Widget.h"



class ListBox :
	public Widget
{
public:
	ListBox();
	~ListBox();

	void CalculateChildrenVisibility();

	void AddChildWidget(Widget* ChildWidget);
	float ChildrenHeight;
	void OnWheelUp();
	void OnWheelDown();
	float Offset;
	const int DeltaPosition = 5;
};

