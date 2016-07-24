// wuziqiView.cpp : implementation of the CWuziqiView class
//

#include "stdafx.h"
#include "wuziqi.h"
#include "MainFrm.h"

#include "wuziqiDoc.h"
#include "wuziqiView.h"
#include "chess.h"
#include "blocksock.h"
#include "connectto.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWuziqiView

IMPLEMENT_DYNCREATE(CWuziqiView, CView)

BEGIN_MESSAGE_MAP(CWuziqiView, CView)
	//{{AFX_MSG_MAP(CWuziqiView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BACK_BLACK, OnBackBlack)
	ON_UPDATE_COMMAND_UI(ID_BACK_BLACK, OnUpdateBackBlack)
	ON_COMMAND(ID_BACK_WHITE, OnBackWhite)
	ON_UPDATE_COMMAND_UI(ID_BACK_WHITE, OnUpdateBackWhite)
	ON_COMMAND(ID_RULE_CHANGE, OnRuleChange)
	ON_UPDATE_COMMAND_UI(ID_RULE_CHANGE, OnUpdateRuleChange)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_RULE_3344, OnRule3344)
	ON_COMMAND(ID_RULE_CLEAN, OnRuleClean)
	ON_COMMAND(ID_CONNECT, OnConnect)
	ON_COMMAND(ID_STARTSERVER, OnStartserver)
	ON_UPDATE_COMMAND_UI(ID_STARTSERVER, OnUpdateStartserver)
	ON_COMMAND(ID_DISCONNECT, OnDisconnect)
	ON_UPDATE_COMMAND_UI(ID_DISCONNECT, OnUpdateDisconnect)
	ON_UPDATE_COMMAND_UI(ID_RULE_RESTART, OnUpdateRuleRestart)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_RULE_RESTART, OnRuleRestart)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONNECT,OnConnect)//�õ��Է��������ݷ��͵���Ϣ
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWuziqiView construction/destruction
CBlockingSocket g_sListen;//���Ӷ���ʵ��
 UINT ServerThreadProc(LPVOID pParam);//�������˽���
  UINT ClientSocketThreadProc(LPVOID pParam);//�������ݽ���
 char ACCEPT[64],SEND[64];//����,����,��������
 CString CONADDR,SERVER;//ip,�������
CSockAddr saClient;//ip��ַ

CWuziqiView::CWuziqiView()
{
	who=whotemp=whohasjinshou=1;//��ʼ�������Ǻ���
	connect=youcansendmessage=connect=startserver=0;//��ʼ��û������
WSADATA wsd;//��ʼ����������
	VERIFY(WSAStartup(0x0101, &wsd) == 0);
	TRACE("WSAStartup -- min version = %x\n", wsd.wVersion);

}

CWuziqiView::~CWuziqiView()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// CWuziqiView drawing

void CWuziqiView::OnDraw(CDC* pDC)
{
	int nline,drawx,drawy,nCount;
	CString str,out;
	char number[3];
	number[2]=number[1]='\0';number[0]='0';
	CRect rectClient;
	nCount=0;

	GetClientRect(rectClient);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(64,64);
	pDC->SetViewportExt(rectClient.right,rectClient.bottom);
//����ͼ��Ϊ64*64,û4��һ����
	for(nline=0;nline<=64;nline+=4)
	{//���ߺʹ�ӡ����
			pDC->MoveTo(nline,0);
			pDC->LineTo(nline,64);
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOut(nline,0,number);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->TextOut(0,nline,number);
			if(number[1]!='\0')number[1]++;
			else
			{
				number[0]++;
				if(number[0]>='9'+1)
				{
					number[0]='1';
					number[1]='0';
				}
			}
	}



	for(nline=0;nline<=64;nline+=4)
	{
		pDC->MoveTo(0,nline);
		pDC->LineTo(64,nline);
	}
//���̻������,��ʼ�����������ݻ�����,0Ϊû��,1Ϊ����,-1Ϊ����
	for(drawx=0;drawx<=17;drawx++)
		for(drawy=0;drawy<=17;drawy++)
		{
			if(all.allqipan[drawx][drawy]==1)
			{
				nCount++;
				out.Format("%d",nCount);
				CRect drawrect(drawx*4-1,drawy*4-1,drawx*4+1,drawy*4+1);
				CBrush brushHatch(RGB(34,108,243));
				pDC->SelectObject(&brushHatch);			
				pDC->Ellipse(drawrect);
			}
			if(all.qipantemp[drawx][drawy]==1)
			{
				CRect drawrect(drawx*4-1,drawy*4-1,drawx*4+1,drawy*4+1);
				CBrush brushHatch(RGB(180,231,180));
				pDC->SelectObject(&brushHatch);
				
				pDC->Ellipse(drawrect);
			}
			if(all.allqipan[drawx][drawy]==-1)
			{
				CRect drawrect(drawx*4-1,drawy*4-1,drawx*4+1,drawy*4+1);
				CBrush brushHatch(RGB(78,229,78));
				pDC->SelectObject(&brushHatch);
				pDC->Ellipse(drawrect);
			}
			if(all.qipantemp[drawx][drawy]==-1)
			{
				CRect drawrect(drawx*4-1,drawy*4-1,drawx*4+1,drawy*4+1);
				CBrush brushHatch(RGB(165,191,239));
				pDC->SelectObject(&brushHatch);
				pDC->Ellipse(drawrect);
			}

		}
		

//����״̬��
		

		CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
		CStatusBar* pStatus=&pFrame->m_wndStatusBar;
		if(pStatus)
		{
			if(connect==0)
			{
				if(who==1)
				{
					str.Format("black down %d,white down %d,black's turn!",black.nCountkey,white.nCountkey);
				}
				else
				{
					str.Format("black down %d,white down %d,white's turn!",black.nCountkey,white.nCountkey);
				}
			}
			if(connect==1)
			{
				if(youcansendmessage==1)
				{
					int out1,out2;
					if(black.nCountkey>=white.nCountkey)
					{
						out1=black.nCountkey;
						out2=white.nCountkey;
					}
					else
					{
						out1=white.nCountkey;
						out2=black.nCountkey;
					}
					str.Format("you down %d,her down %d,your turn!",out2,out1);
				}
				else
				{
					int out1,out2;
					if(black.nCountkey>=white.nCountkey)
					{
						out1=black.nCountkey;
						out2=white.nCountkey;
					}
					else
					{
						out1=white.nCountkey;
						out2=black.nCountkey;
					}

					str.Format("you down %d,her down %d,her turn!",out1,out2);
				}
			}


			pStatus->SetPaneText(0,str);
		}
//�Զ����Է���������
//	OnStartserver();
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWuziqiView diagnostics

#ifdef _DEBUG
void CWuziqiView::AssertValid() const
{
	CView::AssertValid();
}

void CWuziqiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWuziqiDoc* CWuziqiView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWuziqiDoc)));
	return (CWuziqiDoc*)m_pDocument;
}



#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWuziqiView message handlers

void CWuziqiView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	float temp;int com;
	int x,y;
	CRect rectClient;
	GetClientRect(rectClient);
	temp=point.x*16;
	temp=temp/rectClient.right;
	com=temp;
	if(temp-com>0.5)x=temp+1;
	else x=temp;

	temp=point.y*16;
	temp=temp/rectClient.bottom;
	com=temp;
	if(temp-com>0.5)y=temp+1;
	else y=temp;
//��ȷ��λ����λ��
//�жϿɲ���������
	if(connect==0)
	{//û������,ֱ�Ӹ�������
		if(all.allqipan[x][y]==0)
		{			MessageBeep(MB_OK);

			if(who==1) 
			{
				black.keydown(x,y,who);
				all.allqipan[x][y]=1;
				
				if(all.shuangsi(x,y)==1&&whohasjinshou==1)
				{
					Invalidate();
					resultwin lost;
					lost.m_Who.Format("black lost!double 4!");
					lost.DoModal();
					OnRuleRestart();
					return;
				}

				int win=all.winchanglian(x,y);
				if(win==1)
				{
					Invalidate();
					resultwin win;
					win.m_Who.Format("black win!");
					win.DoModal();
					OnRuleRestart();
					return;	
				}		
				if(win==0&&whohasjinshou==1)
				{
					Invalidate();
					resultwin lost;
					lost.m_Who.Format("black lost!long!");
					lost.DoModal();
					OnRuleRestart();
					return;
				}

				if(all.shuangsan(x,y)==1&&whohasjinshou==1) 
				{
					Invalidate();
					resultwin fail;
					fail.m_Who.Format("Black lost!double 3!");
					fail.DoModal();
					OnRuleRestart();
					return;		
				}


			}
			else
			{
				white.keydown(x,y,who);
				all.allqipan[x][y]=-1;
				if(all.shuangsi(x,y)==1&&whohasjinshou==-1)
				{
					Invalidate();
					resultwin lost;
					lost.m_Who.Format("white lost!double 4!");
					lost.DoModal();
					OnRuleRestart();
					return;
				}

				int win=all.winchanglian(x,y);
				if(win==1)			
				{
					Invalidate();
					resultwin win;
					win.m_Who.Format("White win!");
					win.DoModal();
					OnRuleRestart();
					return;	
				}
				if(win==0&&whohasjinshou==-1)
				{				
					Invalidate();				
					resultwin lost;
					lost.m_Who.Format("white lost!long!");
					lost.DoModal();
					OnRuleRestart();
					return;
				}

				if(all.shuangsan(x,y)==1&&whohasjinshou==-1) 
				{
					Invalidate();
					resultwin fail;
					fail.m_Who.Format("white lost!double 3!");
					fail.DoModal();
					OnRuleRestart();
					return;		
				}

	/*			CString str;
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
				CStatusBar* pStatus=&pFrame->m_wndStatusBar;
				if(pStatus)
				{
					str.Format("white downed %d",white.nCountkey);
					pStatus->SetPaneText(1,str);
				}*/
			}

			who=-who;
		}
	}	
//////////////////////////////////////////////////////////////////
	if(connect==1&&youcansendmessage==1)
	{//��������ӻ�Ҫ�������ݵ��Է�����
		if(whoiam==1)
		{
			who=1;
		}
		else
		{
			who=-1;
		}
		if(all.allqipan[x][y]==0)
		{			MessageBeep(MB_OK);//��������

			if(whoiam==1)
			{
				SEND[0]='3';//�º���
			}
			else
			{
				SEND[0]='4';//�°���
			}
			SEND[2]=x;
			SEND[3]=y;
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
			youcansendmessage=0;
			if(who==1) 
			{//������Ǻ���
				whohasjinshou=1;
				black.keydown(x,y,who);
				all.allqipan[x][y]=1;
				
				if(all.shuangsi(x,y)==1&&whohasjinshou==1)
				{
					Invalidate();
					resultwin lost;
					lost.m_Who.Format("black lost!double 4!");
					lost.DoModal();
					youcansendmessage=1;whoiam=1;
					OnRuleRestart();
				//	return;
				}

				int win=all.winchanglian(x,y);
				if(win==1)
				{
					Invalidate();
					resultwin win;
					win.m_Who.Format("black win!");
					win.DoModal();
					youcansendmessage=0;whoiam=-1;
					OnRuleRestart();
				//	return;	
				}		
				if(win==0&&whohasjinshou==1)
				{
					Invalidate();
					resultwin lost;
					lost.m_Who.Format("black lost!long!");
					lost.DoModal();
					youcansendmessage=1;whoiam=1;
					OnRuleRestart();
					//return;
				}

				if(all.shuangsan(x,y)==1&&whohasjinshou==1) 
				{
					Invalidate();
					resultwin fail;
					fail.m_Who.Format("Black lost!double 3!");
					fail.DoModal();
					youcansendmessage=1;whoiam=1;
					OnRuleRestart();
				//	return;		
				}

			}
			else
			{//���ǰ���
				//sendmessage
				whohasjinshou=-1;
				white.keydown(x,y,who);
				all.allqipan[x][y]=-1;
				if(all.shuangsi(x,y)==1&&whohasjinshou==1)
				{
					Invalidate();
					resultwin lost;
					lost.m_Who.Format("white lost!double 4!");
					youcansendmessage=1;whoiam=1;
					lost.DoModal();
					OnRuleRestart();
				//	return;
				}

				int win=all.winchanglian(x,y);
				if(win==1)			
				{
					Invalidate();
					resultwin win;
					win.m_Who.Format("White win!");
					youcansendmessage=0;whoiam=-1;
					win.DoModal();
					OnRuleRestart();
					//return;	
				}
				if(win==0&&whohasjinshou==1)
				{				
					Invalidate();				
					resultwin lost;
					lost.m_Who.Format("white lost!long!");
					lost.DoModal();
					youcansendmessage=1;whoiam=1;
					OnRuleRestart();
					//return;
				}

				if(all.shuangsan(x,y)==1&&whohasjinshou==1) 
				{
					Invalidate();
					resultwin fail;
					fail.m_Who.Format("white lost!double 3!");
					youcansendmessage=1;whoiam=1;
					fail.DoModal();
					OnRuleRestart();
				//	return;		
				}
			}

			/*if(youcansendmessage==1)
			{//�ҷ���������,�����˼Ҹ����������ݵ��ҵ���
				SEND[0]='6';
				AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());

			}*/
		

		}
	}				
	all.cleartemp();//���������������

	Invalidate();



	
	CView::OnLButtonDown(nFlags, point);
}

void CWuziqiView::OnBackBlack() 
{//��ɫ����
	int tempx,tempy;
//	CString str;
	tempx=black.nKeydown[black.nCountkey]/100;
	tempy=black.nKeydown[black.nCountkey]-100*tempx;
	black.bqipan[tempx][tempy]=0;
	all.allqipan[tempx][tempy]=0;
	black.nCountkey--;
	who=-who;

/*	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	if(pStatus)
	{
		str.Format("black downed %d",black.nCountkey);
		pStatus->SetPaneText(0,str);

	}*/

	Invalidate();

}

void CWuziqiView::OnUpdateBackBlack(CCmdUI* pCmdUI) 
{
	if(who!=-1||black.nCountkey==0)
		pCmdUI->Enable(0);
	if(connect==1)
		pCmdUI->Enable(0);

	
}

void CWuziqiView::OnBackWhite() 
{//��ɫ����
	int tempx,tempy;
//	CString str;

	tempx=white.nKeydown[white.nCountkey]/100;
	tempy=white.nKeydown[white.nCountkey]-100*tempx;
	white.bqipan[tempx][tempy]=0;
	all.allqipan[tempx][tempy]=0;
	white.nCountkey--;
	who=-who;
/*	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	if(pStatus)
	{
		str.Format("white downed %d",white.nCountkey);
		pStatus->SetPaneText(1,str);
	}
*/
	
	Invalidate();


	
}

void CWuziqiView::OnUpdateBackWhite(CCmdUI* pCmdUI) 
{
	if(who!=1||white.nCountkey==0)
		pCmdUI->Enable(0);
	if(connect==1)
		pCmdUI->Enable(0);


	
}


void CWuziqiView::OnRuleChange() 
{//��������(����)
	int count1,count2,temp,tempcountkey;
//	CString str;
	whohasjinshou=-whohasjinshou;
	for(count1=0;count1<=17;count1++)
		for(count2=0;count2<=17;count2++)
		{
			temp=black.bqipan[count1][count2];
			black.bqipan[count1][count2]=white.bqipan[count1][count2];
			white.bqipan[count1][count2]=temp;
			if(all.allqipan[count1][count2]!=0)
			{
				if(all.allqipan[count1][count2]==1)
				{
					all.allqipan[count1][count2]=-1;
				}
				else
				{
					all.allqipan[count1][count2]=1;
				}
			}
			
		}
	
	tempcountkey=white.nCountkey;
	white.nCountkey=black.nCountkey;
	black.nCountkey=tempcountkey;
	
	for(count1=0;count1<=31;count1++)
	{
		temp=black.nKeydown[count1];
		black.nKeydown[count1]=white.nKeydown[count1];
		white.nKeydown[count1]=temp;
	}
	who=-who;


/*	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	if(pStatus)
	{
		str.Format("white downed %d",white.nCountkey);
		pStatus->SetPaneText(1,str);
		str.Format("black downed %d",black.nCountkey);
		pStatus->SetPaneText(0,str);


	}*/

		Invalidate();

	if(connect==1)
	{//���������,���͵��Է�����
		if(whoiam!=1)
		{
		whoiam=-whoiam;
			youcansendmessage=1;
			SEND[0]='9';
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
		}
	else
	{
		whoiam=-whoiam;
		youcansendmessage=0;
	}}
	
}

void CWuziqiView::OnUpdateRuleChange(CCmdUI* pCmdUI) 
{
	if((black.nCountkey+white.nCountkey==5&&connect==0)||(black.nCountkey+white.nCountkey==5&&connect==1&&whoiam!=1))
		pCmdUI->Enable(1);

	else
		pCmdUI->Enable(0);	
}

void CWuziqiView::OnRButtonDown(UINT nFlags, CPoint point) 
{//�������,����,���˫���������
	float temp;int com;
	int x,y;
	CRect rectClient;
	GetClientRect(rectClient);
	temp=point.x*16;
	temp=temp/rectClient.right;
	com=temp;
	if(temp-com>0.5)x=temp+1;
	else x=temp;

	temp=point.y*16;
	temp=temp/rectClient.bottom;
	com=temp;
	if(temp-com>0.5)y=temp+1;
	else y=temp;

	if(all.allqipan[x][y]==0)
	{
		if(all.qipantemp[x][y]!=0)
		{		
			switch(all.qipantemp[x][y])
			{
				case 1:
					all.qipantemp[x][y]=-1;
					break;
				case -1:
					all.qipantemp[x][y]=1;
					break;
			}
		}
		else
		{
			switch(whotemp)
			{
			case 1:
				all.qipantemp[x][y]=1;
				whotemp=-1;
				break;
			case -1:
				all.qipantemp[x][y]=-1;
				whotemp=1;
				break;
			}
		}

	}
	CView::OnRButtonDown(nFlags, point);
	Invalidate();

}

void CWuziqiView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{//���˫�����
	float temp;int com;
	int x,y;
	CRect rectClient;
	GetClientRect(rectClient);
	temp=point.x*16;
	temp=temp/rectClient.right;
	com=temp;
	if(temp-com>0.5)x=temp+1;
	else x=temp;

	temp=point.y*16;
	temp=temp/rectClient.bottom;
	com=temp;
	if(temp-com>0.5)y=temp+1;
	else y=temp;

	if(all.qipantemp[x][y]!=0)
	{
		all.qipantemp[x][y]=0;
	}
	Invalidate();

	
	CView::OnRButtonDblClk(nFlags, point);
}

void CWuziqiView::OnRuleRestart() 
{
//���¿�ʼ
	all.cleartemp();
	all.clear();
	black.clearall();
	white.clearall();
	who=1;//connect=0;
	Invalidate();

}

void CWuziqiView::OnRule3344() 
{
//ȡ��˫��˫������,û��д,�����鲻�ù��ʹ���
	//HINSTANCE  n=ShellExecute(NULL, "open", "Readme.htm", NULL,NULL, SW_SHOWNORMAL);	//::WinExec("C:\Program Files\Internet Explorer\IEXPLORE.EXE c:\1.html",SW_SHOW);
	
	// TODO: Add your command handler code here
}

void CWuziqiView::OnRuleClean() 
{//�����ǰ��������
	//::PostMessage(GetSafeHwnd(),WM_CONNECT,0,0);	
	all.cleartemp();
	Invalidate();
	
}
LRESULT CWuziqiView::OnConnect(WPARAM wParam,LPARAM lParam)
{//�ĵ��Է��������ݵ��õĳ���
	ShowWindow(SW_SHOWNORMAL);//�Ѵ��ڷŵ�����
	BringWindowToTop();
	if(ACCEPT[0]=='1')//��������,���ʹ���
	{
		if(connect==1)
		{//����Ѿ���������,���߶Է�
			SEND[0]='a';
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
			return 0;
		}

		askconnect con;
		con.m_pro="connect with 3344";
		con.m_ip=CONADDR;
		int n=con.DoModal();
		if(n==IDOK)
		{
			all.clear();black.nCountkey=0;white.nCountkey=0;				Invalidate();

			SERVER=CONADDR;
			connect=whoiam=1;
			SEND[0]='5';
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
			youcansendmessage=1;
		}
		else
		{
			SEND[0]='n';//���;ܾ���Ϣ
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
		}		
	}
	if(ACCEPT[0]=='2')
	{//��������,��ͨ����,������ʹ��
		if(connect==1)
		{
			SEND[0]='a';
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
			return 0;
		}

			all.clear();black.nCountkey=0;white.nCountkey=0;				Invalidate();

		SERVER=CONADDR;
		askconnect con;
		con.m_pro="connect without 3344";
		con.m_ip=CONADDR;
		int n=con.DoModal();
		if(n==IDOK)
		{
			connect=whoiam=1;
			SEND[0]='5';
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
			youcansendmessage=1;
		}
		else
		{
			SEND[0]='n';
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
		}
	}
	if(ACCEPT[0]=='a')
	{//�ĵ��Է��Ѿ���ʼ����Ϣ
		AfxMessageBox("The Other player has already begin with others!Failed");
	}
	if(ACCEPT[0]=='n')
	{//�õ��Է��ܾ���Ϣ
		AfxMessageBox("The Other player refused!");
	}
	if(ACCEPT[0]=='5')
	{//�ĵ��Է�ͬ����Ϣ
		all.clear();black.nCountkey=0;white.nCountkey=0;				Invalidate();

		connect=1;
		youcansendmessage=0;
		whoiam=-1;
	}
/*	if(ACCEPT[0]=='6')
	{//������
		whoiam=-whoiam;
	}*/
	if(ACCEPT[0]=='!')
	{//�Է�ȡ��������
		connect=0;
		AfxMessageBox("The other user disconnected!you can choose restart game from the menu to restart!");

	}
	if(ACCEPT[0]=='9')
	{//�Է������˽���
		OnRuleChange();
	}
	if(ACCEPT[0]=='3')//him black down,the same as OnLeftButtonDown
	{
		int x,y,who;
		x=ACCEPT[2];y=ACCEPT[3];who=1;whohasjinshou=1;		
		if(all.allqipan[x][y]==0)
		{		
			MessageBeep(MB_OK);

			black.keydown(x,y,who);
			all.allqipan[x][y]=1;			
			if(all.shuangsi(x,y)==1&&whohasjinshou==1)
			{
				Invalidate();
				resultwin lost;
				lost.m_Who.Format("black lost!double 4!");
				lost.DoModal();
				OnRuleRestart();youcansendmessage=0;whoiam=-1;
				return 0;
			}

			int win=all.winchanglian(x,y);
			if(win==1)
			{
				Invalidate();
				resultwin win;
				win.m_Who.Format("black win!");
				win.DoModal();
				OnRuleRestart();youcansendmessage=1;whoiam=1;
				return 0;	
			}		
			if(win==0&&whohasjinshou==1)
			{
				Invalidate();
				resultwin lost;
				lost.m_Who.Format("black lost!long!");
				lost.DoModal();
				OnRuleRestart();youcansendmessage=0;whoiam=-1;
				return 0;
			}

			if(all.shuangsan(x,y)==1&&whohasjinshou==1) 
			{
				Invalidate();
				resultwin fail;
				fail.m_Who.Format("Black lost!double 3!");
				fail.DoModal();
				OnRuleRestart();youcansendmessage=0;whoiam=-1;
				return 0;		
			}
		}
	
	youcansendmessage=1;
	Invalidate();
	//SEND MESSAGE

	}
	if(ACCEPT[0]=='4')//him white down
	{
		int x,y,who;
		x=ACCEPT[2];y=ACCEPT[3];who=-1;whohasjinshou=-1;		
		if(all.allqipan[x][y]==0)
		{		
					MessageBeep(MB_OK);

			white.keydown(x,y,who);
			all.allqipan[x][y]=-1;			
			if(all.shuangsi(x,y)==1&&whohasjinshou==1)
			{
				Invalidate();
				resultwin lost;
				lost.m_Who.Format("white lost!double 4!");
				lost.DoModal();
				OnRuleRestart();youcansendmessage=0;whoiam=-1;
				return 0;
			}

			int win=all.winchanglian(x,y);
			if(win==1)
			{
				Invalidate();
				resultwin win;
				win.m_Who.Format("white win!");
				win.DoModal();
				OnRuleRestart();youcansendmessage=1;whoiam=1;
				return 0;	
			}		
			if(win==0&&whohasjinshou==1)
			{
				Invalidate();
				resultwin lost;
				lost.m_Who.Format("white lost!long!");
				lost.DoModal();
				OnRuleRestart();youcansendmessage=0;whoiam=-1;
				return 0;
			}

			if(all.shuangsan(x,y)==1&&whohasjinshou==1) 
			{
				Invalidate();
				resultwin fail;
				fail.m_Who.Format("white lost!double 3!");
				fail.DoModal();
				OnRuleRestart();youcansendmessage=0;whoiam=-1;
				return 0;
			}
		}
	youcansendmessage=1;
	Invalidate();
	//SEND MESSAGE
	}
	

	return 0;
}
UINT ServerThreadProc(LPVOID pParam)
{//����������
//	::PostMessage((HWND)pParam,WM_CONNECT,0,0);		
	CBlockingSocket sConnect;		 		 

	try
	{
		if(!g_sListen.Accept(sConnect,saClient))
		{
			return 0;
		}
		CONADDR=saClient.DottedDecimal();

		AfxBeginThread(ServerThreadProc, pParam, THREAD_PRIORITY_NORMAL);
	/*	if(CONADDR==INADDR_ANY)
		{������ӵ��Լ�,���ǲ��ɹ�,��û�Ǹ�bug
			AfxMessageBox("you cant't connect to yourself!");
			sConnect.Close();
			return 0;
		}*/

		sConnect.Receive(ACCEPT,8,10);
		::SendMessage((HWND)pParam,WM_CONNECT,0,0);		
		sConnect.Close();
		
	}
	catch(CBlockingSocketException* e)
	{
		e->Delete();
		AfxMessageBox("Connect problem!you can choose restart game from the menu to restart!");
		CFrameWnd* Fram=(CFrameWnd*)AfxGetApp()->m_pMainWnd;
		CWuziqiView* v=(CWuziqiView*)Fram->GetActiveView();
		v->connect=0;
		
	}
	return 0;
}

UINT ClientSocketThreadProc(LPVOID pParam)
 {//������Ϣ�Ľ���
	CBlockingSocket sClient;
	 try
	 {

		 CSockAddr saServer;//(SERVER,84);
		 saServer=CBlockingSocket::GetHostByName(SERVER,84);
		 
		 sClient.Create();
		 //sClient.Bind(saServer);			 
		 sClient.Connect(saServer);
		 sClient.Write(SEND,8,10);
		 sClient.Close();

	 }
	 catch(CBlockingSocketException* e)
	 {
		 e->Delete();		 
		AfxMessageBox("Connect problem!you can choose restart game from the menu to restart!");

			CFrameWnd* Fram=(CFrameWnd*)AfxGetApp()->m_pMainWnd;
		CWuziqiView* v=(CWuziqiView*)Fram->GetActiveView();
		v->connect=0;
	 }
	 return 0;
 }

void CWuziqiView::OnConnect() 
{//��������
if(connect!=1)
	{
		connectto to;
		to.m_where="172.16.1.135";
		int n=to.DoModal();
		if(n==IDOK)
		{
			SERVER=to.m_where;	
			SEND[0]='1';	
			AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
		}
	}
	else
	{//�����ǰ�л����
		AfxMessageBox("You must disconnect first!");
	}
	
}

void CWuziqiView::OnStartserver() 
{//��ʼ����,�Զ���ʼ,û��д�����жϵĳ���
	if(!startserver)
	{
		try
		{
			CSockAddr saServer(INADDR_ANY,84);
			g_sListen.Create();
			g_sListen.Bind(saServer);
			g_sListen.Listen();
			AfxBeginThread(ServerThreadProc, GetSafeHwnd());
			startserver=1;

		}
		catch(CBlockingSocketException* e)
		{
		AfxMessageBox("Connect problem!you can choose restart game from the menu to restart!");
			g_sListen.Cleanup();
			e->Delete();
			startserver=0;
			CFrameWnd* Fram=(CFrameWnd*)AfxGetApp()->m_pMainWnd;
		CWuziqiView* v=(CWuziqiView*)Fram->GetActiveView();
		v->connect=1;
		}
	}
		
}

void CWuziqiView::OnUpdateStartserver(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(startserver);	
}

void CWuziqiView::OnDisconnect() 
{//ȡ������
	SEND[0]='!';
	AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
	connect=0;
	AfxMessageBox("disconnected!you can choose restart game from the menu to restart!");

	//OnRuleRestart();
}

void CWuziqiView::OnUpdateDisconnect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(connect);
}

void CWuziqiView::OnUpdateRuleRestart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!connect);	
}



void CWuziqiView::OnFileOpen() 
{//��һ�������ļ�
	CFileDialog dlg(TRUE,NULL,"*.wzq",NULL,"wuziqi Files (*.wzq)");
	if(dlg.DoModal()==IDOK)
	{
		char temp1[1],temp2[3];
		int nCountx,nCounty;
		CStdioFile file;
		if(!file.Open(dlg.GetPathName(),CFile::modeRead|CFile::typeText))
		{
			AfxMessageBox("Can't open file!");
			return;
		}	
		for(nCountx=0;nCountx<=17;nCountx++)
		{
			for(nCounty=0;nCounty<=17;nCounty++)
			{
				file.Read(temp1,1);				
				if(temp1[0]=='0') all.allqipan[nCountx][nCounty]=0;
				if(temp1[0]=='1') all.allqipan[nCountx][nCounty]=1;
				if(temp1[0]=='!') all.allqipan[nCountx][nCounty]=-1;
				if(temp1[0]!='0'&&temp1[0]!='1'&&temp1[0]!='!')
				{
					AfxMessageBox("File error!");
					OnRuleRestart();
					return;
				}
			}
			file.Read(temp1,1);
		}
		file.Read(temp2,3);
		who=temp2[0]-1;black.nCountkey=temp2[1];white.nCountkey=temp2[2];
		if(who!=1&&who!=-1||(temp2[1]-temp2[2])*(temp2[1]-temp2[2])>1)
		{
	
					AfxMessageBox("File error!");
					OnRuleRestart();
					return;
		}
		Invalidate();
	}

}
 

void CWuziqiView::OnFileSave() 
{//���������ļ�
	/*if(connect==1)
	{
		AfxMessageBox("You must disconnect first!");
		return;
	}*/
	CFileDialog dlg(FALSE,NULL,"*.wzq",NULL,"wuziqi Files (*.wzq)");
	if(dlg.DoModal()==IDOK)
	{
		char temp1[1],temp2[3];
		int nCountx,nCounty;
		CStdioFile file;
		if(!file.Open(dlg.GetPathName(),CFile::modeWrite|CFile::typeText))
		{			
			AfxMessageBox("Can't open file!");
			return;
		}	
		for(nCountx=0;nCountx<=17;nCountx++)
		{
			for(nCounty=0;nCounty<=17;nCounty++)
			{
				temp1[0]=all.allqipan[nCountx][nCounty]+'0';
				if(temp1[0]==-1+'0')temp1[0]='!';
				file.Write(temp1,1);
			}
			temp1[0]='\n';
			file.Write(temp1,1);
		}
		temp2[0]=who+1;temp2[1]=black.nCountkey;temp2[2]=white.nCountkey;
		file.Write(temp2,3);
	}

}

void CWuziqiView::OnFinalRelease() 
{

	if(connect==1)
	{
			SEND[0]='!';
	AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());

	}
	CView::OnFinalRelease();
}

void CWuziqiView::OnDestroy() 
{
	if(connect==1)
	{
			SEND[0]='!';
	AfxBeginThread(ClientSocketThreadProc, GetSafeHwnd());
	}
Sleep(2000);
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
