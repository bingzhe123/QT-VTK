
/*----------------------------------------------------------------------------
   Component       : MyVtkAxisActor3D
   Name            :
   Author          :
   Creation Date   :
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/

#include "vtkSmartPointer.h"
#include "vtkArrowSource.h"
#include "vtkDataSetMapper.h"
#include "vtkMinimalStandardRandomSequence.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkAxesActor.h"


class MyVtkAxisActor3D:public vtkAxesActor
{
public:
    vtkTypeMacro(MyVtkAxisActor3D, MyVtkAxisActor3D);
    static MyVtkAxisActor3D* New();

    MyVtkAxisActor3D();

    vtkActor*GetXAxisActor();
    vtkActor*GetYAxisActor();
    vtkActor*GetZAxisActor();  
};

