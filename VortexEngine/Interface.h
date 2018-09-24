#pragma once
#pragma comment(lib, "windowscodecs.lib")
#include <d2d1_1.h>
#include <Wincodec.h>
#include <vector>
#include "Widget.h"


class SystemClass;
class WidgetManager;


class Interface
{
public:
	Interface();
	~Interface();
	void Initialize(WidgetManager* VWidgetManager);
	void SetViewportSize(HWND hwnd);
	
	void UpdateOutliner();
	void UpdateContentBrowser();
	void CreateInfoPanel();
private:
	HWND hwnd;
	void CreateContentBrowser();
	void CreateWorldOutliner();
	WidgetManager* VWidgetManager;
};

