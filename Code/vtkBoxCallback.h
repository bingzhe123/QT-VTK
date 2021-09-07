/**************************************************************************
Copyright: N.A

Author: N.A

Date: 2021.06.27

Description: ²Ã¼ô»Øµ÷Àà
**************************************************************************/
#ifndef  VTKBOXCALLBACK_H_
#define  VTKBOXCALLBACK_H_
#include "vtkCommand.h"
#include "vtkCallbackCommand.h"
#include "vtkBoxWidget2.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkSmartPointer.h"
#include <string>

class vtkBoxCallback :
    public vtkCallbackCommand
{
public:
    vtkBoxCallback();
    ~vtkBoxCallback();
    static vtkBoxCallback*New() { return new vtkBoxCallback; }
    virtual void Execute(vtkObject*caller, unsigned long, void*);
    void GetCurrentBounds(double bounds[6]);
public:
    double m_currentBounds[6];
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> m_spVolumeMapper;
};
#endif // filedef !   



