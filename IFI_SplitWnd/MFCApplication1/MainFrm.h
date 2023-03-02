
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "MySplitterWnd.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    CRect               GetLeftFrameSize();
    CRect               GetRightFrameSize();

protected:  // control bar embedded members
	CToolBar            m_wndToolBar;
    CWnd*               m_leftFrameWnd;
    CWnd*               m_rightFrameWnd;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

    CMySplitterWnd m_wndSplitter;
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


