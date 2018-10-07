#pragma once
#include "Object.h"
#include "Canvas.h"
class Composer :public Canvas
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

