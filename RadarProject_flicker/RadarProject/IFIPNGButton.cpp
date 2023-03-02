//////////////////////////////////////////////////////////////////////////////////////////////////
// File: IFIPNGButton.cpp																		//
// Class: CIFIPNGButton																			//
// Description: Implement of class CIFIPNGButton												//
// Date: 2018/08																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GdiPlusUtils.h"
#include "IFIPNGButton.h"
#include "IFIGraphicUtils.h"

// CIFIPNGButton

IMPLEMENT_DYNAMIC(CIFIPNGButton, CButton)

CIFIPNGButton::CIFIPNGButton()
: m_dwTextAlign(DT_CENTER)
, m_nCheck(0)
, m_bIsCheckBox(FALSE)
, m_pBitmap(NULL)
, m_pBitmapChecked(NULL)
, m_pCurrBitmap(NULL)
, m_bLMouseDown(FALSE)
, m_iExtraValue(-1)
{
	m_clrText = RGB(0,0,0);

	m_strToolTip = _T("");
}

CIFIPNGButton::~CIFIPNGButton()
{
	
}
void CIFIPNGButton::SetCheckBoxStyle(BOOL bCheckbox)
{
	m_bIsCheckBox = bCheckbox;
}
void CIFIPNGButton::SetPNG(UINT uIDResource, HINSTANCE hInstance/* = NULL*/)
{
	Bitmap* pBmp;
	if(hInstance == NULL)
	{
		CGdiPlusUtils::LoadBitmapFromResource(uIDResource,&pBmp,gDllIFIUIStyle201Ins);
	}
	else
	{
		CGdiPlusUtils::LoadBitmapFromResource(uIDResource,&pBmp,hInstance);
	}

	if(((Bitmap*)m_pBitmap) != NULL)
	{
		delete (Bitmap*)m_pBitmap;
	}
	m_pBitmap = (Bitmap*)pBmp;
}
void CIFIPNGButton::SetPNG(CString strPngPath)
{
	Bitmap* pBmp;
	CGdiPlusUtils::LoadBitmapFromFile(strPngPath,&pBmp);
	if(((Bitmap*)m_pBitmap) != NULL)
	{
		delete (Bitmap*)m_pBitmap;
	}
	m_pBitmap = (Bitmap*)pBmp;
}
void CIFIPNGButton::SetCheckedPNG(UINT uIDResource, HINSTANCE hInstance/* = NULL*/)
{
	Bitmap* pBmp;
	if(hInstance == NULL)
	{
		CGdiPlusUtils::LoadBitmapFromResource(uIDResource,&pBmp,gDllIFIUIStyle201Ins);
	}
	else
	{
		CGdiPlusUtils::LoadBitmapFromResource(uIDResource,&pBmp,hInstance);
	}

	if(((Bitmap*)m_pBitmapChecked) != NULL)
	{
		delete (Bitmap*)m_pBitmapChecked;
	}
	m_pBitmapChecked = (Bitmap*)pBmp;
}
void CIFIPNGButton::SetCheckedPNG(CString strPngPath)
{
	Bitmap* pBmp;
	CGdiPlusUtils::LoadBitmapFromFile(strPngPath,&pBmp);
	if(((Bitmap*)m_pBitmapChecked) != NULL)
	{
		delete (Bitmap*)m_pBitmapChecked;
	}
	m_pBitmapChecked = (Bitmap*)pBmp;
}

BEGIN_MESSAGE_MAP(CIFIPNGButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CIFIPNGButton message handlers
BOOL CIFIPNGButton::OnEraseBkgnd(CDC* pDC) 
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


void CIFIPNGButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CRect rcTemp(lpDrawItemStruct->rcItem);
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			
	pDC->FillSolidRect(rcTemp,pDC->GetBkColor());

	CRgn rgn;
	if(m_pCurrBitmap == NULL)
	{
		m_pCurrBitmap = m_pBitmap;
	}
	if(m_pCurrBitmap != NULL)
	{		
		CGdiPlusUtils::GetRegion((Bitmap*)m_pCurrBitmap,0,rcTemp,true,rgn);
	}

	if(rgn.GetSafeHandle())
	{
		pDC->SelectClipRgn(&rgn,RGN_COPY);					
	}

	BOOL bPressed1 = FALSE;

    if (GetKeyState(VK_LBUTTON) < 0 && GetFocus() == this)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);		
		if(rcTemp.PtInRect(point))
		{
			bPressed1 = TRUE;
		}
	}
	//if(m_bIsCheckBox && m_nCheck != 0)
	//{
	//	bPressed1 = TRUE;
	//}
    if (bPressed1)
	{		
		CGdiPlusUtils::DrawBitmapFitInRect(pDC,(Bitmap*)m_pCurrBitmap,RGB(200,200,200),100,rcTemp);	
	}
	else if(m_pCurrBitmap != NULL)
	{
		if(!IsWindowEnabled())
		{
			CGdiPlusUtils::DrawGrayBitmapFitInRect(pDC,(Bitmap*)m_pCurrBitmap,rcTemp);
		}
		else
		{
			CGdiPlusUtils::DrawBitmapFitInRect(pDC,(Bitmap*)m_pCurrBitmap,-1,255,rcTemp);
		}
	}

	CString strText;
	GetWindowText(strText);	
	CRect rcIcon(rcTemp);
	if(!strText.IsEmpty())
	{
		rcIcon.right = rcIcon.left + rcIcon.Height();
		CRect rcText(rcTemp);

		pDC->SetBkMode(TRANSPARENT);
		if(m_bIsCheckBox && m_nCheck != 0)
		{
			pDC->SetTextColor(m_clrText);
		}
		else
		{
			pDC->SetTextColor(m_clrText);
		}
		rcText.DeflateRect(1,3,1,1);
		pDC->DrawText(strText,rcText,DT_SINGLELINE|DT_VCENTER|m_dwTextAlign);	
	}

	//if(GetFocus() == this && m_clrBorder != -1)
	//{
	//	rcTemp.DeflateRect(0,0,1,1);
	//	COLORREF clrLighter= CIFIGraphicUtils::Lighter(m_clrBgr,1.4f);	
	//	CGdiPlusUtils::DrawSmoothBorder(pDC,rcTemp,clrLighter,m_iCornerRadius,m_iCornerRadius,m_iCornerRadius,m_iCornerRadius,1);
	//}
	rgn.DeleteObject();
	pDC->SelectClipRgn(NULL);
}

void CIFIPNGButton::SetText(CString sText, DWORD dwTextAlign)
{
	SetWindowText(sText);
	m_dwTextAlign = dwTextAlign;
	if(IsWindow(m_hWnd))
	{
		Invalidate();
	}
}
void CIFIPNGButton::SetTextAlign(DWORD dwTextAlign)
{
	m_dwTextAlign = dwTextAlign;
	if(IsWindow(m_hWnd))
	{
		Invalidate();
	}
}
void CIFIPNGButton::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	if(IsWindow(m_hWnd))
	{
		Invalidate();
	}
}


void CIFIPNGButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CButton::OnActivate(nState, pWndOther, bMinimized);

	Invalidate();
}
void CIFIPNGButton::SetToolTipText(CString strToolTip)
{
	m_strToolTip = strToolTip;
	if(IsWindow(GetSafeHwnd()))
	{		
		EnableToolTips(TRUE);
	}
}
INT_PTR CIFIPNGButton::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{	    
	CRect rc;
	GetClientRect(rc);
    pTI->rect = rc;
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT_PTR)(0);
	pTI->uFlags = TTF_ALWAYSTIP;
	pTI->lpszText = new WCHAR[m_strToolTip.GetLength()+1];
				
	StrCpy(pTI->lpszText, m_strToolTip);
	
	return 0;
}
void CIFIPNGButton::SetCheck(int nCheck)
{
	if(m_bIsCheckBox)
	{
		m_nCheck = nCheck;
		if(nCheck == BST_CHECKED)
		{
			m_pCurrBitmap = m_pBitmapChecked;
		}
		else
		{
			m_pCurrBitmap = m_pBitmap;
		}
		Invalidate(FALSE);
	}
	else
	{
		m_pCurrBitmap = m_pBitmap;
		CButton::SetCheck(nCheck);
	}
}
int CIFIPNGButton::GetCheck()
{
	if(m_bIsCheckBox)
	{
		return m_nCheck;
	}
	return CButton::GetCheck();
}
void CIFIPNGButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLMouseDown = TRUE;
	CWnd::OnLButtonDown(nFlags,point);	
}
void CIFIPNGButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(rc);	

	if(rc.PtInRect(point) && m_bIsCheckBox && m_bLMouseDown == TRUE)
	{
		m_nCheck = m_nCheck > 0? 0 : 1;
		
		if(m_nCheck)
		{
			m_pCurrBitmap = m_pBitmapChecked;
		}
		else
		{
			m_pCurrBitmap = m_pBitmap;
		}
	}
	m_bLMouseDown = FALSE;
	CButton::OnLButtonUp(nFlags,point);
}
void CIFIPNGButton::PerformClicked()
{
	::PostMessage(::GetParent(m_hWnd),WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);
}
void CIFIPNGButton::RebuildSkin()
{
	if(m_pBitmap != NULL)
	{
		CRect rc;
		CRgn rgn;
		GetClientRect(rc);

		CGdiPlusUtils::GetRegion((Bitmap*)m_pBitmap,0,rc,true,rgn);
		
		if(m_pBitmapChecked != 0)
		{
			CRgn rgnChecked;
			CGdiPlusUtils::GetRegion((Bitmap*)m_pBitmapChecked,0,rc,true,rgnChecked);
			rgn.CombineRgn(&rgn,&rgnChecked,RGN_OR);
			rgnChecked.DeleteObject();
		}
		SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()),TRUE);
		rgn.DeleteObject();
	}
}
int CIFIPNGButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CIFIPNGButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	RebuildSkin();

	if(m_bgrBitmap.GetSafeHandle())
	{
		m_bgrBitmap.DeleteObject();
	}
}


LRESULT CIFIPNGButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CButton::DefWindowProc(message, wParam, lParam);
}


BOOL CIFIPNGButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CButton::PreCreateWindow(cs);
}


void CIFIPNGButton::PreSubclassWindow()
{	
	CButton::PreSubclassWindow();	
	ModifyStyle(0,BS_OWNERDRAW);
	RebuildSkin();
}

HBRUSH CIFIPNGButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

INT CIFIPNGButton::GetExtraValue()
{
	return m_iExtraValue;
}

void CIFIPNGButton::SetExtraValue(INT iNewValue)
{
	if(m_iExtraValue!=iNewValue)
	{
		m_iExtraValue = iNewValue;
	}
}