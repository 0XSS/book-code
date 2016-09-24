// HookDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HookDll.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

#pragma data_seg("SharedDataName")
HHOOK g_hKeyBoardHook = NULL;
CListBox *g_pList;
#pragma data_seg()
#pragma comment(linker,"/section:.SharedDataName,rws")



//HHOOK g_hKeyBoardHook;//ȫ�ֱ��������湳�ӵľ��
LRESULT CALLBACK HookKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

/////////////////////////////////////////////////////////////////////////////
// CHookDllApp

BEGIN_MESSAGE_MAP(CHookDllApp, CWinApp)
    //{{AFX_MSG_MAP(CHookDllApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookDllApp construction

CHookDllApp::CHookDllApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHookDllApp object

CHookDllApp theApp;

LRESULT CALLBACK HookKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0) {
        CString strTemp, strAll;
        strTemp.Format("������%c[%d] ", wParam, wParam);
        strAll += strTemp;
        strTemp.Format("���ͣ�%s ", nCode == HC_NOREMOVE ? "����" : "�Ƴ�");
        strAll += strTemp;
        //0-15λ�����������¼��̴�����
        //16-23λ:ָ��ɨ����. ������OEM
        //24λ:��24λΪ1ʱ�򣺱�ʾ��������չ��;��24λΪ0ʱ�򣺱�ʾ���������ּ��̰���
        //25-28λ:����λ
        //29λ:�����ļ���Ϊ1ʱ�� ALT���£��������Ϊ0
        //30λ:����ǰ������º��͵���Ϣ��30λΪ0������ǰ���̧���30λΪ1��
        //31λ:ָ��ת��״̬;31λΪ0ʱ�򣬰������ڱ����£�Ϊ1ʱ�򣬰������ڱ��ͷ�
        strTemp.Format("����������%d", (UINT)lParam & 0xFFFF);
        strAll += strTemp;
        strTemp.Format("ɨ���룺%d", (UINT)(lParam & 0x0FF0000) / 0x10000);
        strAll += strTemp;
        strTemp.Format("�����ͣ�%s", (lParam & 0x1000000) == 0 ? "���ּ���" : "��չ��");
        strAll += strTemp;
        strTemp.Format("ALT��%s", (lParam & 0x20000000) == 0 ? "δ��" : "����");
        strAll += strTemp;
        strTemp.Format("��ǰ״̬��%s", (lParam & 0x40000000) == 0 ? "δ̧��" : "��̧��");
        strAll += strTemp;
        strTemp.Format("ת��״̬��%s", (lParam & 0x80000000) == 0 ? "��������" : "�����ͷ�");
        strAll += strTemp;
        //AfxMessageBox(strAll);
        g_pList->InsertString(0, strAll);
    }

    return CallNextHookEx(g_hKeyBoardHook, nCode, wParam, lParam);
}

void CHookDllApp::StartHook(CListBox *pList)
{
    g_pList = pList;
    g_hKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD, HookKeyBoardProc, NULL, GetCurrentThreadId());
    //CString strTemp;
    //strTemp.Format("%d",g_hKeyBoardHook);
    //AfxMessageBox(strTemp);
}

void CHookDllApp::StopHook()
{
    UnhookWindowsHookEx(g_hKeyBoardHook);
    g_hKeyBoardHook = NULL;
}

