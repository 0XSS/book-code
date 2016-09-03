// MagicHouseView.h : CMagicHouseView ��Ľӿ�
//

#pragma once

class CMagicHouseView : public CView
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CMagicHouseView)

public:
	BYTE*	m_pImageBuffer;				// �༭ͼ��ԭʼ��������
	BYTE*	m_pImageTempBuffer;			// ��������������
	UINT	m_nPicWidth;				// ��ǰ�༭ͼ����
	UINT	m_nPicHeight;				// ��ǰ�༭ͼ��߶�
	UINT	m_nTempWidth;				// �����ͼ��Ŀ��
	UINT	m_nTempHeight;				// �����ͼ��ĸ߶�


	bool	m_bWidhtOut;				// ��ʾͼ���Ƿ񳬳���ͼ���
	bool	m_bHeightOut;				// ��ʾͼ���Ƿ񳬳���ͼ�߶�
	bool	m_bIsEditMode;				// ��ǰ�Ƿ�Ϊ�༭ģʽ
	int		m_nShowType;				// ��ʾ����

	virtual ~CMagicHouseView();

	void ShowPicture(CDC* pDC, Image& image, int nShowType);
										// ��ָ��ģʽ��ʾͼ��
	void SetShowType(int nType);		// ������ʾģʽ
	CSize GetShowPicSize(Image& image, int nShowType);
										// ���ָ��ģʽ��ͼ��Ĵ�С

	void OpenFile(const CString& strPath);	// ��ͼ���ļ�

	//----------------------------------
	//	����:		���ͼ���ƫ������
	//----------------------------------
	void ClearOffset(void)
	{
		m_nXX = m_nYY = m_nXXMax = m_nYYMax = 0;
	}

	void FixOffset(void);				// ����ͼ���ƫ������

	//----------------------------------
	//	����:		���ָ�������ļ���ȫ·��
	//----------------------------------
	CString GetFilePath(int nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < m_nPicNum);
		return m_strPath + L"\\" + m_FilesNameAry[nIndex];
	}

	void RefreshIndicator();			// ����״̬����ָʾ����Ϣ

	CMagicHouseDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);		// ��д�Ի��Ƹ���ͼ

	// ��Ϣ��Ӧ����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
										// ����ƶ�����Ӧ����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
										// ��������������Ӧ����
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
										// ���������
	afx_msg void OnFileOpen();			// "��"��������Ӧ����
	afx_msg void OnFileSave();			// "����"��������Ӧ����
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);// ��ֹ�ػ汳��

	afx_msg void OnModeBrowse();		// ���ģʽ
	afx_msg void OnModeEdit();			// �༭ģʽ
	
	// "ģʽ"���º���
	afx_msg void OnUpdateModeBrowse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeEdit(CCmdUI *pCmdUI);

	// "��ʾģʽ"�˵���غ���
	afx_msg void OnModeNormal();
	afx_msg void OnModeWidth();
	afx_msg void OnModeHeight();
	afx_msg void OnModeScreen();
	afx_msg void OnUpdateModeNormal(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeWidth(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeHeight(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeScreen(CCmdUI *pCmdUI);
	// "��ͼ"��"ͼ��"�˵���غ���
	afx_msg void OnImagePrev();
	afx_msg void OnImageNext();
	afx_msg void OnImageFirst();
	afx_msg void OnImageLast();
	afx_msg void OnUpdateImagePrev(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageNext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageFirst(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageLast(CCmdUI *pCmdUI);
	// "��ʾ��Ч"�˵���غ���
	afx_msg void OnEffectNone();
	afx_msg void OnEffectRand();
	afx_msg void OnEffectScan();
	afx_msg void OnEffectDscan();
	afx_msg void OnEffectMove();
	afx_msg void OnEffectVBlind();
	afx_msg void OnEffectHRaster();
	afx_msg void OnEffectMosaic();
	afx_msg void OnEffectHsmove();
	afx_msg void OnEffectSblind();
	afx_msg void OnEffectVraster();
	afx_msg void OnEffectRaindrop();
	afx_msg void OnUpdateEffectNone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectScan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectDscan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectMove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectBlind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRaster(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectMosaic(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectHsmove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectSblind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectVraster(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectRaindrop(CCmdUI *pCmdUI);

	// �˾�
	afx_msg void OnFilterNegative();
	afx_msg void OnFilterEmboss();
	afx_msg void OnFilterBw();
	afx_msg void OnFilterFog();
	afx_msg void OnFilterMosaic();
	afx_msg void OnFilterSketch();

protected:
	CMagicHouseView();

	virtual void OnInitialUpdate();		// ���º���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	int				m_nEffectDisplayType;// ��ǰ��Ч����
	bool			m_bEffectDraw;		// �Ƿ�ʹ����Ч��ʾ
	Bitmap*			m_pBitmap;			// ˫�����ͼ
	int				m_nPicNum;			// ��ǰĿ¼���ļ�����
	CString			m_strPath;			// ��ǰ��Ŀ¼
	CStringArray	m_FilesNameAry;		// ���浱ǰĿ¼�������ļ���������
	int				m_nPos;				// ��ǰ�ļ��������е�����
	int				m_nShowPicHeight;	// ��ʾͼƬ�ĸ߶�
	int				m_nShowPicWidth;	// ��ʾͼƬ�Ŀ��

	CPoint			m_psMove;			// ��¼�ƶ�ǰ����λ��
	int				m_nXX;				// ͼƬX�����ƫ����
	int				m_nYY;				// ͼƬY�����ƫ����
	int				m_nXXMax;			// X�������ƫ����
	int				m_nYYMax;			// Y�������ƫ����

	void PreDrawImage(void);			// ���ڴ���Ԥ�Ȼ���ͼ��
	void EffectDisplayImage(CDC* pDC, CDC* pMemDC);
										// ʹ����Ч��ʾͼ��
	bool GetImageBuffer(CString& strPath);
										// ��ָ��·����ͼ����Ϣ��ȡ��������
	void FreeImageBuffer(void);			// �ͷ�ͼ����Ϣռ�õ��ڴ�

	void WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname);
										// ��ͼ����Ϣд��bmp�ļ���
	void ResetImage();					// ����m_pImageTempBuffer����Ϣ

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#ifndef _DEBUG  // MagicHouseView.cpp �еĵ��԰汾
inline CMagicHouseDoc* CMagicHouseView::GetDocument() const
   { return reinterpret_cast<CMagicHouseDoc*>(m_pDocument); }
#endif

