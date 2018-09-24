#include "stdafx.h"
#include "MaterialWidget.h"
#include "Button.h"
#include "MaterialWidgetSlot.h"
#include "MaterialMaster.h"


MaterialWidget::MaterialWidget()
{
	hasNodeView = true;
	
}
void MaterialWidget::Initialize()
{
	
	CreateWidget();


}

void MaterialWidget::CreateWidget()
{
	int ViewSize = 40;
	int NodeSize = 10;
	
	for (int i = 0; i < InputSlots; i++)
	{
		char Id[30];
		sprintf_s(Id, "%d", i);

		MaterialWidgetSlot* VNodeInput;
		VNodeInput = new MaterialWidgetSlot;
		VNodeInput->VWidgetName = (this->GetName() + "InputSlot" + Id);
		VNodeInput->SetSize(float(NodeSize), float(NodeSize));
		VNodeInput->SetColor(0.7f, 0.7f, 0.7f, 1.0f);
		VNodeInput->SetRelativeTransform(5,Size.y / (InputSlots)*i + (Size.y / (InputSlots)- VNodeInput->GetSize().y) / 2);
		VNodeInput->SetParentTransform(Transform.x, Transform.y);
		VNodeInput->UpdateTransform();
		VNodeInput->OnLeftMouseButtonDownDelegate.Bind<MaterialWidget, &MaterialWidget::SetBindingTarget>(this);
		VNodeInput->isInputSlot = true;
		VNodeInput->ID = i;


		AddChildWidget(VNodeInput);
	}

	for (int i = 0; i < OutputSlots; i++)
	{
		char Id[30];
		sprintf_s(Id, "%d", i);
		MaterialWidgetSlot* VNodeInput;
		VNodeInput = new MaterialWidgetSlot;
		VNodeInput->VWidgetName = (this->GetName() + "OutputSlot" + Id);
		VNodeInput->SetSize(float(NodeSize), float(NodeSize));
		VNodeInput->SetColor(0.7f, 0.7f, 0.7f, 1.0f);
		VNodeInput->SetRelativeTransform(Size.x - 5 - VNodeInput->GetSize().x , Size.y/(OutputSlots)*i+ (Size.y / (OutputSlots) - VNodeInput->GetSize().y)/2);
		VNodeInput->SetParentTransform(Transform.x, Transform.y);
		VNodeInput->OnLeftMouseButtonDownDelegate.Bind<MaterialWidget, &MaterialWidget::SetBindingSource>(this);
		VNodeInput->UpdateTransform();
		VNodeInput->ID = i;
		AddChildWidget(VNodeInput);
	}

	if (hasNodeView)
	{
		Widget* VNodeInput;
		VNodeInput = new Widget;
		VNodeInput->VWidgetName = (this->GetName() + "NodeView");
		VNodeInput->SetSize(Size.x - ViewSize, Size.y - ViewSize);
		VNodeInput->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
		VNodeInput->SetRelativeTransform(float(ViewSize / 2), float(ViewSize / 2));
		VNodeInput->SetParentTransform(Transform.x, Transform.y);
		VNodeInput->UpdateTransform();
		VNodeInput->SetEnabled(false);
		AddChildWidget(VNodeInput);
	}

}
void MaterialWidget::SetBindingTarget(int ID)
{
	
	Master->SetBindingAnchor(this,ID,false);

}

void MaterialWidget::SetBindingSource(int ID)
{
	
	Master->SetBindingAnchor(this, ID,true);

}

void 	MaterialWidget::OnMouseButtonDown()
{


}
void 	MaterialWidget::OnMouseButtonUp()
{
}
void   MaterialWidget::OnMouseOver()

{
}
void	MaterialWidget::OnMouseOverEnd()
{

}

void MaterialWidget::StartDrag()
{

}

void MaterialWidget::EndDrag()
{
	

}

MaterialWidget::~MaterialWidget()
{
}

