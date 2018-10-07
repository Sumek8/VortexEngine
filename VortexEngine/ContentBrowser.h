#pragma once
#include "Widget.h"
#include <vector>

class ContentBrowser : public Widget
{
public:
	void CreateListWidget();
	ContentBrowser();
	~ContentBrowser();
	void AddItem(string AssetName);
	Widget* AssetViewer;
	Widget* FolderViewer;
	void SetSize(float SizeX, float SizeY);
	void LoadItem();
	shared_ptr<Widget*> Viewer;
};

