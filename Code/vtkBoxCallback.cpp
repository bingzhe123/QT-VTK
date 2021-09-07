#include "vtkBoxCallback.h"
#include "vtkBoxWidget2.h"
#include "vtkSmartPointer.h"
#include "vtkBoxRepresentation.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkClipPolyData.h"
#include "vtkPlanes.h" 

vtkBoxCallback::vtkBoxCallback()
{
}

vtkBoxCallback::~vtkBoxCallback()
{
}

void vtkBoxCallback::Execute(vtkObject * caller, unsigned long, void * clientData)
{
    vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkBoxWidget2::SafeDownCast(caller);
    vtkBoxRepresentation* box = static_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation());
    vtkSmartPointer< vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    box->GetPolyData(polyData);
    polyData->GetBounds(m_currentBounds); 


   // vtkSmartPointer<vtkPlanes> planesClipping = vtkSmartPointer<vtkPlanes>::New();
   // vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation())->GetPlanes(planesClipping);
    //m_spVolumeMapper->SetClippingPlanes(planesClipping);
    m_spVolumeMapper->SetCroppingRegionPlanes(m_currentBounds);
    /*vtkClipPolyData* clipperPoly = static_cast<vtkClipPolyData*>(clientData);
    clipperPoly->SetClipFunction(planesClipping);*/
}
void vtkBoxCallback::GetCurrentBounds(double bounds[6])
{
    memcpy(bounds, m_currentBounds, sizeof(double) * 6);
}
