#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>


using namespace std;
class WindowsCore
{
public:
	WindowsCore();
	~WindowsCore();



private:
	LPCWSTR			 VapplicationName;
	HINSTANCE		 Vhinstance;
	HWND			 Vhwnd;
	vector<HWND>	 VWindows;
	POINT		     cursorPos;
	string           ProjectDirectory;
};

