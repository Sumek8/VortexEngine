
#include "stdafx.h"
#include "Interface.h"
#include "Graphics.h"
#include "WidgetManager.h"

Interface::Interface()
{
}


Interface::~Interface()
{


}



void Interface::Initialize(WidgetManager* SourceManager)
{
	VWidgetManager = SourceManager;
	CreateContentBrowser();
	CreateWorldOutliner();
	CreateInfoPanel();
	UpdateOutliner();

	return;
};


void Interface::UpdateOutliner()
{
	/*

	Widget* wWidget;
	for (int i = 0; i < 5 ; i++)
	{
		wWidget = new Widget;

		VWidgetManager->AddWidget(wWidget, "WorldOutliner");
		wWidget->SetColor(0.6f, 0.6f, 0.6f, 1);
		wWidget->SetRelativeTransform(5.0f, float(i * 12+10));
		wWidget->isDraggable = false;
		wWidget->SetSize(190, 10);
	
		wWidget->SetName(string("WorldOutlinerBox").append(to_string(i)));
	
	}
	*/
	return;
}

void Interface::UpdateContentBrowser()
{

	Widget* wWidget;

	for (size_t i = 0; i < 5; i++)
	{

	
		

		wWidget = new Button;
		VWidgetManager->AddWidget(wWidget, "ContentBrowser");
		wWidget->SetColor(0.7f, 0.7f, 0.7f,1.0f);
		wWidget->SetRelativeTransform(float(25 + i * 75), 25.0f);
		wWidget->SetSize(25, 25);
		wWidget->SetName(string("Icon").append(to_string(i)));

		


	}

}

void Interface::CreateInfoPanel()
{
	Widget* wWidget;
	wWidget = new Widget;

	RECT window;

	GetClientRect(hwnd, &window);
	wWidget->SetColor(0.3f, 0.3f, 0.3f, 1);
	VWidgetManager->AddWidget(wWidget, "Root");
	wWidget->SetRelativeTransform(0,0);
	wWidget->SetSize(float( window.right), 25);
	wWidget->SetName(string("InfoPanel"));


	
	wWidget = new Widget;

	wWidget->SetColor(1.0f, 1.0f, 1.0f, 1);
	VWidgetManager->AddWidget(wWidget,"InfoPanel");
	wWidget->SetRelativeTransform(float(window.right-25),0.0f);
	wWidget->SetSize(25, 25);
	wWidget->SetName(string("Exit"));
	//wWidget->



}

void Interface::CreateContentBrowser()
{
	Widget* wWidget;
	wWidget = new Button;
	
	RECT window;
	
	GetClientRect(hwnd, &window);
	wWidget->SetColor(0.3f, 0.3f, 0.3f, 1);
	VWidgetManager->AddWidget(wWidget, "Root");
	wWidget->SetRelativeTransform(0,float(window.bottom-100));
	wWidget->SetSize(float(window.right),100.0f);
	wWidget->SetName(string("ContentBrowser"));
	

}

void Interface::CreateWorldOutliner()
{
	RECT window;
	
	GetClientRect(hwnd, &window);
	Widget* wWidget;
	float SizeX,SizeY;
	SizeX = 200.0f;
	SizeY = 200.0f;

	wWidget = new Widget;

	VWidgetManager->AddWidget(wWidget, "Root");
	wWidget->SetColor(0.3f, 0.3f, 0.3f, 1);
	wWidget->SetRelativeTransform(window.right - SizeX,25);
	wWidget->SetSize(SizeX, SizeY);
	wWidget->SetName(string("WorldOutliner"));




}


void Interface::SetViewportSize(HWND srchwnd)
{
	hwnd = srchwnd;
}

