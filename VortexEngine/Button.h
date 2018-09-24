#pragma once
#include "Widget.h"
#include "Delegate.h"
class Button :
	public Widget
{
public:
	Button();
	~Button();
	virtual	void OnMouseButtonDown();
	void		 OnMouseOver();
	void		 OnMouseOverEnd();
	virtual	void OnMouseButtonUp();


	
	//void OnMouseButtonDownBind();



	void Initialize();
	void OnRightButtonDown();

	
	void SetMouseOverColor(float r, float g, float b, float a);
	void SetMouseOverEndColor(float r, float g, float b, float a);
	
private:
	string Tooltip;
	VColor MouseOverColor;
	VColor MouseOverEndColor;
};

