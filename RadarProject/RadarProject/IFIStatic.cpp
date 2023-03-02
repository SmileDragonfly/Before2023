//////////////////////////////////////////////////////////////////////////////////////////////////
// File: IFIStatic.cpp																			//
// Class: CIFIStatic																			//
// Description: Implement of class CIFIStatic													//
// Date: 2018/11																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GdiPlusUtils.h"
#include "IFIStatic.h"
#include "IFISkinSetting.h"

CIFIStatic::CSkinProperties::CSkinProperties(CWnd* pWndOwner)
: CIFISkinProperties(pWndOwner)
, m_nRoundCornerRadius(0)
, m_nPaddingLeft(0)
, m_nPaddingTop(0)
, m_nPaddingRight(0)
, m_nPaddingBottom(0)
{

}
CIFIStatic::CSkinProperties::~CSkinProperties()
{
}
void CIFIStatic::CSkinProperties::SetRoundCornerRadius(UINT nRadius)
{
	m_nRoundCornerRadius = nRadius;
	if(!m_bLockWindowsInvalidate && m_pWndOwner && m_pWndOwner->GetSafeHwnd())
	{
		m_pWndOwner->Invalidate();
	}
}

void CIFIStatic::CSkinProperties::SetPadding(UINT left, UINT top, UINT right, UINT bottom)
{
	m_nPaddingLeft		= left;
	m_nPaddingTop		= top;
	m_nPaddingRight		= right;
	m_nPaddingBottom	= bottom;
}

IMPLEMENT_DYNAMIC(CIFIStatic, CStatic)

CIFIStatic::CIFIStatic(void)
: CStatic()
, m_SkinProperties(this)
, m_SkinType(CIFIStatic::Label)
{
	SetSkinType(CIFIStatic::Label);
}


CIFIStatic::~CIFIStatic(void)
{
	if(m_bgrBitmap.GetSafeHandle())
	{
		m_bgrBitmap.DeleteObject();
	}
}

void CIFIStatic::SetSkinType(ESkinType eSkinType)
{
	m_SkinType = eSkinType;
	UpdateSkin();
}
void CIFIStatic::UpdateSkin(const TSkinLabel* pSkinLabel/* = NULL*/)
{
	m_SkinProperties.SetWindowsInvalidateLock(TRUE);

	if(pSkinLabel == NULL)
	{
		pSkinLabel = &CIFISkinSetting::GetIFISkinSetting().m_SkinLabels[m_SkinType];
	}

	m_SkinProperties.SetBgrColor(pSkinLabel->clrBgr);
	m_SkinProperties.SetTextColor(pSkinLabel->clrText);

	m_SkinProperties.SetFontName(pSkinLabel->tFont.szFontName);
	m_SkinProperties.SetFontSize(pSkinLabel->tFont.nFontSize);
	m_SkinProperties.SetFontScale(pSkinLabel->tFont.nFontScale);
	m_SkinProperties.SetFontWeight(pSkinLabel->tFont.lfFontWeight);
	m_SkinProperties.SetFontItalic(pSkinLabel->tFont.lfFontItalic);
	m_SkinProperties.SetFontUnderline(pSkinLabel->tFont.lfFontUnderline);

	m_SkinProperties.SetWindowsInvalidateLock(FALSE);
	if(GetSafeHwnd())
	{
		SetFont(m_SkinProperties.GetCustomFont());
		RedrawWindow(NULL,NULL,RDW_ERASENOW|RDW_FRAME|RDW_INVALIDATE);
	}
}
CSize CIFIStatic::GetContentSize()
{
	CSize size(0,0);
	if(GetSafeHwnd())
	{
		CDC* pDC = GetDC();
		CString strText(_T(""));
		GetWindowText(strText);

		CFont* pFont = m_SkinProperties.GetCustomFont();
		if(pFont)
		{
			pDC->SelectObject(pFont);
		}
		size = pDC->GetTextExtent(strText);

		ReleaseDC(pDC);
	}
	return size;
}

BEGIN_MESSAGE_MAP(CIFIStatic, CStatic)

	ON_WM_CTLCOLOR_REFLECT()
	//ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_SIZE()
END_MESSAGE_MAP()

HBRUSH CIFIStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(m_SkinProperties.m_clrText);

	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CIFIStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting		
	CRect rc;
	GetClientRect(rc);
	dc.SetTextColor(m_SkinProperties.m_clrText);
	dc.SetBkMode(TRANSPARENT);
	rc.DeflateRect(0,0,1,1);
	//dc.SetBkColor(m_clrBgr);
	if(m_SkinProperties.m_nRoundCornerRadius > 0)
	{
		CRgn rgn;		
		//rgn.CreateRoundRectRgn(rc.left,rc.top,rc.Width(),rc.Height(),m_nRoundCornerRadius*2, m_nRoundCornerRadius*2);
		CGdiPlusUtils::GetRgn(&dc,rc,m_SkinProperties.m_nRoundCornerRadius,rgn);
		if(rgn.GetSafeHandle())
		{
			dc.SelectClipRgn(&rgn,RGN_COPY);
		}
	}
	rc.DeflateRect(1,1,1,1);
	CGdiPlusUtils::FillSmoothRect(&dc,rc,m_SkinProperties.m_clrBgr,m_SkinProperties.m_clrBorder,m_SkinProperties.m_nRoundCornerRadius,m_SkinProperties.m_nBorderWidth);
	if(m_SkinProperties.m_clrBgr != -1)
	{		
		dc.FillRect(rc,&CBrush(m_SkinProperties.m_clrBgr));
	}
	else
	{
		//if(GetParent())
		//{
		//	COLORREF clrParentBgr =  GetParent()->GetDC()->GetBkColor();
		//	dc.FillRect(rc,&CBrush(clrParentBgr));
		//}		
	}

	CFont *pFont = m_SkinProperties.GetCustomFont();	
	CFont* pOldFont = NULL;
	if(pFont)
	{
		pOldFont = dc.SelectObject(pFont);
	}

	CString strText;
	GetWindowText(strText);
	DWORD dwStyle = GetStyle();
	DWORD dwTextDrawStyle = 0;
	if(dwStyle&SS_LEFT)
	{
		dwTextDrawStyle |= DT_LEFT;
	}
	else if(dwStyle&SS_RIGHT)
	{
		dwTextDrawStyle |= DT_RIGHT;
	}
	else if(dwStyle&SS_CENTER)
	{
		dwTextDrawStyle |= DT_CENTER;
	}
	if(dwStyle&SS_ENDELLIPSIS)
	{
		dwTextDrawStyle |= DT_END_ELLIPSIS;
	}
	if(dwStyle&SS_WORDELLIPSIS)
	{
		dwTextDrawStyle |= DT_WORD_ELLIPSIS;
	}
	if(dwStyle&SS_PATHELLIPSIS)
	{
		dwTextDrawStyle |= DT_PATH_ELLIPSIS ;
	}	
	if(dwStyle&SS_CENTERIMAGE)
	{
		dwTextDrawStyle |= DT_VCENTER | DT_SINGLELINE;
	}
	if(dwStyle&SS_NOPREFIX)
	{
		dwTextDrawStyle |= DT_NOPREFIX;
	}
	if(!(dwStyle&(SS_LEFTNOWORDWRAP|SS_CENTERIMAGE|SS_WORDELLIPSIS|SS_ENDELLIPSIS|SS_PATHELLIPSIS)))
	{
		dwTextDrawStyle |= DT_WORDBREAK;
	}
	rc.DeflateRect(m_SkinProperties.m_nPaddingLeft,m_SkinProperties.m_nPaddingTop,m_SkinProperties.m_nPaddingRight,m_SkinProperties.m_nPaddingBottom);
	dc.DrawText(strText,rc,dwTextDrawStyle);
	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}

	dc.SelectClipRgn(NULL);
}

BOOL CIFIStatic::OnEraseBkgnd(CDC* pDC)
{
	pDC->SelectStockObject(NULL_BRUSH);

	if (m_bgrBitmap.GetSafeHandle() == NULL)
	{
		CRect Rect;
		GetWindowRect(&Rect);
		CWnd *pParent = GetParent();
		
		ASSERT(pParent);
		pParent->ScreenToClient(&Rect);    //convert our co-ordinates
		//to our parents

		//copy what's on the parents at this point
		CDC *pDC = pParent->GetDC();
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		m_bgrBitmap.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());
		CBitmap *pOldBmp = MemDC.SelectObject(&m_bgrBitmap);
		pParent->InvalidateRect(Rect);
		MemDC.BitBlt(0,0,Rect.Width(),Rect.Height(),pDC,Rect.left,
			Rect.top,SRCCOPY);

		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
		pParent->ReleaseDC(pDC);
	}
	else    //copy what we copied off the parent the first time
		//back onto the parent
	{
		CRect Rect;
		GetClientRect(Rect);
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp = MemDC.SelectObject(&m_bgrBitmap);
		pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
	}
	return TRUE;
}


void CIFIStatic::PreSubclassWindow()
{
	UpdateSkin();

	CFont* pFont = m_SkinProperties.GetCustomFont();
	SetFont(pFont);
	ModifyStyle(0,SS_NOTIFY);
	__super::PreSubclassWindow();
}
LRESULT CIFIStatic::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT res = Default();

	RedrawWindow(NULL,NULL,RDW_ERASENOW|RDW_FRAME|RDW_INVALIDATE);
	return 1;
}

void CIFIStatic::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if(m_bgrBitmap.GetSafeHandle())
	{
		m_bgrBitmap.DeleteObject();
	}
}
