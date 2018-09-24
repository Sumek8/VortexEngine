
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
FPSCounterWidget->SetSize(120, 12);
FPSCounterWidget->SetText("FPS:");
AddChildWidget(FPSCounterWidget);
FPSCounter = FPSCounterWidget;


Text* PolyCounterWidget;
PolyCounterWidget = new Text;
PolyCounterWidget->SetRelativeTransform(5, 20);
PolyCounterWidget->SetSize(120, 12);
PolyCounterWidget->SetText("Polycount:");
AddChildWidget(PolyCounterWidget);
PolyCounter = PolyCounterWidget;


Text* DrawCallWidget;
DrawCallWidget = new Text;
DrawCallWidget->SetRelativeTransform(5, 35);
DrawCallWidget->SetSize(120, 12);
DrawCallWidget->SetText("Polycount:");
DrawCallCounter = DrawCallWidget;
AddChildWidget(DrawCallCounter);
}
void Viewport::UpdateFPS(int FpsCount)
{
	
	char Buffer[20];
	int bufferSize;
		sprintf_s(Buffer,"FPS: %d",FpsCount);
	FPSCounter->SetText(Buffer);


}

void Viewport::UpdateDrawCall(int DrawCallCount)
{
	char Buffer[20];
	int bufferSize;
	sprintf_s(Buffer, "DrawCalls: %d", DrawCallCount);
	DrawCallCounter->SetText(Buffer);


}
void Viewport::UpdatePolycount(int PolygonCount)
{

	char Buffer[20];
	int bufferSize;
	sprintf_s(Buffer, "Polycount: %d", PolygonCount);
	PolyCounter->SetText(Buffer);

}

