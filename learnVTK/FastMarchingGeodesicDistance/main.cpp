#include <iostream>
#include <fstream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkTextSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCoordinate.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <string>
#include <vtkTransform.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>

#include "vtkFastMarchingGeodesicDistance.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

void MarkPoint(int index, vtkPolyData* pd, vtkRenderer* render)
{
    cout << "index: " << index << endl;
    vtkSmartPointer<vtkTextSource> text2D =
        vtkSmartPointer<vtkTextSource>::New();
    text2D->SetText(std::to_string(index).c_str());

    vtkSmartPointer<vtkTransform> text2DTransform =
        vtkSmartPointer<vtkTransform>::New();
    double* center = pd->GetPoint(index);
    text2DTransform->Translate(center[0], center[1], center[2]);
    text2DTransform->Scale(0.005, 0.005, 0.005);

    vtkSmartPointer<vtkTransformPolyDataFilter> text2DDataFilter =
        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    text2DDataFilter->SetTransform(text2DTransform);
    text2DDataFilter->SetInputConnection(text2D->GetOutputPort());

    vtkSmartPointer<vtkCoordinate> coords =
        vtkSmartPointer<vtkCoordinate>::New();
    coords->SetCoordinateSystemToWorld();

    vtkSmartPointer<vtkPolyDataMapper2D> text2DMapper =
        vtkSmartPointer<vtkPolyDataMapper2D>::New();
    text2DMapper->SetInputConnection(text2DDataFilter->GetOutputPort());
    text2DMapper->SetTransformCoordinate(coords);

    vtkSmartPointer<vtkActor2D> text2DActor =
        vtkSmartPointer<vtkActor2D>::New();
    text2DActor->SetMapper(text2DMapper);

    render->AddActor(text2DActor);
}

int main()
{
    ofstream out("out.txt");
    streambuf* oldCout = cout.rdbuf(out.rdbuf());

    vtkSPtrNew(renderer, vtkRenderer);
    vtkSPtrNew(sphere, vtkSphereSource);
    sphere->SetCenter(0, 0, 0);
    sphere->SetRadius(1);
    sphere->SetThetaResolution( 100 );
    sphere->SetPhiResolution( 50 );
    sphere->Update();

    vtkPolyData* data = sphere->GetOutput();
    data->BuildCells();
    data->BuildLinks();

    vtkSPtrNew(ptsScalars, vtkIntArray);
    ptsScalars->SetNumberOfTuples(data->GetNumberOfPoints());
    for (int i = 0; i < data->GetNumberOfPoints(); ++i)
    {
        ptsScalars->SetTuple1(i, 0);
    }
    data->GetPointData()->SetScalars(ptsScalars);

    int seedCellId = 0;
    MarkPoint(seedCellId, data, renderer);

    vtkSPtrNew(seedList, vtkIdList);
    seedList->InsertNextId(seedCellId);

    double disLimit = 0.2;
    vtkSPtrNew(fastMarching, vtkFastMarchingGeodesicDistance);
	fastMarching->SetInputData( data );
	fastMarching->SetSeeds(seedList);
	fastMarching->SetDistanceStopCriterion(disLimit);
	fastMarching->SetFieldDataName("GeodesicDis");
	fastMarching->Update();
	vtkPolyData* outputForDistance = fastMarching->GetOutput();
	vtkFloatArray* distanceArray = vtkFloatArray::SafeDownCast(outputForDistance->GetPointData()->GetArray("GeodesicDis"));

    for (int i = 0; i < data->GetNumberOfPoints(); ++i)
    {
        double dis = distanceArray->GetValue(i);
        if (dis > 0 && dis < disLimit)
        {
            ptsScalars->SetTuple1(i, 1);
        }
        std::cout << "i: " << i <<", dis: " << dis << std::endl;
    }

    vtkSPtrNew(lut, vtkColorTransferFunction);
    lut->SetClamping(0);
    double colors[4][3] = { {199, 21, 133}, { 0, 255, 0 }, {85, 107, 47}, { 123, 104, 238 } };
    for (int i = 0; i < 4; ++i)
    {
        lut->AddRGBPoint(i, colors[i][0] / 255.0, colors[i][1] / 255.0, colors[i][2] / 255.0);
    }

    vtkSPtrNew(mapper, vtkPolyDataMapper);
    mapper->SetInputData(data);
    mapper->SetScalarModeToUsePointData();
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(0, 3);

    vtkSPtrNew(sphereActor, vtkActor);
    sphereActor->SetMapper(mapper);

    renderer->AddActor(sphereActor);
    renderer->SetBackground(0, 0, 0);

    vtkSPtrNew(renderWindow, vtkRenderWindow);
    renderWindow->AddRenderer(renderer);

    vtkSPtrNew(renderWindowInteractor, vtkRenderWindowInteractor);
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();

    cout.rdbuf(oldCout);
    out.close();
    return 0;
}
