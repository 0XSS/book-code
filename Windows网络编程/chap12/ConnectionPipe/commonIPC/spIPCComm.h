#ifndef _SP_COMM_H_
#define _SP_COMM_H_

#include <afxsock.h>

//#include "winsock.h"

//�����������
//�ɹ�
#define SP_ERR_SUCCESS			0
//�������
#define SP_ERR_NETWORK			1
//û����ɶ���д,ֻ�����д������Ҫ��Ҫ������
#define SP_ERR_NOT_FINISH		2
//����ʱ��
#define SP_ERR_TIMEOUT			3
//������
#define SP_ERR_NODATA			4
//δʵ�ָõ���
#define SP_ERR_NOT_IMPLEMENT	98
//һ�����
#define SP_ERR_GENERAL			99


//pszLocalAddrΪ������ʹ�õ�IP��ַ,���ΪNULL,����ʹ������IP��ַ,ϵͳ�Զ�ѡ��
//pszLocalAddr��Ҫ�󶨵ı��ص�ַ�����ΪNULL�������е�ַ
//����INVALID_SOCKET����ʧ��
SOCKET SPCreateListenSocket(LPCSTR pszLocalAddr,UINT uPort);
//�����ͻ���SOCKET
//pszLocalAddr��Ҫ�󶨵ı��ص�ַ�����ΪNULL�������е�ַ
//����INVALID_SOCKET����ʧ��
SOCKET SPCreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr=NULL);

class CIPCComm
{
public :
	CIPCComm(){};
	~CIPCComm(){};
public:

	//��:ֱ����ȫ�������ݣ����Ǵ�����
	//���أ�SP_ERR_SUCCESS��SP_ERR_NETWORK
	//iWant:��Ҫ������ֽ���
	//iRead:����ʵ�ʶ�����ֽ���
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead)=0;

	//д:ֱ����ȫд�����ݣ����Ǵ�����
	//���أ�SP_ERR_SUCCESS��SP_ERR_NETWORK
	//iWant:��Ҫд�����ֽ���
	//iWrote:����ʵ��д�����ֽ���
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote)=0;

	//�����Ƿ������ݵ���
	//���أ�SP_ERR_SUCCESS(�����ݵ���)��SP_ERR_NETWORK��SP_ERR_NODATA
	//iTimeOut: ��ʱʱ�䣬��
	virtual int TestRead(int iTimeOut)=0;

	//�����Ƿ������ӵ���
	//���أ�SP_ERR_SUCCESS(�����ݵ���)��SP_ERR_NETWORK��SP_ERR_NODATA�������ӵ��
	//iTimeOut: ��ʱʱ�䣬��
	virtual int TestConnect(int iTimeOut)=0;

	//�ڹ涨ʱ���ڶ���Ҫ������������,�����ܶ�Ķ�������
	//����SP_ERR_SUCCESS��SP_ERR_NETWORK��SP_ERR_NOT_FINISH����ʱ��ֻ���벿�����ݣ���SP_ERR_NODATA��δ�����κ����ݣ�
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)=0;

	//������ݲ����룬�ڼ�⵽���ݺ�ֻ����һ�ζ�������
	//���أ�SP_ERR_SUCCESS������Ҫ�����ݣ���SP_ERR_NETWORK��SP_ERR_NOT_FINISH����⵽���ݣ���ֻ���벿�����ݣ���SP_ERR_NODATA����ʱ��δ�����κ����ݣ�
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)=0;
};

class CIPCSocket: public CIPCComm
{
public:
	//fAutoDel �Ƿ���ɾ������ʱ�Զ��ر�Socket
	CIPCSocket(SOCKET sockComm,BOOL fAutoDel=FALSE);
	~CIPCSocket();
public:
	//����Socket helper function
	static SOCKET CreateListenSocket(LPCSTR pszLocalAddr,UINT uPort)
	{
		return SPCreateListenSocket(pszLocalAddr,uPort);
	}
	static SOCKET CreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr)
	{
		return SPCreateConnectSocket( pszRemoteAddr, iPort, pszLocalAddr);
	}
	//
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead);
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote);
	virtual int TestRead(int iTimeOut);
	virtual int TestConnect(int iTimeOut);
	static int TestRead(SOCKET sock,int iTimeOut)
	{
		CIPCSocket sTemp(sock,FALSE);
		return sTemp.TestRead(iTimeOut);
	};
	static int TestConnect(SOCKET sock,int iTimeOut)
	{
		CIPCSocket sTemp(sock,FALSE);
		return sTemp.TestConnect(iTimeOut);
	};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
protected:
	SOCKET m_sockComm;
	BOOL m_fAutoDel;
};

//pszPipeName \\.\pipe\pipe_name
//���� INVALID_HANDLE_VALUE ����ʧ��
//�����������˹ܵ�,���ȴ�����
//iTimeout Ϊ�ȴ����ӵĳ�ʱ�г����������
HANDLE SPCreateServerPipe(LPCSTR pszPipeName,int iTimeout=10);
//�����ͻ��˹ܵ��������ӵ���������
//pszPipeName \\server\pipe\pipe_name
//iTimeout Ϊ��ʱʱ��
HANDLE SPCreateClientPipe(LPCSTR pszPipeName,int iTimeout=10);

class CIPCNamedPipe: public CIPCComm
{
public:
	//fAutoDel �Ƿ���ɾ������ʱ�Զ��ر�Socket
	CIPCNamedPipe(HANDLE hPipe,BOOL fAutoDel=FALSE);
	~CIPCNamedPipe();
public:
	static HANDLE CreateServerPipe(LPCSTR pszPipeName,int iTimeout=10)
	{
		return SPCreateServerPipe(pszPipeName,iTimeout);
	};
	static HANDLE CreateClientPipe(LPCSTR pszPipeName,int iTimeout=10)
	{
		return SPCreateClientPipe(pszPipeName,iTimeout);
	};

	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead);
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote);
	virtual int TestRead(int iTimeOut);
	virtual int TestConnect(int iTimeOut);
	static int TestRead(HANDLE hPipe,int iTimeOut)
	{
		CIPCNamedPipe sTemp(hPipe,FALSE);
		return sTemp.TestRead(iTimeOut);
	};
	static int TestConnect(HANDLE hPipe,int iTimeOut)
	{
		CIPCNamedPipe sTemp(hPipe,FALSE);
		return sTemp.TestConnect(iTimeOut);
	};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
protected:
	HANDLE m_hPipe;
	BOOL m_fAutoDel;
};

#endif

