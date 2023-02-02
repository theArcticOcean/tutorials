#include "CustomIteractorStyle.h"
#include "point.hpp"

#include <vtkCamera.h>

void CustomIteractorStyle::OnMouseWheelForward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
    KeepSameCameraStatus();
}

void CustomIteractorStyle::OnMouseWheelBackward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
    KeepSameCameraStatus();
}

void CustomIteractorStyle::Rotate()
{
    vtkInteractorStyleTrackballCamera::Rotate();
    KeepSameCameraStatus();
}

void CustomIteractorStyle::Pan()
{
    vtkInteractorStyleTrackballCamera::Pan();
    KeepSameCameraStatus();
}

void CustomIteractorStyle::Init()
{
    vtkSPtrNew( matrix1, vtkMatrix4x4 );
    matrix1->DeepCopy( m_LeftRender->GetActiveCamera()->GetViewTransformObject()->GetMatrix() );
    m_LastTrans->SetMatrix( matrix1 );
    m_LastTrans->Update();
}

CustomIteractorStyle::CustomIteractorStyle()
{
    m_LastTrans = vtkSmartPointer<vtkTransform>::New();
}

CustomIteractorStyle::~CustomIteractorStyle()
{
}

void CustomIteractorStyle::KeepSameCameraStatus()
{
    vtkCamera *targetCamera = m_RightRender->GetActiveCamera();
    vtkCamera *sampleCamera = m_LeftRender->GetActiveCamera();

    vtkSmartPointer<vtkTransform> curTrans = vtkSmartPointer<vtkTransform>::New();
    vtkSPtrNew( matrix1, vtkMatrix4x4 );
    matrix1->DeepCopy( sampleCamera->GetViewTransformObject()->GetMatrix() );
    curTrans->SetMatrix( matrix1 );
    curTrans->Update();

    vtkSmartPointer<vtkTransform> LastTransInv = vtkSmartPointer<vtkTransform>::New();
    LastTransInv->DeepCopy( m_LastTrans );
    LastTransInv->Inverse();
    LastTransInv->Update();

    vtkSmartPointer<vtkTransform> diffTrans = vtkSmartPointer<vtkTransform>::New();
    diffTrans->Concatenate( LastTransInv );
    diffTrans->Concatenate( curTrans );
    diffTrans->Update();

    diffTrans->Inverse();

    vtkSmartPointer<vtkTransform> finalTrans = vtkSmartPointer<vtkTransform>::New();
    vtkSPtrNew( matrix2, vtkMatrix4x4 );
    matrix2->DeepCopy( targetCamera->GetViewTransformObject()->GetMatrix() );
    finalTrans->SetMatrix( matrix2 );
    finalTrans->Update();
    finalTrans->Concatenate( diffTrans );
    finalTrans->Update();
    vtkMatrix4x4 *matrix = finalTrans->GetMatrix();
    Point newPos(-matrix->GetElement(0,3), -matrix->GetElement(1, 3), -matrix->GetElement(2, 3));
    //Point viewPlaneNormal( matrix->GetElement(2, 0), matrix->GetElement(2, 1), matrix->GetElement(2, 2) );

    //targetCamera->SetPosition( newPos.point );

    targetCamera->ApplyTransform( diffTrans ); //finalTrans ); //
    targetCamera->SetParallelScale( sampleCamera->GetParallelScale() );

    m_LastTrans->DeepCopy( curTrans );
    m_LastTrans->Update();
    m_RightRender->ResetCameraClippingRange();

    m_LeftRender->GetRenderWindow()->Render();
}
