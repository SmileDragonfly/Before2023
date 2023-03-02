
// BAITHUCHANH1View.h : interface of the CBAITHUCHANH1View class
//

#pragma once


class CBAITHUCHANH1View : public CView
{
protected: // create from serialization only
	CBAITHUCHANH1View();
	DECLARE_DYNCREATE(CBAITHUCHANH1View)

// Attributes
public:
	CBAITHUCHANH1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBAITHUCHANH1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BAITHUCHANH1View.cpp
inline CBAITHUCHANH1Doc* CBAITHUCHANH1View::GetDocument() const
   { return reinterpret_cast<CBAITHUCHANH1Doc*>(m_pDocument); }
#endif

