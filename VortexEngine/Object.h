#pragma once
#include <string>
#include "Math.h"

using namespace std;

class Object
{
public:
	Object();
	~Object();
	string GetName();
	void   SetName(string Name);
	int ID;
	virtual void Shutdown();
protected:
	
	string Name;
};

