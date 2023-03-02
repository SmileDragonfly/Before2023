//////////////////////////////////////////////////////////////////////////////////////////////////
// File: IFIPNGButton.h																			//
// Class: CIFIPNGButton																			//
// Description: Define of class CIFIPNGButton													//
// Date: 2018/08																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CIFIPNGButton_H
#define _CIFIPNGButton_H
#pragma once

class CIFIPNGButton : public CButton
#endif
{
	CBitmap m_bgrBitmap;
	DECLARE_DYNAMIC(CIFIPNGButton)
protected:	
	DWORD		m_dwTextAlign;
	COLORREF	m_clrText;
	CString		m_strToolTip;
	BOOL		m_bLMouseDown;
	int			m_nCheck;
	BOOL		m_bIsCheckBox;
	void*		m_pBitmap;
	void*		m_pBitmapChecked;
	void*		m_pCurrBitmap;
	INT			m_iExtraValue;
public:
	CIFIPNGButton();
	virtual ~CIFIPNGButton();
	void SetText(CString sText, DWORD dwTextAlign);
	void SetTextAlign(DWORD dwTextAlign);
	void SetTextColor(COLORREF clrText);
	void SetPNG(UINT uIDResource, HINSTANCE hInstance = NULL);
	void SetPNG(CString strPngPath);
	void SetCheckedPNG(UINT uIDResource, HINSTANCE hInstance = NULL);
	void SetCheckedPNG(CString strPngPath);


	void SetToolTipText(CString strToolTip);
	int  GetCheck();
	void SetCheck(int nCheck);
	void SetCheckBoxStyle(BOOL bCheckbox);
	void PerformClicked();
	INT GetExtraValue();
	void SetExtraValue(INT iNewValue);

// Overrides
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Generated message map functions
protected:
	void RebuildSkin();
	//{{AFX_MSG(CIFIPNGButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};
