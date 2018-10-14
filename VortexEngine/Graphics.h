#pragma once

//#define DIRECTX = 1
#define WINDOWS = 1


#ifdef WINDOWS
#include <windows.h>
#endif  WINDOWS




#include "Math.h"

#include "DirectionalLight.h"
#include "Cameraclass.h"
#include "PBRShaderClass.h"
#include "DepthShader.h"
#include "Model.h"
#include "Texture.h"
#include "WidgetShader.h"
#include "LineShader.h"
#include "Widget.h"
#include "Font.h"
#include "Text.h"
#include "Viewport.h"


#ifdef DIRECTX
#include <d3d11.h>
#include "D3DClass.h"
#endif 




/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN =  false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class ResourceManager;
class World;
class WidgetManager;
class MaterialMaster;

struct FontDesc
{
	char Letter;
	int Width;
	int Height;
	int	X;
	int Y;
		
};

class Graphics
{
public:
	Graphics();
	~Graphics();
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	
	void SetLightRotation(VRotation Rotation);
	void SetLightPosition(VVector Position);
	void GetLightRotation(VRotation Rotation);
	void UpdatePolycount();


	bool LoadTextureFromFile(WCHAR* filename, ID3D11Resource** out);
	bool LoadShaderResource(WCHAR* filename, ID3D11ShaderResourceView** out);
	void UpdateFPS();
	void UpdateDrawCallCount();
	void RenderBuffers(ID3D11DeviceContext* deviceContext,ID3D11Buffer* vertexBuffer, ID3D11Buffer* IndexBuffer);
	void SetDirectionalLight(DirectionalLight * Light);
	void RenderWidgetBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer);
	void ToggleWireframe();



	void SetResourceManager(ResourceManager* SrcManager);
	void SetWidgetManager(WidgetManager* VWidgetManager);
	void RenderLines(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer);
	void UpdateInterfaceBuffers(int WindowID);
	bool CreateTextBuffers(int WindowID);
	bool CreateChunkBuffers(Model* VModel);
	void RenderInstancedStaticMeshes(ID3D11DeviceContext* deviceContext, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer);
	bool CreateWidgetBuffer(int WindowID);
	bool CreateViewportBuffer(int WindowID);
	bool CreateLineListBuffer(int WindowID);
	

	PBRShaderClass* GetShader();

	
	D3DClass* GetD3DClass();

	void ToggleDrawInterface();

	ID3D11ShaderResourceView* BaseColorMap;
	ID3D11ShaderResourceView* NormalMap;
	ID3D11ShaderResourceView* CubeMap;
	ID3D11ShaderResourceView* RoughnessMetalness;
	ID3D11ShaderResourceView* FontMap;

	ID3D11ShaderResourceView* GizmoMap;

	CameraClass* VCamera;

	float			  GizmoScale;

private:
			  
	bool			  Render();
	bool			  bApplyPostProcess;
	bool			  bRenderWireframe;

	int				  DrawCallCount;

	D3DClass*		  VDirect3D;
	
	DirectionalLight* VDirectionalLight;
	
	PBRShaderClass*   VShaderClass;
	LineShader*       VLineShader;
	WidgetShader*     VWidgetShader;
	DepthShader*      VDepthShader;
	ResourceManager*  VResourceManager;
	WidgetManager*	  VWidgetManager;
				 

	ID3D11Buffer*	 WidgetVertexBuffer;
	ID3D11Buffer*	 WidgetIndexBuffer;

	ID3D11Buffer*	 TextVertexBuffer;
	ID3D11Buffer*	 TextIndexBuffer;

	ID3D11Buffer*	 LineVertexBuffer;
	ID3D11Buffer*	 LineIndexBuffer;

	int				 screenWidth,screenHeight;

	bool			 bDrawInterface = true;
public:
	MaterialMaster*	 VMaterialMaster;
};

