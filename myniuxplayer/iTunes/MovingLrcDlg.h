#pragma once


// CMovingLrcDlg 对话框

class CMovingLrcDlg : public CDialog
{
	DECLARE_DYNAMIC(CMovingLrcDlg)

public:
	CMovingLrcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMovingLrcDlg();
	float ipast;
	CFont m_Font;
// 对话框数据
	enum { IDD = IDD_MOVINGLRC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int CalLrcWidthPix(int Num=-1);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
