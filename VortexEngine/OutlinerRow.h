#pragma once
#include "Button.h"
class OutlinerRow :public Button
{
public:
	OutlinerRow();
	~OutlinerRow();
	void HideActor();
	void SelectActor();
	void OnMouseButtonDown();

	void OnWheelDown();
	void OnWheelUp();
	string ActorName;
	
};

