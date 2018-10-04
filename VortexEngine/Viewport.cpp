
#include "stdafx.h"
#include "Viewport.h"

Viewport::Viewport()
{

	SetName("Viewport");
	Type = ViewportWidget;
	SetColor(1, 1, 1, 1);
	ShowDebugWindow();
	bShowDebug = true;
}
float Viewport::GetViewportAspect()
{

	return Size.x / Size.y;

}

Viewport::~Viewport()
{
}


void Viewport::ToggleDebugInfo()
{
	bShowDebug = !bShowDebug;
	for each (Widget* ChildWidget in ChildWidgets)
	{
		ChildWidget->SetIsVisible(bShowDebug);
	}
	
	
}

void Viewport::ShowDebugWindow()
{
	Text* FPSCounterWidget;
	FPSCounterWidget = new Text;
FPSCounterWidget->SetRelativeTransform(5, 5);
FPSCounterWidget->SetSize(180, 14);
FPSCounterWidget->SetText("FPS:");
AddChildWidget(FPSCounterWidget);
FPSCounter = FPSCounterWidget;


Text* PolyCounterWidget;
PolyCounterWidget = new Text;
PolyCounterWidget->SetRelativeTransform(5, 20);
PolyCounterWidget->SetSize(180, 14);
PolyCounterWidget->SetText("Polycount:");
AddChildWidget(PolyCounterWidget);
PolyCounter = PolyCounterWidget;


Text* DrawCallWidget;
DrawCallWidget = new Text;
DrawCallWidget->SetRelativeTransform(5, 35);
DrawCallWidget->SetSize(180, 14);
DrawCallWidget->SetText("DrawCallCount:");
DrawCallCounter = DrawCallWidget;
AddChildWidget(DrawCallCounter);
}
void Viewport::UpdateFPS(int FpsCount)
{
	
	string FPSCount = "FPS: " + to_string(FpsCount);
	FPSCounter->SetText(FPSCount);


}

void Viewport::UpdateDrawCall(int DrawCallCount)
{
	string DrawCalls = "DrawCallCount: " + to_string(DrawCallCount);
	DrawCallCounter->SetText(DrawCalls);
}
void Viewport::UpdatePolycount(int PolygonCount)
{
	string PolyCount = "Polycount: " + to_string(PolygonCount);
	PolyCounter->SetText(PolyCount);

}

