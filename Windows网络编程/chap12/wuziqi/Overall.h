// Overall.h: interface for the Overall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERALL_H__5AB8BE54_13D9_4705_A701_978D94B71083__INCLUDED_)
#define AFX_OVERALL_H__5AB8BE54_13D9_4705_A701_978D94B71083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//������ӵ�е���
class Overall  
{
public:
	Overall();
	virtual ~Overall();
	int allqipan[18][18];//�������ε�����
	int qipantemp[18][18];//���ߵ�����
	int tempjustxy;//see .cpp blow
	void cleartemp();
	void clear();
	int winchanglian(int x,int y);
	int huosi(int x,int y);
	int shuangsan(int x,int y);
	int shuangsi(int x,int y);

};

#endif // !defined(AFX_OVERALL_H__5AB8BE54_13D9_4705_A701_978D94B71083__INCLUDED_)
