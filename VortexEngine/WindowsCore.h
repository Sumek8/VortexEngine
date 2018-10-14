#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>
#include <vector>

#include <fstream>
#include <Commdlg.h>
#include <shobjidl.h> 
#include <sys/types.h>
#include <experimental/filesystem>
#include <iostream>
#include <Lmcons.h>


using namespace std;
class WindowsCore
{
public:
	WindowsCore();
	~WindowsCore();
	void OpenFileBrowser();


private:
	LPCWSTR			 VapplicationName;
	HINSTANCE		 Vhinstance;
	HWND			 Vhwnd;
	vector<HWND>	 VWindows;
	POINT		     cursorPos;
	string           ProjectDirectory;
};

