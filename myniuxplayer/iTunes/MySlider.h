#pragma once
#ifndef WM_MYSLIDER
#define WM_MYSLIDER (WM_USER+0x76)
#endif

// CMySlider

class CMySlider : public CWnd
{
	DECLARE_DYNAMIC(CMySlider)

public:
	CMySlider();
	virtual ~CMySlider();
	void SetProgress(double process);
	BOOL m_LeftBtnDown;
protected:
	CBitmap m_roundSlider;
	double m_iProgress;
	int m_DragProgress;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


