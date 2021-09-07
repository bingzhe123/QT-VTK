#include "MyVtkAxisActor3D.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
MyVtkAxisActor3D * MyVtkAxisActor3D::New()
{
    return new MyVtkAxisActor3D();
}
MyVtkAxisActor3D::MyVtkAxisActor3D()
{
  
}

vtkActor* MyVtkAxisActor3D::GetXAxisActor()
{
    return XAxisTip;
}

vtkActor * MyVtkAxisActor3D::GetYAxisActor()
{
    return YAxisTip;
}

vtkActor * MyVtkAxisActor3D::GetZAxisActor()
{
    return ZAxisTip;
}
