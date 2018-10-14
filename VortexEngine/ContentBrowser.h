#pragma once
#include "Widget.h"
#include <vector>

class ContentBrowser : public Widget
{

	enum ResourceType
	{
		T_None,
		T_Texture,
		T_Material,
		T_StaticMesh,
		T_Skeleton,
		T_Animation,

	};

public:
	ContentBrowser();
	
	~ContentBrowser();
	void	Update();
	void	AddItem(string AssetName,ResourceType Type);
	Widget* AssetViewer;
	Widget* FolderViewer;
	void SetSize(float SizeX, float SizeY);
	void LoadItem();
	shared_ptr<Widget*> Viewer;
};

