#include "stdafx.h"
#include "D2DClass.h"
#include <string> 

D2DClass::D2DClass()
{
	pBrush= NULL;
	fps = 0;
	showFPSFlag = false;
	showPolycontFlag = false;
	isDragging = false;
}


D2DClass::~D2DClass()
{
}

bool D2DClass::Initialize(HWND hwnd,IDXGISwapChain* swapChain)
{
	 HRESULT hr;
	 FLOAT dpiX, dpiY;

	 ID2D1Factory* pD2DFactory;
	 D2D1_FACTORY_OPTIONS options;
	 IDWriteFactory* writeFactory;
	

	 options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	 hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &pD2DFactory);
	
	 pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
	 bool result = IsProcessDPIAware();


		 hr = DWriteCreateFactory(
			 DWRITE_FACTORY_TYPE_SHARED,
			 __uuidof(IDWriteFactory),
			 reinterpret_cast<IUnknown**>(&writeFactory)
		 );
	 
		 hr = writeFactory->CreateTextFormat(
			 L"Arial",                // Font family name.
			 NULL,                       // Font collection (NULL sets it to use the system font collection).
			 DWRITE_FONT_WEIGHT_REGULAR,
			 DWRITE_FONT_STYLE_NORMAL,
			 DWRITE_FONT_STRETCH_NORMAL,
			 12.0f,
			 L"en-us",
			 &writeFormat
		 );
		 writeFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(0));
		// DWRITE_TEXT_ALIGNMENT_LEADING
		// DWRITE_TEXT_ALIGNMENT_CENTER


	 //GetWindowSize
	GetClientRect(hwnd, &clientSize);


	//InitBackBuffer
	IDXGISurface *dxgiBackbuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackbuffer));

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(dxgiBackbuffer, &props, &d2dRenderTarget);
	if (FAILED(hr))
		return false;


	
	DXGI_SWAP_CHAIN_DESC swapDesc;
	hr = swapChain->GetDesc(&swapDesc);

	
	hr = 	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White),&pBrush);
	if (FAILED(hr))
	{
		 return false;
	}
	


	if (writeFactory)
	{
		writeFactory->Release();
		writeFactory = 0;
	}

	if (pD2DFactory)
	{
		pD2DFactory->Release();
		pD2DFactory = 0;
	}

   dxgiBackbuffer->Release();
 
 return true;
}
void D2DClass::DrawFPS(int srcFPS)
{

	fps = srcFPS;

}

void D2DClass::DrawPolycount(int srcPolycount)
{

	polycount = srcPolycount;

}
void D2DClass::SetMousePos(int posX, int posY)
{	
		mouseX = posX;
		mouseY = posY;
}
void D2DClass::UpdateDragWidget()
{
	D2D1_RECT_F rect;
	rect = D2D1::RectF(mouseX-DragWidgetBrush.w_SizeX/2, mouseY- DragWidgetBrush.w_SizeY / 2, mouseX + DragWidgetBrush.w_SizeX/2,mouseY + DragWidgetBrush.w_SizeY/2);
	DragWidgetBrush.w_rect = rect;
}

void  D2DClass::Render()
{
	HRESULT hr = E_FAIL;


	




	if (d2dRenderTarget)
	{	
		d2dRenderTarget->BeginDraw();
		
	
		
		
		//draw FPS
		
		if (showFPSFlag = true)
		{

			int digitNumber;
			digitNumber = 2;
			int n;
			n = 2;
			while (fps > 1 * pow(10, n))
			{

				digitNumber += 1;
				n += 1;

			}

			const D2D1_RECT_F StatRect = D2D1::RectF(20, 50, 50, 80);
			wchar_t buffer[20];
			swprintf_s(buffer, size_t(20), L"%d",fps);
			d2dRenderTarget->DrawTextW(buffer, digitNumber+1, writeFormat, StatRect, pBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}
	

		if (showPolycontFlag = true)
		{
			const D2D1_RECT_F PolyCountRect = D2D1::RectF(20, 80, 150, 110);
			int digitNumber;
			digitNumber = 1;
			int n;
			n = 1;
			while (polycount > 1 * pow(10, n))
			{

				digitNumber += 1;
				n += 1;

			}
			wchar_t buffer[10];
			swprintf_s(buffer, size_t(10), L"%d", polycount);
			d2dRenderTarget->DrawTextW(buffer, digitNumber + 1, writeFormat, PolyCountRect, pBrush, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
		}

		
		hr = d2dRenderTarget->EndDraw();
		
	}

	
}






void D2DClass::Shutdown()
{
	if (pBrush)
	{
		pBrush->Release();
		pBrush= 0;
	}
	if (d2dRenderTarget)
	{
		d2dRenderTarget->Release();
		d2dRenderTarget = 0;
	}
	


	
	if (DragWidgetBrush.w_brush)
	{
		DragWidgetBrush.w_brush->Release();
		DragWidgetBrush.w_brush = 0;
	}
	if (DragWidgetBrush.w_outlineBrush)
	{
		DragWidgetBrush.w_outlineBrush->Release();
		DragWidgetBrush.w_outlineBrush = 0;
	}
	
	for (size_t i = 0; i < brushes.size(); i++)
	{
		//if (brushes.at(i).w_bitmap)
		//	brushes.at(i).w_bitmap->Release();
		if (brushes.at(i).w_brush)
		{
			brushes.at(i).w_brush->Release();
			brushes.at(i).w_outlineBrush->Release();
		}
	}
	brushes.clear();
	
}


