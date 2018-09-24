#include "stdafx.h"
#include "SaveMaster.h"
#include <string.h>

using namespace std;

SaveMaster::SaveMaster()
{
}

bool SaveMaster::SaveProject()
{
	ofstream SaveFile;
	
	SaveFile.open("/ProjectFile.vproject");
	SaveFile << "XXProject";
	SaveFile.close();

	return true;

}


SaveMaster::~SaveMaster()
{
}
