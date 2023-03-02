
// VirtualMemoryView.cpp : implementation of the CVirtualMemoryView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "VirtualMemory.h"
#endif

#include "VirtualMemoryDoc.h"
#include "VirtualMemoryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVirtualMemoryView

IMPLEMENT_DYNCREATE(CVirtualMemoryView, CView)

BEGIN_MESSAGE_MAP(CVirtualMemoryView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CVirtualMemoryView construction/destruction

CVirtualMemoryView::CVirtualMemoryView() noexcept
{
	// TODO: add construction code here

}

CVirtualMemoryView::~CVirtualMemoryView()
{
}

BOOL CVirtualMemoryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVirtualMemoryView drawing

void CVirtualMemoryView::OnDraw(CDC* /*pDC*/)
{
	CVirtualMemoryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CVirtualMemoryView printing

BOOL CVirtualMemoryView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVirtualMemoryView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVirtualMemoryView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CVirtualMemoryView diagnostics

#ifdef _DEBUG
void CVirtualMemoryView::AssertValid() const
{
	CView::AssertValid();
}

void CVirtualMemoryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVirtualMemoryDoc* CVirtualMemoryView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVirtualMemoryDoc)));
	return (CVirtualMemoryDoc*)m_pDocument;
}
#endif //_DEBUG


// CVirtualMemoryView message handlers
