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
	bool GetIsMultiline();
	VAlignment Alignment;
	string GetText();
	void SetIsMultiline(bool isMultiline);
private:
	string VText;
	bool   isMultiline;
};

