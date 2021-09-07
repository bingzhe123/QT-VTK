
/*----------------------------------------------------------------------------
   Component       : MyVtkAxisActor2D
   Name            :
   Author          :
   Creation Date   :
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/

#ifndef  MYVTKAXISACTOR2D_H_
#define  MYVTKAXISACTOR2D_H_


#include "vtkAxisActor2D.h"
class MyVtkAxisActor2D :public vtkAxisActor2D
{
public:
	vtkTypeMacro(MyVtkAxisActor2D, vtkAxisActor2D);
	MyVtkAxisActor2D();
	virtual ~MyVtkAxisActor2D();

	static MyVtkAxisActor2D* New();

	virtual void BuildAxis(vtkViewport *viewport);

	void SetLabelsNumber(int iNumber);

	bool m_bValidValue = true;

};

#endif