//=======================================================
// ����MC�㷨��CTͼ���ؽ�
//=======================================================

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkStripper.h"
#include "vtkPolyDataNormals.h"
#include "vtkMarchingCubes.h"

//-------------------------------------------------------
// ������
//-------------------------------------------------------
int main (int argc, char **argv)
{

    // �����������
    if(argc < 2)
    {
        cout << "�÷��� " << argv[0] << " DATADIR/headsq/quarter" << endl;
        return 1;
    }

    // ---------��ȡ����---------

    // ����2D���ݶ��߶���
    vtkVolume16Reader *v16 = vtkVolume16Reader::New();
    // �������ݵĸ�ά��С
    v16->SetDataDimensions(64,64);
    // �������ݵ��ֽ����з�ʽ
    v16->SetDataByteOrderToLittleEndian();
    // �����ļ�ǰꡣ����������һ�������ԡ�FilePrefix.%d����ʽ��ȡ����ļ�
    v16->SetFilePrefix (argv[1]);
    // �����ļ���ȡ��Χ
    v16->SetImageRange(1, 93);
    // ����CT���ݵ���Ƭ���ͼ��
    v16->SetDataSpacing (3.2, 3.2, 1.5);  
    
    // ---------�ؽ�Ƥ��---------

    // ����һ��Marching Cubes�㷨�Ķ���
    vtkMarchingCubes *skinExtractor = vtkMarchingCubes::New();
    // �������ȡ��CT����
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    // ��ȡ��CTֵΪ500��Ƥ������
    skinExtractor->SetValue(0, 500);
    // ���¼��㷨����
    vtkPolyDataNormals *skinNormals = vtkPolyDataNormals::New();
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);
    // �������Ǵ�����
    vtkStripper *skinStripper = vtkStripper::New();
    // �����ɵ�����Ƭ���ӳ����Ǵ�
    skinStripper->SetInputConnection(skinNormals->GetOutputPort());
    // ����һ������ӳ�����
    vtkPolyDataMapper *skinMapper = vtkPolyDataMapper::New();
    // �����Ǵ�ӳ��Ϊ��������
    skinMapper->SetInputConnection(skinStripper->GetOutputPort()); 
    skinMapper->ScalarVisibilityOff();
    // ����һ������Ƥ������Ա����
    vtkActor *skin = vtkActor::New();
    // ���Ƥ���������ݵ�����
    skin->SetMapper(skinMapper);
    // ����Ƥ����ɫ������
    skin->GetProperty()->SetDiffuseColor(1, .49, .25);
    // ���÷�����
    skin->GetProperty()->SetSpecular(.3);
    // ���÷����ǿ�� 
    skin->GetProperty()->SetSpecularPower(20);
    // ���ò�͸����
    skin->GetProperty()->SetOpacity(1.0);

    // ---------�ؽ�����---------

    // ����һ��Marching Cubes�㷨�Ķ���
    vtkMarchingCubes *boneExtractor = vtkMarchingCubes::New();
    // �������ȡ��CT����
    boneExtractor->SetInputConnection(v16->GetOutputPort());
    // ��ȡ��CTֵΪ1150�Ĺ�������
    boneExtractor->SetValue(0, 1150);
    // ���¼��㷨����
    vtkPolyDataNormals *boneNormals = vtkPolyDataNormals::New();
    boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
    boneNormals->SetFeatureAngle(60.0);
    // �������Ǵ�����
    vtkStripper *boneStripper = vtkStripper::New();
    // �����ɵ�����Ƭ���ӳ����Ǵ�
    boneStripper->SetInputConnection(boneNormals->GetOutputPort());
    // ����һ������ӳ�����
    vtkPolyDataMapper *boneMapper = vtkPolyDataMapper::New();
    // �����Ǵ�ӳ��Ϊ��������
    boneMapper->SetInputConnection(boneStripper->GetOutputPort());
    boneMapper->ScalarVisibilityOff();
    // ����һ�������������Ա����
    vtkActor *bone = vtkActor::New();
    // ��ù����������ݵ�����
    bone->SetMapper(boneMapper);
    // ���ù�����ɫ������
    bone->GetProperty()->SetDiffuseColor(1, 1, .9412);

    // ---------��������---------

    // ������ͼ������ܽ����������Է������û�����
    vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
    outlineData->SetInputConnection(v16->GetOutputPort());
    vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
    mapOutline->SetInputConnection(outlineData->GetOutputPort());
    vtkActor *outline = vtkActor::New();
    outline->SetMapper(mapOutline);
    outline->GetProperty()->SetColor(0,0,0);

    // ---------�������---------

    // ����һ���������
    vtkCamera *aCamera = vtkCamera::New();
    // ����������ӽ�
    aCamera->SetViewUp (0, 0, -1);
    // ���������λ��
    aCamera->SetPosition (0, 1, 0);
    // ��������Ľ���
    aCamera->SetFocalPoint (0, 0, 0);
    aCamera->ComputeViewPlaneNormal();

    // ---------׼����ʾ---------
    
    // ������ʾ�߶���
    vtkRenderer *aRenderer = vtkRenderer::New();
    // ������ʾ���ڶ���
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    // ����ʾ����ӵ���ʾ����
    renWin->AddRenderer(aRenderer);
    // ������ʾ���ڽ�������ʹ���û���������ꡢ���̺ʹ��ڽ���
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    // ����ʾ���ڹ�������ʾ���ڽ�������
    iren->SetRenderWindow(renWin);

    // ��������ӵ���ʾ�߶���
    aRenderer->AddActor(outline);
    // ��Ƥ����ӵ���ʾ�߶���
    aRenderer->AddActor(skin);
    // ��������ӵ���ʾ�߶���
    aRenderer->AddActor(bone);
    // ������ʾ����ʹ�õ����
    aRenderer->SetActiveCamera(aCamera);
    // ʹ�����Ч
    aRenderer->ResetCamera();
    aCamera->Dolly(1.5);  
    // ������ʾ�ߵı�����ɫ
    aRenderer->SetBackground(1,1,1);
    // ���������ȡƽ��
    aRenderer->ResetCameraClippingRange();

    // ������ʾ���ڴ�С
    renWin->SetSize(640, 480);

    // ---------��ʼ��ʾ---------

    // ��ʾ���ڽ��������ʼ��
    iren->Initialize();
    // ��ʼ��ʾ
    iren->Start(); 

    // ---------��ʾ����---------

    // ɾ�������ͷſռ�
    v16->Delete();
    skinExtractor->Delete();
    skinNormals->Delete();
    skinStripper->Delete();
    skinMapper->Delete();
    skin->Delete();
    boneExtractor->Delete();
    boneNormals->Delete();
    boneStripper->Delete();
    boneMapper->Delete();
    bone->Delete();
    outlineData->Delete();
    mapOutline->Delete();
    outline->Delete();
    aCamera->Delete();
    aRenderer->Delete();
    renWin->Delete();
    iren->Delete();

    return 0;
}
