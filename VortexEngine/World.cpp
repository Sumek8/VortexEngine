#include "stdafx.h"
#include "World.h"
#include "StaticMeshActor.h"

using namespace std;

World::World()
{
	GravityMultiplier = 9.81f;
}
void World::Shutdown()
{
	for (size_t i = 0; i <SkeletalMeshActors.size(); i++)
	{
		SkeletalMeshActors[i]->Shutdown();
		delete SkeletalMeshActors[i];
		SkeletalMeshActors[i] = 0;
	}
	SkeletalMeshActors.clear();

	for (size_t i = 0; i <VActors.size(); i++)
	{
		VActors[i]->Shutdown();
		delete VActors[i];
		VActors[i] = 0;
	}
	VActors.clear();
}



bool World::CreateStaticMeshActor(StaticMesh* Mesh)
{
	if (Mesh)
	{
		int NameID = 1;
		StaticMeshActor* StaticActor = new StaticMeshActor;
		StaticActor->Mesh = Mesh;
		StaticActor->SetPosition(0, 0, 0);
		StaticActor->CalculateMatrix();
		StaticActor->ID = int(VActors.size());
		for (size_t i = 0; i < VActors.size(); i++)
		{
		
				if (VActors[i]->GetName().find_first_of(Mesh->GetName()) != string::npos)
					NameID += 1;
		}
		if (NameID != 1)
		{
			string ID = to_string(NameID);
			StaticActor->SetName(Mesh->GetName() + ID);
		}
		else
		StaticActor->SetName(Mesh->GetName());
		VActors.push_back(StaticActor);
		return true;
	}
	else return false;

	
}


void World::UpdateStaticMeshPolycount()
{
	StaticMeshPolyCount = 0;
	if (VActors.size() > 0 )
		
	for (int i = 0; i < VActors.size(); i++)
		if (VActors[i]->bIsVisible && dynamic_cast<StaticMeshActor*>(VActors.at(i)))
		StaticMeshPolyCount += dynamic_cast<StaticMeshActor*>(VActors.at(i))->Mesh->GetPolycount();


}



void World::RecalculateStaticMeshArray()
{
	for (int i = 0; i < VActors.size(); i++)
	{
		VActors[i]->ID = i;
	}

}
void World::SelectAll()
{
	SelectedActors.clear();

	for (int i = 0; i < VActors.size(); i++)
	{
		SelectedActors.push_back(VActors[i]);
	}

	return;
}


void World::SelectActor(Actor* InActor, bool ClearSelection)
{
	if (ClearSelection)
		SelectedActors.clear();
	else
		if (SelectedActors.size() > 0)
		{
			for (size_t i = 0; i < SelectedActors.size(); i++)
				if (SelectedActors[i]->ID == InActor->ID)
					return;
		}
	SelectedActors.push_back(InActor);

	return;

}
int	 World::GetSelectedActorsCount()
{

	return SelectedActors.size();

}

Actor*	 World::GetSelectedActor(int ID)
{
	if (ID < SelectedActors.size())
	return SelectedActors[ID];
	else 
		return 0;
}
Actor* World::GetActor(int ID)
{
	return VActors[ID];

}

vector<Actor*> World::GetSelectedActors()
{
	if (SelectedActors.size() > 0)

	return SelectedActors;
	else return {};
}
void World::DeleteSelectedActors()
{
	vector<int>IDArray;
	int SelectionCount = int(SelectedActors.size());


	if (SelectionCount > 0)
	{
		if(SelectionCount == VActors.size())
		{
			VActors.clear();
		
		}
		else
		{

			IDArray.push_back(SelectedActors[0]->ID);
			for (size_t i = 1; i < SelectionCount; i++)
			{
				{
					//Shuffle
					int ID = SelectedActors[i]->ID;
					if (ID > IDArray[0])
						IDArray.insert(IDArray.begin(), ID);
					else
					{
						IDArray.push_back(ID);

					}

				}
			}
			for (size_t i = 0; i < IDArray.size(); i++)
				VActors.erase(VActors.begin() + IDArray[i]);

			OutputDebugStringA("ActorsDeleted");



			RecalculateStaticMeshArray();
			
		}
		for (size_t i = 0; i < SelectionCount; i++)
		{
			delete	SelectedActors[i];
			SelectedActors[i] = 0;
		}

		SelectedActors.clear();
		IDArray.clear();

		RecalculateStaticMeshArray();
		UpdateStaticMeshPolycount();
	}
}
void World::ClearSelectedActors()
{
	SelectedActors.clear();
}

bool World::DoesActorNameExists(string Name)
{
	for (int i = 0; i < VActors.size(); i++)
	{
		if (VActors[i]->GetName() == Name)
			return true;

	}
	return false;
}

World::~World()
{
}
