
// MFCApplication1Doc.cpp : implementation of the CMFCApplication1Doc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication1Doc

IMPLEMENT_DYNCREATE(CMFCApplication1Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication1Doc, CDocument)
END_MESSAGE_MAP()


// CMFCApplication1Doc construction/destruction

CMFCApplication1Doc::CMFCApplication1Doc() noexcept
{
	// TODO: add one-time construction code here
    m_strData = L"Hello world";
}

CMFCApplication1Doc::~CMFCApplication1Doc()
{
}

BOOL CMFCApplication1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMFCApplication1Doc serialization

void CMFCApplication1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCApplication1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCApplication1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCApplication1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCApplication1Doc diagnostics

#ifdef _DEBUG
void CMFCApplication1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
CString CMFCApplication1Doc::GetData()
{
    return m_strData;
}
void CMFCApplication1Doc::SetData(CString strInput)
{
    m_strData = strInput;
}
#endif //_DEBUG


// CMFCApplication1Doc commands


// Split left view to n*m 
BOOL CMFCApplication1Doc::GridLeftView(unsigned int row, unsigned int column)
{
    m_row = row;
    m_column = column;
    CSize itemSize((m_rectLeftFrame.right - m_rectLeftFrame.left) / column, (m_rectLeftFrame.bottom - m_rectLeftFrame.top) / row);

    for (int i = 0; i < (row*column); i++)
    {
        CGraphicItem* item = new CGraphicItem();
        unsigned int col = i % column;
        unsigned int row = (unsigned int)i / column;
        item->m_rect.left = col * itemSize.cx;
        item->m_rect.right = item->m_rect.left + itemSize.cx;
        item->m_rect.top = row * itemSize.cy;
        item->m_rect.bottom = item->m_rect.top + itemSize.cy;
        m_arrGraphicItem.Add(item);
    }

    return 0;
}

void CMFCApplication1Doc::UpdateLeftFrameRect(CRect rc)
{
    m_rectLeftFrame = rc;
}

void CMFCApplication1Doc::UpdateData()
{
    UpdateAllViews(NULL);
}

unsigned int CMFCApplication1Doc::GetLeftFrameRow()
{
    return m_row;
}

unsigned int CMFCApplication1Doc::GetLefttFrameColumn()
{
    return m_column;
}
