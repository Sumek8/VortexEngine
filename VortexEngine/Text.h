#pragma once
#include "Widget.h"


enum VAlignment
{

	AlignmentLeft,
	AlignmentRight,
	AlignmentCenter,
};


class Text :
	public Widget
{

	
public:
	Text();
	~Text();
	void SetText(string Text);
	
	VAlignment Alignment;
	string VText;
	string GetText();
	
};

