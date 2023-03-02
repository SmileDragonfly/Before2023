
// VirtualMemoryView.h : interface of the CVirtualMemoryView class
//

#pragma once


class CVirtualMemoryView : public CView
{
protected: // create from serialization only
	CVirtualMemoryView() noexcept;
	DECLARE_DYNCREATE(CVirtualMemoryView)

// Attributes
public:
	CVirtualMemoryDoc* GetDocument() const;

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
	virtual ~CVirtualMemoryView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VirtualMemoryView.cpp
inline CVirtualMemoryDoc* CVirtualMemoryView::GetDocument() const
   { return reinterpret_cast<CVirtualMemoryDoc*>(m_pDocument); }
#endif

