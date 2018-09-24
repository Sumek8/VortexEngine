#include "stdafx.h"
#include "Object.h"


Object::Object()
{
}

string Object::GetName()
{
	return Name;
}

void   Object::SetName(string NewName)
{

	Name = NewName;

}

void Object::Shutdown()
{
}

Object::~Object()
{
}

