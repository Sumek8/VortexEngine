#include "stdafx.h"
#include "DetailPanel.h"
#include "Text.h"
#include "ListBox.h"





DetailPanel::DetailPanel()
{

	Size.x = 120;
	
	SetName("Details");

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

				/*Text* ScaleValueX = new Text;
				ScaleValueX->SetSize(Size.x, 10);
				ScaleValueX->SetRelativeTransform(70.0f,0);
				ScaleValueX->SetText("0");
				ScaleBox->AddChildWidget(ScaleValueX);

				Text* ScaleValueY = new Text;
				ScaleValueY->SetSize(Size.x, 10);
				ScaleValueY->SetRelativeTransform(80.0f, 0);
				ScaleValueY->SetText("0");
				ScaleBox->AddChildWidget(ScaleValueY);

				Text* ScaleValueZ = new Text;
				ScaleValueZ->SetSize(Size.x, 10);
				ScaleValueZ->SetRelativeTransform(90.0f, 0);
				ScaleValueZ->SetText("0");
				ScaleBox->AddChildWidget(ScaleValueZ);
				*/


}


void DetailPanel::Update()
{

	

}

DetailPanel::~DetailPanel()
{

	


}
