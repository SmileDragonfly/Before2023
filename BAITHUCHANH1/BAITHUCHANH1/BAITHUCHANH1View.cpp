
// BAITHUCHANH1View.cpp : implementation of the CBAITHUCHANH1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BAITHUCHANH1.h"
#endif

#include "BAITHUCHANH1Doc.h"
#include "BAITHUCHANH1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBAITHUCHANH1View

IMPLEMENT_DYNCREATE(CBAITHUCHANH1View, CView)

BEGIN_MESSAGE_MAP(CBAITHUCHANH1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBAITHUCHANH1View construction/destruction

CBAITHUCHANH1View::CBAITHUCHANH1View()
{
	// TODO: add construction code here

}

CBAITHUCHANH1View::~CBAITHUCHANH1View()
{
}

BOOL CBAITHUCHANH1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBAITHUCHANH1View drawing

void CBAITHUCHANH1View::OnDraw(CDC* /*pDC*/)
{
	CBAITHUCHANH1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CBAITHUCHANH1View printing

BOOL CBAITHUCHANH1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBAITHUCHANH1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBAITHUCHANH1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBAITHUCHANH1View diagnostics

#ifdef _DEBUG
void CBAITHUCHANH1View::AssertValid() const
{
	CView::AssertValid();
}

void CBAITHUCHANH1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBAITHUCHANH1Doc* CBAITHUCHANH1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBAITHUCHANH1Doc)));
	return (CBAITHUCHANH1Doc*)m_pDocument;
}
#endif //_DEBUG


// CBAITHUCHANH1View message handlers
