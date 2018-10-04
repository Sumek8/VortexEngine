#include "stdafx.h"
#include "DetailPanel.h"
#include "Text.h"
#include "ListBox.h"
#include "InputBox.h"
#include "Composer.h"



DetailPanel::DetailPanel()
{

	Size.x = 120;
	Size.y = 600;
	SetName("Details");

	/*Composer* WidgetComposer = new Composer;
	WidgetComposer->SetIsHorizontal(false);
	AddChildWidget(WidgetComposer);
	*/
	ListBox* List = new ListBox;
	List->SetSize(Size.x,Size.y);
	AddChildWidget(List);

	
		Widget* TransformBox;
		TransformBox = new Widget;
		TransformBox->SetName("TransformBox");
		TransformBox->SetSize(Size.x,60);
		List->AddChildWidget(TransformBox);

		
		Text* TransformText = new Text;
		TransformText->SetSize(Size.x, 10);
		TransformText->SetText("Transform");
		TransformBox->AddChildWidget(TransformText);

		
		

			Text* LocationText = new Text;
			LocationText->SetSize(Size.x, 10);
			LocationText->SetRelativeTransform(10.0f,10);
			LocationText->SetText("Location");
			TransformBox->AddChildWidget(LocationText);

			Text* RotationText = new Text;
			RotationText->SetSize(Size.x, 10);
			RotationText->SetRelativeTransform(10.0f, 20.0f);
			RotationText->SetText("Rotation");
			TransformBox->AddChildWidget(RotationText);

			Widget* ScaleBox = new Widget;
			ScaleBox->SetSize(Size.x, 10);
			ScaleBox->SetRelativeTransform(10.0f, 30.0f);
			TransformBox->AddChildWidget(ScaleBox);

			Text* ScaleText = new Text;
			ScaleText->SetSize(Size.x, 10);
			ScaleText->SetText("Scale");
			ScaleBox->AddChildWidget(ScaleText);

				InputBox* ScaleValueX = new InputBox;
				ScaleValueX->SetSize(Size.x, 10);
				ScaleValueX->SetRelativeTransform(70.0f,0);
				ScaleValueX->SetText("0");
				ScaleValueX->SetName("X");
				ScaleBox->AddChildWidget(ScaleValueX);

				InputBox* ScaleValueY = new InputBox;
				ScaleValueY->SetSize(Size.x, 10);
				ScaleValueY->SetRelativeTransform(80.0f, 0);
				ScaleValueY->SetText("0");
				ScaleBox->AddChildWidget(ScaleValueY);

				InputBox* ScaleValueZ = new InputBox;
				ScaleValueZ->SetSize(Size.x, 10);
				ScaleValueZ->SetRelativeTransform(90.0f, 0);
				ScaleValueZ->SetText("0");
				ScaleBox->AddChildWidget(ScaleValueZ);
				
			//	WidgetComposer->UpdateChildTransform();


}


void DetailPanel::Update()
{

	

}

DetailPanel::~DetailPanel()
{

	


}
