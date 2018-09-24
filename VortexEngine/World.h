#pragma once
#include <vector>
#include "Object.h"
#include "Actor.h"
#include "StaticMeshActor.h" 
#include "SkeletalMeshActor.h"
#include "Terrain.h"
#include "PointLight.h"

using namespace std;

class World
{
public:
	float	 GravityMultiplier;
	bool	 CreateStaticMeshActor(StaticMesh* Mesh);
//	int		 GetStaticMeshPolycount();
	int		 StaticMeshPolyCount;
	void   	 UpdateStaticMeshPolycount();
	void	 SelectActor(Actor* InActor,bool ClearSelection);
	void	 Shutdown();
	void	 DeleteSelectedActors();
	void	 SelectAll();
	void	 ClearSelectedActors();
	bool     DoesActorNameExists(string Name);
	vector<Actor*>	 GetSelectedActors();
	int		 GetSelectedActorsCount();
	Actor*	 GetSelectedActor(int ID);
	Actor*	 GetActor(int ID);
	World();
	~World();
	//vector<StaticMeshActor*> StaticMeshActors;
	vector<Actor*> VActors;
	void RecalculateStaticMeshArray();
private:
	vector<Actor*> SelectedActors;
	vector<SkeletalMeshActor*> SkeletalMeshActors;
	
	
};

