#pragma once


// CLrcWnd 对话框

class CLrcWnd : public CDialog
{
	DECLARE_DYNAMIC(CLrcWnd)

public:
	CLrcWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLrcWnd();

// 对话框数据
	enum { IDD = IDD_LRCWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();

	float ipast;
	int m_alllength;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
