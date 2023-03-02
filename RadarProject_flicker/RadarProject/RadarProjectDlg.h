
// RadarProjectDlg.h : header file
//

#pragma once
#include "GraphicView.h"
#include "AmplitudeView.h"
#include "TestData.h"
#include "IFIPNGButton.h"
#include "IFIButton.h"
#include "RadarInput.h"

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
public:
    // Variables
    CGraphicView m_graphicView;
    CAmplitudeView m_amplitudeView;

protected:
    // Variables
	HICON m_hIcon;
    CIFIButton m_buttonZoomIn;
    CIFIButton m_buttonZoomOut;
    CTestData m_testData;
    CRadarInput m_radarInput;
    FILE* m_pFile;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnTimer(UINT_PTR nIDEvent);
    
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedButtonTest();
    afx_msg void OnBnClickedButtonZoomIn();
    afx_msg void OnBnClickedButtonZoomOut();
};
