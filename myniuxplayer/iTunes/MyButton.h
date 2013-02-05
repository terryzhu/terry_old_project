#pragma once
//#include "iTunesDlg.h"
class CiTunesDlg;
// CMyButton
typedef void (CiTunesDlg::*pTypeFnLBtnUp)();
class CMyButton : public CWnd
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton(UINT normal_id,UINT move_id,UINT press_id
		,pTypeFnLBtnUp pFnLBtnUp,CiTunesDlg * pDlg);
	virtual ~CMyButton();
	void FillNormal();
	void FillMove();
	void FillPress();
	
protected:
	pTypeFnLBtnUp m_pFnLBtnUp;
	UINT m_normal_id;
	UINT m_move_id;
	UINT m_press_id;
	CiTunesDlg *m_pDlg;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


