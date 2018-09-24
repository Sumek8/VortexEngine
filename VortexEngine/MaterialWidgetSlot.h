#pragma once
#include "Button.h"
#include "MaterialWidget.h"



class MaterialWidgetSlot :
	public Button
{
public:
	MaterialWidgetSlot();
	~MaterialWidgetSlot();
	bool isInputSlot;
	void OnMouseButtonDown();
	void OnMouseButtonUp();

	int ID;
};

