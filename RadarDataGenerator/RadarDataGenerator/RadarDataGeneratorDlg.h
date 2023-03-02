
// RadarDataGeneratorDlg.h : header file
//

#pragma once
#include "CreateData.h"

// CRadarDataGeneratorDlg dialog
class CRadarDataGeneratorDlg : public CDialogEx
{
// Construction
public:
	CRadarDataGeneratorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RADARDATAGENERATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
    CArray<CPoint> m_arrPoint;
    CCreateData m_createData;
    CPoint m_circleCenter;
    CArray<TData> m_data;
    int m_currentDrawPoint;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedOk();
};
