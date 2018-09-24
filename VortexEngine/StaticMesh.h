#pragma once

#include <vector>
#include "Object.h"
#include "Material.h"
#include "Model.h"
#include "Math.h"

using namespace DirectX;

using namespace std;




class StaticMesh :public  Object
{

public:
		StaticMesh();
		~StaticMesh();


public:
		
		int GetPolycount();
		void SetModel(Model* srcModel);
		void Shutdown();
		Model* GetModel();
		void CalculateSphereBounds();
		float GetSphereBoundsRadius();
		string GetName();
		
private:
		string	Name;
		int					polygonCount;
		int					vertexCount;
		int					edgeCount;
		char*				ModelPath;
		Model*				VModel;
		vector<Material*>	Materials;
		float				BoundSphereRadius;
	
};