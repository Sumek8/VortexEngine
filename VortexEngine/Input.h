
#pragma once
#include "stdafx.h"

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();
	
	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	LPPOINT GetMouseInput();
	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};



