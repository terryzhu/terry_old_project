#pragma once


// CMovingLrcDlg �Ի���

class CMovingLrcDlg : public CDialog
{
	DECLARE_DYNAMIC(CMovingLrcDlg)

public:
	CMovingLrcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMovingLrcDlg();
	float ipast;
	CFont m_Font;
// �Ի�������
	enum { IDD = IDD_MOVINGLRC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
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
