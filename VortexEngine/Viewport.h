#pragma once
#include "Widget.h"
#include "Text.h"

class Viewport :
	public Widget
{
public:
	float GetViewportAspect();
	Viewport();
	~Viewport();
	void ToggleDebugInfo();
	void ShowDebugWindow();
	void UpdateFPS(int Fps);
	void UpdateDrawCall(int DrawCallCount);
	void UpdatePolycount(int PolygonCount);
	bool bShowDebug;
	Text* FPSCounter;
   Text* PolyCounter;
   Text* DrawCallCounter;
};

