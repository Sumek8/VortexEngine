#include "stdafx.h"
#include "Composer.h"
#include "Button.h"


Composer::Composer()
{
	SeparatorSize  = 4;
	bIsEnabled = true;
	bIsVisible = false;
	bIsHorizontal = false;
	
}

void Composer::SetIsHorizontal(bool bIsHorizontalSplit)
{

	bIsHorizontal = bIsHorizontalSplit;

}

void Composer::UpdateChildren()
{

	
	if (ChildWidgets.size() == 0)
				return;
	
	NormalizeDistribution();
	int WidgetCount = (GetChildCount() + 1) / 2;

	for (size_t i = 0; i < WidgetCount; i++)
	{
		int ChildIndex = i * 2;
		int DeltaPosition = 0;
		int DeltaSize	  = 0;

		///////Separator Size  Correction 
		if (i == 0 || i == WidgetCount - 1)
		{
				DeltaSize = SeparatorSize / 2;
			if (i == WidgetCount - 1)
				DeltaPosition = SeparatorSize/2;
		}
		else
		{
			DeltaSize = SeparatorSize;
			DeltaPosition = SeparatorSize / 2;
		}
				///////////////
		



		float DistributionValue;
		DistributionValue = 0;
			if(i > 0)
			for (size_t j = 0; j < i; j++)
			{
				DistributionValue += (DistributionArray[j]);
			}
			if (bIsHorizontal)
			{
				ChildWidgets[ChildIndex]->SetSize((Size.x* DistributionArray[i]) - DeltaSize, Size.y);
				ChildWidgets[ChildIndex]->SetRelativeTransform((Size.x*DistributionValue)+DeltaPosition, 0);
			}
			else
			{
			ChildWidgets[ChildIndex]->SetSize(Size.x,(DistributionArray[i] * Size.y) - DeltaSize);
			ChildWidgets[ChildIndex]->SetRelativeTransform(0, (DistributionValue*Size.y) + DeltaPosition);
			
			}

		ChildWidgets[ChildIndex]->UpdateTransform();
		
		
	}
	UpdateSeparators();
	
	
}

/////Calculate distribution to 0-1 value
void Composer::NormalizeDistribution()
{
	float TotalValue;
	TotalValue = 0;
	for (size_t i = 0; i <DistributionArray.size(); i++)
	{
		TotalValue += DistributionArray[i];
	}

	for (size_t i = 0; i <DistributionArray.size(); i++)
	{
		DistributionArray[i] = DistributionArray[i]/TotalValue;
	}

}

void Composer::UpdateSeparators()
{
	

	int SeparatorCount = (GetChildCount() - 1) / 2;

	for (size_t i = 0; i < SeparatorCount; i++)
	{
		//Get every 2nd widget//
		int ChildIndex = (i * 2) + 1;

		float DistributionOffset = 0;

			for (size_t j = 0; j < i+1; j++)
			{
				DistributionOffset += (DistributionArray[j]);
			}
		if (!bIsHorizontal)
		{
			ChildWidgets[ChildIndex]->SetRelativeTransform(0, DistributionOffset*Size.y - SeparatorSize / 2);
			ChildWidgets[ChildIndex]->SetSize(Size.x, SeparatorSize);
		}
		else
		{
			ChildWidgets[ChildIndex]->SetRelativeTransform(DistributionOffset*Size.x - SeparatorSize / 2, 0);
			ChildWidgets[ChildIndex]->SetSize(SeparatorSize, Size.y);
		}
		ChildWidgets[ChildIndex]->UpdateTransform();
		ChildWidgets[ChildIndex]->SetEnabled(bIsEnabled);
	}
}

void Composer::CreatePartitionWidget()
{
	int WidgetCount = GetChildCount();

	float WidgetSize = 6;

		Button* PartitionWidget;
		PartitionWidget = new Button;
		PartitionWidget->SetName("Separator");
		PartitionWidget->SetColor(0.4, 0.4, 0.4, 1);
		PartitionWidget->SetMouseOverEndColor(0.4, 0.4, 0.4, 1);
		PartitionWidget->SetMouseOverColor(0.8,0.8, 0.8, 1);
		PartitionWidget->SetDraggable(true);
		PartitionWidget->OnDragDelegate.Bind<Composer,&Composer::RecalculateWindow>(this);
		PartitionWidget->LockDragAxis(!bIsHorizontal, bIsHorizontal);
		float DistributionValue;
		DistributionValue = 0;
		
		AddSeparator(PartitionWidget);
}





void Composer::RecalculateWindow()
{
	

	//Get Active Separator//
	int SeparatorCount = (GetChildCount() - 1) / 2;
	int ActiveSeparatorID = 0;

	for (size_t i = 0; i < SeparatorCount; i++)
	{
		int SeparatorIndex = (i * 2) + 1;

		if (GetChildWidget(SeparatorIndex)->GetState() == Dragged)
		{
			ActiveSeparatorID = i;
			break;
		}
	}


	float Position = 0;


	

		int SeparatorIndex = (ActiveSeparatorID * 2) + 1;
		if( ActiveSeparatorID == 0)
			if (bIsHorizontal)
			{

				Position = GetChildWidget(SeparatorIndex)->GetRelativeTransform().x + SeparatorSize / 2;
	
				float Distribution = DistributionArray[ActiveSeparatorID] + DistributionArray[ActiveSeparatorID + 1];
				DistributionArray[ActiveSeparatorID] = Position / Size.x;
				DistributionArray[ActiveSeparatorID + 1] = Distribution - (Position / Size.x);
			}
			else
			{
				Position = GetChildWidget(SeparatorIndex)->GetRelativeTransform().y + SeparatorSize / 2;
				float Distribution = DistributionArray[ActiveSeparatorID] + DistributionArray[ActiveSeparatorID + 1];
				DistributionArray[ActiveSeparatorID] = Position / Size.y;
				DistributionArray[ActiveSeparatorID + 1] = Distribution - (Position / Size.y);
				int i = 0;
			}
		if(ActiveSeparatorID == SeparatorCount-1)

			if (bIsHorizontal)
			{

				Position = GetChildWidget(SeparatorIndex)->GetRelativeTransform().x + SeparatorSize / 2;

				float Distribution = DistributionArray[ActiveSeparatorID] + DistributionArray[ActiveSeparatorID + 1];
				DistributionArray[ActiveSeparatorID] = Position / Size.x -( 1 - Distribution);
				DistributionArray[ActiveSeparatorID + 1] = Distribution - (Position / Size.x) + (1 - Distribution);
			}
			else
			{
				Position = GetChildWidget(SeparatorIndex)->GetRelativeTransform().y + SeparatorSize / 2;
				float Distribution = DistributionArray[ActiveSeparatorID] + DistributionArray[ActiveSeparatorID + 1];
				DistributionArray[ActiveSeparatorID] = (Position / Size.y) - (1 - Distribution);
				DistributionArray[ActiveSeparatorID + 1] = Distribution - (Position / Size.y) + (1 - Distribution);
				int i = 0;
			}

	


}

void Composer::UpdateChildTransform()
{
	for (size_t i = 0; i < ChildWidgets.size(); i++)
	{
		ChildWidgets[i]->SetParentTransform(GetTransform().x, GetTransform().y);
		UpdateChildren();
		ChildWidgets[i]->UpdateChildTransform();
	}

}

void Composer::AddSeparator(Widget* ChildWidget)
{
	ChildWidget->SetParentTransform(Transform.x, Transform.y);
	ChildWidget->UpdateTransform();
	ChildWidgets.push_back(ChildWidget);
}

void Composer::AddChildWidget(Widget* Child)
{



	if (GetChildCount() > 0)
		CreatePartitionWidget();

	Widget::AddChildWidget(Child);
	

	

}

Composer::~Composer()
{
	DistributionArray.clear();

}
