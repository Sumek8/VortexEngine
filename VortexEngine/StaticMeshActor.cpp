#include "stdafx.h"
#include "StaticMeshActor.h"




StaticMeshActor::StaticMeshActor()
{
	Scale.x = 1;
	Scale.y = 1;
	Scale.z = 1;
	bCastDynamicShadows = true;
	bIsVisible = true;
}



void StaticMeshActor::Destroy()
{


}


StaticMeshActor::~StaticMeshActor()
{
}

void StaticMeshActor::Shutdown()
{
	Mesh = 0;

}