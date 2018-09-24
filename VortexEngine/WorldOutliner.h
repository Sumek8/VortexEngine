#pragma once
#include "Widget.h"
#include "Button.h"
#include "Object.h"
#include "World.h"
#include "Text.h"

class WorldOutliner : public Widget
{
public:
	void SetWidgetOffset(int Offset);
	int GetWidgetOffset();
	//void AddStaticMeshActor(StaticMeshActor* StaticMesh);
	void AddSkeletalMeshActor(SkeletalMeshActor* SkeletalMesh);
	void CreateIcons();
	void CreateList();
	void ClearList();
	WorldOutliner();
	~WorldOutliner();
	void SetSize(float newSizeX, float newSizeY);
	void AddActor(Actor * LightActor);
	int WidgetOffset;
};

