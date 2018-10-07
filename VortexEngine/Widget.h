#pragma once
#include <vector>
#include <string>
#include "Object.h"
#include "Delegate.h"
#include "Math.h"

using namespace std;


class SystemClass;

class Graphics;


enum WidgetType
{

	BaseWidget,
	ButtonWidget,
	TextWidget,
	ImageWidget,
	ViewportWidget,
	BorderWidget,
};

enum WidgetState
{
	InActive,
	Dragged,
	Selected,
	Disabled,
	MouseOver,
	Hidden,
};






struct WidgetSize
{
	float x;
	float y;
	
};



class Widget:
	public	Object
{
public:
				     Widget();
	virtual			 ~Widget();
	virtual void 	 OnDrag();
	virtual void	 OnMouseButtonDown();
	virtual void	 OnMouseOver();
	virtual void     OnMouseOverEnd();
	virtual void     OnMouseButtonUp();
	virtual void	 OnWheelUp();
	virtual	void	 OnWheelDown();
	virtual void     Initialize();
	virtual void	 OnLeftButtonDoubleClick();
	virtual void     OnRightButtonDown();
	virtual void	 Shutdown();
	virtual void     SetColor(VColor Color);
	virtual void	 SetColor(float r,float g, float b, float a);
	virtual void	 SetSize(float newSizeX, float newSizeY);
	void		     UpdateTransform();
	void			 SetState(WidgetState State);
	void		     SetName(string name);
	void			 SetRelativeTransform(float x,float y);
	void		     SetZOrder(int Order);
	void			 StartDrag();
	void			 EndDrag();
	void			 AddChildWidget(Widget* ChildWidget);

	void			 RemoveChild(int ID);
	void			 RemoveChild();

	int				 GetChildCount();
	vector<Widget*>  GetChildren();
	vector<Widget*>  ChildWidgets;
	WidgetState		 GetState();
	Widget*		     GetChildWidget(int ID);
	WidgetType		 GetType();
	void			 SetParentTransform(float x, float y);
	void			 SetTransform(float x, float y);
	void			 SetIsClipping(bool isClipping);
	void			 SetInheritSize(bool InheritsSize);
	bool		 	 GetIsVisible();
	bool			 GetIsDraggable();
	bool             GetEnabled();
	bool		     GetIsClipping();
	bool			 GetInheritSize();
	int				 GetZOrder();
	VColor		     GetOutlineColor();
	VColor		     GetColor();
	VVector2		 GetRelativeTransform();
	VVector2	     GetSize();
	VVector2		 GetTransform();
	
	string			 GetName();
	string			 VWidgetName;

	

	bool			 GetIsDragXLocked();
	bool			 GetIsDragYLocked();
	void			 Destroy();
	virtual void	 UpdateChildTransform();
	void		     RemoveChildren();
	void			 SetIsVisible(bool bIsVisible);
	void			 SetEnabled(bool bIsEnabled);
	void			 SetDraggable(bool bIsDraggable);
	void			 LockDragAxis(bool LockX, bool LockY);
	VVector2		 GetParentTransform();
	Delegate OnLeftMouseButtonDownDelegate;
	Delegate OnRightMouseButtonDownDelegate;
	Delegate OnLeftMouseDoubleClickDelegate;
	Delegate OnRightMouseDoubleClickDelegate;
	Delegate OnWheelUpDelegate;
	Delegate OnWheelDownDelegate;
	Delegate OnDragDelegate;
	Delegate OnRemoveDelegate;
protected:
	
	VColor		Color;
	VColor		OutlineColor;
	VVector2	Size;
	VVector2	Transform;
	VVector2    RelativeTransform;
	VVector2    ParentTransform;
	
	int				 ZOrder;
	bool		     bIsEnabled;
	bool			 bIsVisible;
	bool			 bIsClipping;
	bool			 bIsDraggable;
	bool			 bIsAxisXLocked;
	bool			 bIsAxisYLocked;
	bool			 bInheritSize;
	enum WidgetState CurrentState;
	enum WidgetType  Type;

	
};

template<class C>
static C * CreateWidget()
{
	C* Widget = new C;

	return Widget;
}