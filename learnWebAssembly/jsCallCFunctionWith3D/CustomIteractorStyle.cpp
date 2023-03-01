#include "CustomIteractorStyle.h"
#include <string>
#include <iostream>

CustomIteractorStyle::CustomIteractorStyle()
{
	std::cout << "sym: " << std::endl;
}

CustomIteractorStyle::~CustomIteractorStyle()
{

}

void CustomIteractorStyle::OnKeyPress()
{
	vtkRenderWindowInteractor* rwi = this->Interactor;
	int isCtrl = rwi->GetControlKey();
	std::string sym = rwi->GetKeySym();
	std::cout << "sym: " << sym << std::endl;
	if( "W" == sym )
	{

	}
}
