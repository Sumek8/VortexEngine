#pragma once
#pragma comment (lib, "Dwrite.lib")
#include <d2d1_1.h>
#include <windows.h>
#include <Wincodec.h>
#include <Dwrite.h>
#include <vector>




using namespace std;

struct WidgetBrush
{
	ID2D1SolidColorBrush* w_brush;
	ID2D1SolidColorBrush* w_outlineBrush;
	int w_zOrder;
	bool w_isVisible;
	bool w_hasBitmap;
	D2D1_RECT_F w_rect;
	ID2D1Bitmap* w_bitmap;
	string w_tooltip;
	float w_SizeX, w_SizeY;
};


class D2DClass

{
public:
	D2DClass();
	~D2DClass();
	bool Initialize(HWND hwnd, IDXGISwapChain* swapchain);
	void Render();
	void Shutdown();
	void LoadTexture(LPCWSTR filepath,ID2D1Bitmap* bitmap);
	void DrawFPS(int srcFPS);
	void DrawPolycount(int srcPolycount);
	
	void UpdateDragWidget();
	void SetMousePos(int posX, int posY);
	void RemoveDragWidget();
	

private :
	
	bool				    isDragging;
	WidgetBrush				DragWidgetBrush;
	ID2D1SolidColorBrush* pBrush;
	RECT clientSize;
	ID2D1RenderTarget *d2dRenderTarget; 
	IDWriteTextFormat* writeFormat;
	vector<WidgetBrush> brushes;
	size_t widgetsQuantity;
	bool showFPSFlag,showPolycontFlag;
	int fps;
	int polycount;
	int mouseX, mouseY;

	
};

