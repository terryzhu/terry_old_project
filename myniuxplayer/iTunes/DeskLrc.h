#pragma once


// CDeskLrc 对话框

class CDeskLrc : public CDialog
{
	DECLARE_DYNAMIC(CDeskLrc)

public:
	CDeskLrc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeskLrc();
	void UpdateLayered();
	void SmoothDrawTextEx(LPTSTR lpszText,CRect rc,COLORREF clr1,COLORREF clr2,UINT nFontSize=14,DWORD dwFlag=DT_CENTER|DT_VCENTER|DT_SINGLELINE,CFont* pFont=NULL);
	void SmoothShadow(LPTSTR lpszText,CRect rc,COLORREF clrText,UINT nFontSize=14,DWORD dwFlag=DT_CENTER|DT_VCENTER|DT_SINGLELINE,CFont* pFont=NULL);
	void SetLyric(LPTSTR lpszText);

	HBITMAP m_hmemBmp;
	BYTE* m_pBits;
	DWORD m_dwSize;
	DWORD m_dwEffWidth;
	BOOL  m_bEscape;
	CRect m_rect;
	TCHAR m_szText[260];

//	LPTSTR m_arrLyric[5];
// 对话框数据
	enum { IDD = IDD_DESKLRC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
