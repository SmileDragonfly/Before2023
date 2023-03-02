
// RadarProjectDlg.h : header file
//

#pragma once
#include "GraphicView.h"
#include "TestData.h"

// CRadarProjectDlg dialog
class CRadarProjectDlg : public CDialogEx
{
// Construction
public:
	CRadarProjectDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RADARPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CGraphicView m_graphicView;
    CTestData m_testData;

    afx_msg void OnTimer(UINT_PTR nIDEvent);
    
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedButtonTest();
};
