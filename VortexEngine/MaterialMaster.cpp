#include "stdafx.h"
#include "MaterialMaster.h"



MaterialMaster::MaterialMaster()
{
	MaterialPath = "Shaders/Material.fx";
	TextureSampleCount = 0;
	
	IsBinding = false;
}

void MaterialMaster::Initialize(WidgetManager* newVWidgetManager,Input* newVInput)
{
	
		VInput = newVInput;
		VWidgetManager = newVWidgetManager;

		
		WidgetNodeContainer = VWidgetManager->GetWidgetContainer(VWidgetManager->WidgetContainers.size() - 1);
		WidgetNodeGraph = VWidgetManager->GetWidgetByName("MaterialGraph");
		LoadNodes();
//	CompileShader();

		WidgetNodeContainer->CalculateContainer();
		WidgetNodeGraph->UpdateChildTransform();
	return;
}

void MaterialMaster::LoadNodes()
{

	

	AddConstantVector();
	AddConstant2Vector();
	AddConstant3Vector();

	SampleTexture(0);
	

	WidgetNodeGraph->AddChildWidget(CreateMaterialOutputNode());
	CreateWidgets();
	
}

Widget* MaterialMaster::CreateMaterialOutputNode()
{
	


	MaterialWidget* MatWidget;
	MatWidget = new MaterialWidget;
	MatWidget->SetColor(0.5, 0.5, 0.5, 0.5);
	MatWidget->SetName("MaterialOutputWidget");
	MatWidget->InputSlots = 10;
	MatWidget->OutputSlots = 0;
	MatWidget->SetDraggable(true);
	MatWidget->SetSize(150,400);
	MatWidget->SetRelativeTransform(100,100);
	MatWidget->UpdateTransform();
	MatWidget->Initialize();
	MatWidget->Master = this;

	return MatWidget;
}

Widget*	MaterialMaster::CreateMaterialWidget(int ID)
{
	char buffer[10];
	sprintf_s(buffer, "%d", ID);
	
	MaterialWidget* MatWidget;
	MatWidget = new MaterialWidget;
	MatWidget->SetColor(0.5, 0.5, 0.5, 0.5);
	MatWidget->SetName("MaterialWidget" + ID);
	MatWidget->InputSlots = MaterialWidgets[ID]->InputSlots;
	MatWidget->OutputSlots = MaterialWidgets[ID]->OutputSlots;
	MatWidget->SetDraggable(true);
	MatWidget->SetSize(100,100);
	MatWidget->SetRelativeTransform(0,0);
	MatWidget->UpdateTransform();
	MatWidget->Initialize();
	MatWidget->Master = this;
	for (size_t i = 0; i < MatWidget->InputSlots; i++)
	{
		SlotBinding Binding;
		
		MatWidget->SlotBindings.push_back(Binding);
	}
	

	return MatWidget;

}

void MaterialMaster::ClearBinding(MaterialWidget* Node, int SlotID, bool isInputSlot)
{
	vector<BindLink>NewBinding;

	if (isInputSlot)
	{
		
		for (size_t i = 0; i < BindingArray.size(); i++)
		{
			if (!(BindingArray[i].TargetNode == Node && BindingArray[i].TargetNodeSlot == SlotID))
			
					NewBinding.push_back(BindingArray[i]);
		}
		
	}
	else
	{
		for (size_t i = 0; i < BindingArray.size(); i++)
		{
			if (!(BindingArray[i].SourceNode == Node && BindingArray[i].SourceNodeSlot == SlotID))
				NewBinding.push_back(BindingArray[i]);

		}
	}
	
	BindingArray.clear();
	BindingArray = NewBinding;

	for (size_t j = 0; j < WidgetNodeContainer->LineList.size(); j++)
	{
		delete	WidgetNodeContainer->LineList[j];

	}
	WidgetNodeContainer->LineList.clear();

	CreateLinks();
	return;

}


void MaterialMaster::CreateLinks()
{
	
	for (int i = 0; i < BindingArray.size(); i++)
	{
		Line* Link;
		Link = new Line;
		Link->Color = VColor(1, 1, 1, 1);
		BindingArray[i].Link = Link;
		WidgetNodeContainer->LineList.push_back(Link);
	}
	int count = int(WidgetNodeContainer->LineList.size());
	int count2 = int(BindingArray.size());

	UpdateLinks();
	
}

void MaterialMaster::SetBindingAnchor(MaterialWidget* Node,int SlotID,bool isInputSlot)
{
	if (VInput->IsKeyDown(17))
		{
		ClearBinding(Node, SlotID, isInputSlot);
		return;
		}

	if (!IsBinding)
	{
		if (!isInputSlot)
		{
			AnchorSlot.TargetWidgetID = 0;
			AnchorSlot.TargetWidgetSlotID = SlotID;



		Binding.SourceNode = Node;
		Binding.SourceNodeSlot = SlotID;
		Binding.Link = 0;
		Binding.SourceNode = 0;
		Binding.SourceNodeSlot = 0;
		}
		else
		{
			AnchorSlot.TargetWidgetID = 0;
			AnchorSlot.TargetWidgetSlotID = SlotID;


			Binding.TargetNode = Node;
			Binding.TargetNodeSlot = SlotID;
		}
		Line* Link;
		Link = new Line;

		Link->Points[0] = VVector(Node->GetChildren()[SlotID]->GetTransform().x + Node->GetChildren()[SlotID]->GetSize().x / 2, Node->GetChildren()[SlotID]->GetTransform().y + Node->GetChildren()[SlotID]->GetSize().y / 2, 0);
		Link->Points[1] = VVector(0, 0, 0);
		Link->Color = VColor(1, 1, 1, 1);
		ActiveLink = Link;
		WidgetNodeContainer->AddLine(Link);

		IsBinding = true;
		
	}
	else
	{	
		if (Binding.SourceNode == Node || Binding.TargetNode == Node)
			{
			EndBinding();
			return;
			}
		if (isInputSlot && Binding.SourceNode != 0 && Binding.SourceNode != Node)
		{
			IsBinding = false;
			Binding.TargetNode = Node;
			Binding.TargetNodeSlot = SlotID;
			BindNodes();
			ActiveLink = 0;
		}
		if (!isInputSlot  && Binding.TargetNode != Node  && Binding.TargetNode != Node)
		{
			Binding.SourceNode = Node;
			Binding.SourceNodeSlot = SlotID;

			IsBinding = false;
			BindNodes();
			ActiveLink = 0;
		}
	}
	
	
	
	return;
}

void MaterialMaster::ReplaceLinks()
{
	
}

void MaterialMaster::EndBinding()
{
	if (IsBinding)
	{
		
	if (WidgetNodeContainer->LineList.size()>0)
		delete WidgetNodeContainer->LineList[WidgetNodeContainer->GetLineList().size()-1];
		WidgetNodeContainer->LineList.resize(WidgetNodeContainer->GetLineList().size() - 1);

	}

	ActiveLink = 0;
	IsBinding = false;
	UpdateLinks();
	
}

void MaterialMaster::BindNodes()
{
	
	 Binding.Link = ActiveLink;

	 BindingArray.push_back(Binding);
	 ActiveLink = 0;
	 Binding.SourceNode = 0;
	 Binding.TargetNode = 0;
	 UpdateLinks();
	// CompileShader();
	
}



void MaterialMaster::UpdateLinks()
{
	
	for (size_t i = 0; i < BindingArray.size(); i++)
	{

		Widget* SourceNode = BindingArray[i].SourceNode->GetChildren()[BindingArray[i].SourceNodeSlot];
		Widget* TargetNode = BindingArray[i].TargetNode->GetChildren()[BindingArray[i].TargetNodeSlot];

		BindingArray[i].Link->Points[0] = VVector(SourceNode->GetTransform().x + SourceNode->GetSize().x/2, SourceNode->GetTransform().y + SourceNode->GetSize().y/2, 0);
		BindingArray[i].Link->Points[1] = VVector(TargetNode->GetTransform().x + TargetNode->GetSize().x/2, TargetNode->GetTransform().y + TargetNode->GetSize().y/2, 0);


	}
	

}


bool MaterialMaster::CompileShader()
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	ofstream ShaderFile;

	


	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	ShaderFile.open(MaterialPath);

	CreateConstantBuffers();
	CreatePixelInput();
	AddSampler();
	for (int i = 0; i < 2; i++)
	{
		AddTextureSampler(i);
	}


	BeginShader();
		
	for (int i = 0; i < TextureSampleCount; i++)
	{
		SampleTexture(i);
	}
	


	

	//Add(MaterialWidgets[0], MaterialWidgets[1]);
	//Divide(MaterialWidgets[0], MaterialWidgets[1]);
	//Multiply(MaterialWidgets[0], MaterialWidgets[1]);
	//Subtract(MaterialWidgets[0], MaterialWidgets[1]);
	
	//SetNormalOutput(MaterialWidgets[1]);
	//SetBaseColorOutput(MaterialWidgets[0]);
	FinalizeShader();
	ShaderFile << CodeArray.c_str();


	
	ShaderFile.close();

	// Release the error message.
	if (errorMessage)
	{
		errorMessage->Release();
		errorMessage = 0;
	}
	OutputDebugStringA("Compiled");

	/*
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "VertexShaderFunction", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		return false;
	}
	// Set 0 or 1 for True or false
	
	*/
	
	
	char* bColorMap = "1";

	D3D_SHADER_MACRO Shader_Macros[] = {
		"BASECOLORMAP",bColorMap,
		"EMISSION","0",
		"NORMALMAP","1",

		nullptr, nullptr };

	// Compile the pixel shader code.

	size_t newsize = strlen(MaterialPath.c_str()) + 1;
	wchar_t * FilePath = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars,FilePath, newsize, MaterialPath.c_str(), _TRUNCATE);


	result = D3DCompileFromFile(FilePath, Shader_Macros, NULL, "PixelShaderFunction", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
		return false;

	if (pixelShaderBuffer)
		pixelShaderBuffer->Release();
	if (vertexShaderBuffer)
		vertexShaderBuffer->Release();
		
	return true;

}


void MaterialMaster::CreateWidgets()
{
	for (int i = 0; i < MaterialWidgets.size(); i++)
		WidgetNodeGraph->AddChildWidget(CreateMaterialWidget(i));
		

	return;
}

MaterialMaster::~MaterialMaster()
{
}

void MaterialMaster::AddSampler()
{
	string Sampler = "SamplerState SampleType : register(s0); \n";
	CodeArray.append(Sampler);
}


void MaterialMaster::SetNormalOutput(MaterialWidget Node)
{
	
		string NormalOutput = "";
		NormalOutput.append(Node.GetName() + "= (" + Node.GetName() + " * 2.0f) - 1.0f;	\n");
		NormalOutput.append("float3 Normal = ("+ Node.GetName() + ".x * input.tangent) + (" + Node.GetName() +".y * input.binormal) + (" + Node.GetName() +".z * input.normal); \n");
		NormalOutput.append("Normal = normalize(Normal);\n Out.Normal.xyz = Normal;\n");
	


	
	CodeArray.append(NormalOutput);
	
}


void MaterialMaster::Math(MaterialWidget Node1, MaterialWidget Node2,string Expression)
{
	MaterialWidget Node;
	string Equation;
	Equation = "";
	int ID = int(MaterialWidgets.size());
	char Id[30];
	sprintf_s(Id, "%d", ID);
	string Name;



	/*
	if (Node1.Type == Node2.Type || Node2.Type == Float)

	{
		switch (Node2.Type)
		{

		case Float:     Name = ("float Expression" + string(Id));

		case Float2:	Name = ("float2 Expression" + string(Id));

		case Float3:	Name = ("float3 Expression" + string(Id));

		case Float4:	Name = ("float4 Expression" + string(Id));

		}
		Equation.append(Name + "=" + Node1.GetName() + Expression + Node2.GetName() + "; \n");
		Node.GetName() = Name;
		//Node.Type = Node1.Type;
		Node.InputSlots = 2;
		Node.OutputSlots = 1;
		MaterialWidgets.push_back(Node);
		CodeArray.append(Equation);
		

	}
	*/

}
void MaterialMaster::Add(MaterialWidget Node1, MaterialWidget Node2)
{
	Math(Node1, Node2, "+");
}
void MaterialMaster::Subtract(MaterialWidget Node1, MaterialWidget Node2)
{
	
	Math(Node1, Node2, "-");

};

void MaterialMaster::Multiply(MaterialWidget Node1, MaterialWidget Node2)
{
	Math(Node1, Node2, "*");

}
void MaterialMaster::Divide(MaterialWidget Node1, MaterialWidget Node2)
{
	Math(Node1, Node2, "/");

}
void MaterialMaster::Time()
{}
void MaterialMaster::Dot(MaterialWidget Node1, MaterialWidget Node2)

{

	MaterialWidget Node;
	string Equation;
	Equation = "";
	int ID = int(MaterialWidgets.size());
	char Id[30];
	sprintf_s(Id, "%d", ID);
	string Name;


	/*

	if (Node1.Type == Node2.Type || Node2.Type == Float)

	{
		switch (Node2.Type)
		{

		case Float:     Name = ("float Expression" + string(Id));

		case Float2:	Name = ("float2 Expression" + string(Id));

		case Float3:	Name = ("float3 Expression" + string(Id));

		case Float4:	Name = ("float4 Expression" + string(Id));

		}
		Equation.append(Name + "=" + Node1.GetName() +".x" +"*" + Node2.GetName() +".x" + "+" + Node1.GetName() +".y" +"*" + Node2.GetName() +".y" + "+" + Node1.GetName() + ".z" + "*" + Node2.GetName() + ".z; \n");
		Node.GetName() = Name;
		Node.Type = Node1.Type;
		MaterialWidgets.push_back(Node);
		CodeArray.append(Equation);


	}
	*/
}

void MaterialMaster::SetBaseColorOutput()
{

	for (int i = 0; i < BindingArray.size(); i++)
		if (BindingArray[i].TargetNode->GetName() == MaterialWidgets[0]->GetName() && BindingArray[i].TargetNodeSlot == 0)
		{
			//if (Node.Type == Float4)
			//{
			string ColorOutput =
				"\n Out.Color = ";
			ColorOutput.append(BindingArray[i].TargetNode->GetName() + ";\n");

			CodeArray.append(ColorOutput);
			//}
		}
	
}
void MaterialMaster::AddTextureSampler(int ID)
{
	
	
	char Id[30];
	sprintf_s(Id, "%d", ID);

	string TextSample = "\n Texture2D TextureSample";
	TextSample.append(string(Id) + ": register(t" + Id + "); \n");

	CodeArray.append(TextSample);
	TextureSampleCount += 1;

}


void  MaterialMaster::CreateConstantBuffers()
{
	string Buffer = "\n\
cbuffer ConstantBufferType \n\
{							\n\
float3 LightDirection; \n\
float4 DiffuseColor; \n\
float padding;  \n\
}; \n";
	CodeArray.append(Buffer);
}


void MaterialMaster::AddConstant3Vector()
{

	int ID = int(MaterialWidgets.size());
	char Id[30];
	sprintf_s(Id, "%d", ID);
	string End = " = float3(0,0,0);\n";
	
	MaterialWidget* Node;
	//Node.Type = Float3;
	Node = new MaterialWidget;
	Node->SetName(string("Constant3Float") + Id);
	Node->InputSlots = 0;
	Node->OutputSlots = 3;
	MaterialWidgets.push_back(Node);

	string Constant = "float3 Constant3Float";
	Constant.append(Id + End);

	CodeArray.append(Constant);
	
}
void MaterialMaster::GetChannelValue(int Channel)
{



}
void MaterialMaster::AddConstant2Vector()
{

	int ID = int(MaterialWidgets.size());
	char Id[30];
	sprintf_s(Id, "%d", ID);
	string End = " = float2(0,0);\n";

	MaterialWidget* Node;
	//Node.Type = Float2;
	Node = new MaterialWidget;
	Node->InputSlots = 0;
	Node->OutputSlots = 2;
	Node->SetName(string("Constant2Float") + Id);
	MaterialWidgets.push_back(Node);



	string Constant = "float2 Constant2Float";
	Constant.append(Id + End);

	CodeArray.append(Constant);

}
void MaterialMaster::SampleTexture(int ID)
{
	

	char Id[30];
	sprintf_s(Id, "%d", ID);
	
	MaterialWidget* Node;
	Node = new MaterialWidget;
	Node->OutputSlots = 5;
	Node->SetName((string("Texture") + Id));
	
	MaterialWidgets.push_back(Node);

	string Texture = "float4 Texture";
	Texture.append(string(Id) + "= TextureSample" + Id + ".Sample(SampleType, input.uv); \n");

	CodeArray.append(Texture);

}
void MaterialMaster::AddConstantVector()
{
	

	int ID = int(MaterialWidgets.size());
	char Id[30];
	sprintf_s(Id, "%d", ID);


	MaterialWidget* Node;
	Node = new MaterialWidget;
	Node->SetName("MaterialWidgetConstant" + ID);
	//Node.Type = Float;
	Node->InputSlots = 0;
	Node->OutputSlots = 1;

	MaterialWidgets.push_back(Node);




	string End = " = 0;\n";

	string Constant = "float  ConstantFloat";
	Constant.append(Id);
	Constant.append(End);

	CodeArray.append(Constant);
	
}

void MaterialMaster::FinalizeShader()
{
	string End ="\n\
Out.Position.xyz = input.viewDirection;\n\
Out.Depth = input.position.z / input.position.w;\n\
return Out;										\n\
				  }								\n";
				
	CodeArray.append(End);

}
void MaterialMaster::BeginShader()
{
	string Begin;
	Begin = "\n \
PixelOutputType   PixelShaderFunction(PixelInputType input){ \n\
PixelOutputType Out;						\n\
Out.Normal = float4(0.5, 0.5, 1, 1);		 \n\
Out.Color = float4(0, 0, 0, 1);				\n\
Out.Position = float4(0, 0, 0, 1);			\n\
\n";


	CodeArray.append(Begin);
}
void MaterialMaster::CreatePixelInput()
{
	string PixelStructs;

PixelStructs = "\n struct PixelInputType	\n\
{									\n\
float4 position : SV_POSITION;		\n\
float2 uv : TEXCOORD;				\n\
float3 normal : TEXCOORD1;			\n\
float3 tangent : TEXCOORD2;			\n\
float3 binormal : TEXCOORD3;		\n\
float3 viewDirection : TEXCOORD4;   \n\
}; \n \
struct PixelOutputType			    \n\
{									\n\
	float4 Color : SV_Target0;	    \n\
	float4 Normal : SV_Target1;		\n\
	float4 Position : SV_Target2;	\n\
	float4 Depth : SV_Target3;		\n\
};\n"
;


CodeArray.append(PixelStructs);
}

int MaterialMaster::GetNodeByName(string Name)
{
	
	for (int i = 0; i < MaterialWidgets.size(); i++)
	{
		if (MaterialWidgets[i]->GetName() == Name)
			return i;
	}
	return 0;

	
}

void MaterialMaster::Shutdown()
{
	
	for (int i = 0; i < MaterialWidgets.size(); i++)
	{
		MaterialWidgets[i]->SlotBindings.clear();
		MaterialWidgets[i]->Shutdown();
		 delete MaterialWidgets[i];
	}
	MaterialWidgets.clear();
	CodeArray.clear();
	MaterialPath.clear();
	DestroyWindow(VMaterialHwnd);
	VMaterialHwnd = NULL;

}