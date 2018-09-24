////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "inputClass.h"
#include <stdio.h>

inputClass::inputClass()
{
}


inputClass::inputClass(const inputClass& other)
{
}


inputClass::~inputClass()
{
}


void inputClass::Initialize()
{
	int i;


	// Initialize all the keys to being released and not pressed.
	for (i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}


void inputClass::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	
	//char dest[100];
	//sprintf_s(dest, size_t(100), "Count   %d", input);
	//OutputDebugStringA(dest);

	
	return;
}


void inputClass::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool inputClass::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

LPPOINT inputClass::GetMouseInput()
//GetMouseInput()
{
	POINT MousePos;
	


	if (GetCursorPos(&MousePos))
		return &MousePos;
	else return 0;
}
