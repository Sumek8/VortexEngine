
#pragma once
#pragma comment(lib, "d3dcompiler.lib")




#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <vector>

#include "Widget.h"
#include "Math.h"
#include "ResourceManager.h"
#include "WidgetManager.h"
#include "MaterialWidgetSlot.h"
#include "MaterialWidget.h"
#include "Input.h"


using namespace DirectX;
using namespace std;

enum NodeType
{
	Float,
	Float2,
	Float3,
	Float4,
};


struct BindLink
{
	MaterialWidget* SourceNode;
	int SourceNodeSlot;

	MaterialWidget* TargetNode;
	int TargetNodeSlot;
	Line* Link;
};



class MaterialMaster
{

private:

	Widget*	CreateMaterialWidget(int ID);
	Widget* CreateMaterialOutputNode();
	void AddConstant3Vector();
	void AddConstant2Vector();
	void AddConstantVector();
	
	
	
	void BindNodes();
	void FinalizeShader();
	void BeginShader();
	void CreatePixelInput();
	void CreateConstantBuffers();
	void GetChannelValue(int Channel);
	void AddTextureSampler(int ID);
	void AddSampler();


	void ReplaceLinks();

	void SampleTexture(int ID);

	void Math(MaterialWidget Node1, MaterialWidget Node2,string Expression);

	void SetBaseColorOutput();
	void SetNormalOutput(MaterialWidget Node);

	void Dot(MaterialWidget Node1, MaterialWidget Node2);
	void Add(MaterialWidget Node1, MaterialWidget Node2);
	void Subtract(MaterialWidget Node1, MaterialWidget Node2);
	void Multiply(MaterialWidget Node1, MaterialWidget Node2);
	void Divide(MaterialWidget Node1, MaterialWidget Node2);
	void Time();
	

	

public:
	bool CompileShader();
	MaterialMaster();
	~MaterialMaster();
	int GetNodeByName(string Name);
	void Shutdown();
	void Initialize(WidgetManager* VWidgetManagerm,Input* VInput);

	void CreateWidgets();
	void SetBindingAnchor(MaterialWidget* Node, int SlotID, bool isInputSlot);
	void ClearBinding(MaterialWidget* Node, int SlotID, bool isInputSlot);
	void UpdateLinks();
	void EndBinding();
	void CreateLinks();
	bool IsBinding;
	HWND VMaterialHwnd;
	Line* ActiveLink;
private:

	Input* VInput;
	WidgetManager* VWidgetManager;
	string CodeArray;
	string MaterialPath;
	int TextureSampleCount;
	
	void LoadNodes();
	vector<BindLink>BindingArray;
	vector<MaterialWidget*>MaterialWidgets;
	Widget* WidgetNodeGraph;
	WidgetContainer* WidgetNodeContainer;
	BindLink Binding;
	SlotBinding AnchorSlot;
};

