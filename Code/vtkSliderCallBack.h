#ifndef  VTKSLIDERCALLBACK_H_
#define  VTKSLIDERCALLBACK_H_
#include "vtkCommand.h"
#include "vtkCallbackCommand.h"
#include "vtkBoxWidget2.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkSmartPointer.h"
#include <string>
class vtkSliderCallBack : public vtkCallbackCommand
{
public:
    vtkSliderCallBack();
    ~vtkSliderCallBack();
    static vtkSliderCallBack*New() { return new vtkSliderCallBack; }
    virtual void Execute(vtkObject*caller, unsigned long, void*);
public:
};
#endif

