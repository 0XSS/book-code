//=======================================================
// VTK�������Գ���
//=======================================================

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "Windows.h"       

//-------------------------------------------------------
// ������
//-------------------------------------------------------
int main()
{
    // ����һ��Բ׶��Դ����
    vtkConeSource *cone = vtkConeSource::New();
    // ����Բ׶��ĸ�
    cone->SetHeight( 3.0 );
    // ����Բ׶��İ뾶
    cone->SetRadius( 1.0 );
    // ����Բ׶��ľ�ϸ��
    cone->SetResolution( 10 );

    // ����һ������ӳ�����
    vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
    // ��Բ׶��Դ�������������ӳ�����
    coneMapper->SetInputConnection( cone->GetOutputPort() );

    // ����һ����Ա����
    vtkActor *coneActor = vtkActor::New();
    // ������ӳ������������Ա����
    coneActor->SetMapper( coneMapper );
   
    // ����һ����ʾ�߶���
    vtkRenderer *aRenderer= vtkRenderer::New();
    // ����Ա������ӵ���ʾ�߶���
    aRenderer->AddActor( coneActor );
    // ������ʾ�߶���ı�����ɫ
    aRenderer->SetBackground( 0.1, 0.2, 0.4 );

    // ����һ�����ƴ��ڶ���
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    // ����ʾ�߶�����ӵ����ƴ�����
    renWin->AddRenderer( aRenderer );
    // ���û��ƴ��ڴ�С
    renWin->SetSize( 300, 300 );

    // �Ӳ�ͬ�Ƕȹ۲�Բ׶��
    for (int i = 0; i < 25; ++i)
    {
        // ����
        renWin->Render();
        // ��ͣ
        Sleep(500);
        // �ı��ӽ�
        aRenderer->GetActiveCamera()->Azimuth( i );
    }

    // ��������ͷſռ�
    cone->Delete();
    coneMapper->Delete();
    coneActor->Delete();
    aRenderer->Delete();
    renWin->Delete();

    return 0;
}
