#pragma once
#include "Button.h"
#include "Text.h"

class InputBox :
	public Text
{
public:
	Delegate OnChangeDelegate;
	InputBox();
	~InputBox();
};

