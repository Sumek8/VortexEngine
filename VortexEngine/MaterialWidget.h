#pragma once
#include "Widget.h"


class MaterialMaster;




struct SlotBinding
{
	int TargetWidgetID      = 0;
	int TargetWidgetSlotID  = 0;

	int SourceWidgetID      = 0;
	int SourceWidgetSlotID  = 0;
};

class MaterialWidget:
	public Widget
{
void 	OnMouseButtonDown();
void 	OnMouseButtonUp();
void    OnMouseOver();
void	OnMouseOverEnd();
void	StartDrag();
void	EndDrag();
void	CreateWidget();
public:
void	Initialize();
void SetBindingTarget(int ID);
void SetBindingSource(int ID);
public:
	
	int InputSlots;
	int OutputSlots;
	bool hasNodeView;
	MaterialWidget();
	~MaterialWidget();
	MaterialMaster* Master;
	vector<SlotBinding> SlotBindings;
	int ID;
};
