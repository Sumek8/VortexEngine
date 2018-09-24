#pragma once
#include "Widget.h"
class Panel :
	public Widget
{
public:
	void UpdateChildTransform();
	
	void AddChildWidget(Widget* ChildWidget);
	int ActiveChildID;
	void MoveTab();
	Panel();
	~Panel();
private:
	float TabHeight;
	float UnderlineSize;
};

