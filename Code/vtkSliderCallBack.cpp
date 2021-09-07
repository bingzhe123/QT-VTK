#include "vtkSliderCallBack.h"
#include "vtkSliderWidget.h"
#include "vtkSliderRepresentation.h"
vtkSliderCallBack::vtkSliderCallBack()
{
}

vtkSliderCallBack::~vtkSliderCallBack()
{
}

void vtkSliderCallBack::Execute(vtkObject * caller, unsigned long, void *)
{
    vtkSliderWidget *sliderWidget =  reinterpret_cast<vtkSliderWidget*>(caller);
    double scaleValue = static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue();
    cout << "scaleValue: " << scaleValue << endl;    
}
