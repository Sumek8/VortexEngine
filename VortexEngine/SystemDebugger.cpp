#include "stdafx.h"
#include "SystemDebugger.h"
#include <ctime>

SystemDebugger::SystemDebugger()
{
	tickTime = clock();

}


SystemDebugger::~SystemDebugger()
{
}

int SystemDebugger::GetFPS()
{

	int fps = 0;

	
		fps = 1 / (clock() - tickTime) * 1000;
		tickTime = clock();
		
		
	
	return fps;

}
