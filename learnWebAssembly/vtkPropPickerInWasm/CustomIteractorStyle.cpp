#include "CustomIteractorStyle.h"
#include "worker.h"

//vtkStandardNewMacro(CustomIteractorStyle);

CustomIteractorStyle::CustomIteractorStyle()
{
    m_Worder = nullptr;
}

CustomIteractorStyle::~CustomIteractorStyle()
{
    m_Worder = nullptr;
}

void CustomIteractorStyle::OnLeftButtonDown()
{
    if( m_Worder )
    {
        m_Worder->OnLeftButtonDown();
    }
}