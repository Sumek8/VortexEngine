#pragma once
#include "Widget.h"
#include "Object.h"
class Composer :public Widget
{
public:
	Composer();
	~Composer();


	void		 UpdateChildren();
	

	vector<float>DistributionArray;

	void		  UpdateChildTransform();
	void		  AddChildWidget(Widget* Child);
	void		  SetIsHorizontal(bool bIsHorizontalSplit);
private:
	bool		 bIsHorizontal;
	void		  RecalculateWindow();
	void		  UpdateSeparators();
	void		  CreatePartitionWidget();
	void		  NormalizeDistribution();
	int			  SeparatorSize;
	void		  AddSeparator(Widget* ChildChild);
};

