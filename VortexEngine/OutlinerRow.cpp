#include "stdafx.h"
#include "OutlinerRow.h"
#include "SystemClass.h"

OutlinerRow::OutlinerRow()
{

}
void OutlinerRow::HideActor()
{
	SystemClass::GetSystem()->HideActor(ID);

}
void OutlinerRow::SelectActor()
{

	SystemClass::GetSystem()->SelectActor(ID);

}

void OutlinerRow::OnMouseButtonDown()
{
	SelectActor();
}
OutlinerRow::~OutlinerRow()
{
}

void OutlinerRow::OnWheelDown()
{
	OnWheelDownDelegate.Execute();
}

void OutlinerRow::OnWheelUp()
{
	OnWheelUpDelegate.Execute();
}