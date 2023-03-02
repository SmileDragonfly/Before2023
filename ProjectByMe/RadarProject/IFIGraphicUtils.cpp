//////////////////////////////////////////////////////////////////////////////////////////////////
// File: IFIGraphicUtils.cpp																	//
// Class: CIFIGraphicUtils																		//
// Description: Implement of class CIFIGraphicUtils												//
// Date: 2018/08																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "IFIUIDefined.h"
#include "IFIGraphicUtils.h"

CIFIGraphicUtils::CIFIGraphicUtils(void)
{
}

CIFIGraphicUtils::~CIFIGraphicUtils(void)
{
}
void CIFIGraphicUtils::DrawGradient(CDC *pDC, CRect r, COLORREF clrBegin, COLORREF clrEnd, BOOL bVertical)
{
	CRect stepR;					// rectangle for color's band
	COLORREF color;				// color for the bands
	float fStep;
	
	if(bVertical)
		fStep = ((float)r.Height())/255.0f;	
	else
		fStep = ((float)r.Width())/255.0f;	// width of color's band
	
	for (int iOnBand = 0; iOnBand < 255; iOnBand++) 
	{
		// set current band
		if(bVertical)
		{
			::SetRect(&stepR,
				r.left, 
				r.top+(int)(iOnBand * fStep),
				r.right, 
				r.top+(int)((iOnBand+1)* fStep));	
		}
		else
		{
			::SetRect(&stepR,
				r.left+(int)(iOnBand * fStep), 
				r.top,
				r.left+(int)((iOnBand+1)* fStep), 
				r.bottom);	
		}

		// set current color
		color = RGB((GetRValue(clrEnd)-GetRValue(clrBegin))*((float)iOnBand)/255.0f+GetRValue(clrBegin),
			(GetGValue(clrEnd)-GetGValue(clrBegin))*((float)iOnBand)/255.0f+GetGValue(clrBegin),
			(GetBValue(clrEnd)-GetBValue(clrBegin))*((float)iOnBand)/255.0f+GetBValue(clrBegin));
		// fill current band
		pDC->FillSolidRect(stepR,color);
	}
}

COLORREF CIFIGraphicUtils::Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedNew, bBlueNew, bGreenNew;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedNew = (BYTE)(bRed * fFactor);
	bBlueNew = (BYTE)(bBlue * fFactor);
	bGreenNew = (BYTE)(bGreen * fFactor);

	return RGB(bRedNew, bGreenNew, bBlueNew);
}

COLORREF CIFIGraphicUtils::Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedNew, bBlueNew, bGreenNew;

	bRed = 255 - GetRValue(crBase);
	bBlue = 255 - GetBValue(crBase);
	bGreen = 255 - GetGValue(crBase);

	bRedNew = 255 - (BYTE)(bRed * fFactor);
	bBlueNew = 255 - (BYTE)(bBlue * fFactor);
	bGreenNew = 255 - (BYTE)(bGreen * fFactor);

	return RGB(bRedNew, bGreenNew, bBlueNew);
}
COLORREF CIFIGraphicUtils::Gray(COLORREF crBase)
{
	BYTE bThreshold = (GetRValue(crBase) + GetBValue(crBase) + GetGValue(crBase))/3;
	return RGB(bThreshold,bThreshold,bThreshold);
}
HICON CIFIGraphicUtils::LoadIcon(UINT nIDIcon, CSize size)
{
	return (HICON)::LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(nIDIcon), RT_GROUP_ICON),
							  MAKEINTRESOURCE(nIDIcon), IMAGE_ICON, size.cx,size.cy, LR_SHARED);
}
void CIFIGraphicUtils::CalculRectangles(RECT &Source, RECT &Dest, BOOL Maximize)
{
	POINT Zone = {Dest.right - Dest.left, Dest.bottom - Dest.top};
	POINT Img = {Source.right - Source.left, Source.bottom - Source.top};
	if (Maximize)
	{
		if (MulDiv(Zone.x, 1000, Img.x) > MulDiv(Zone.y, 1000, Img.y))
		{
			Dest.bottom = Zone.y;
			Dest.right = MulDiv(Img.x, Zone.y, Img.y);
		}
		else
		{
			Dest.right = Zone.x;
			Dest.bottom = MulDiv(Img.y, Zone.x, Img.x);
		}
		Dest.left += (Zone.x - Dest.right) / 2;
		Dest.top += (Zone.y - Dest.bottom) / 2;
	}
	else
	{
		RECT rcClient = Dest;
		Dest.left += (Zone.x - Img.x) / 2;
		Dest.top += (Zone.y - Img.y) / 2;
		if (Dest.left < rcClient.left)
			Dest.left = rcClient.left;
		if (Dest.top < rcClient.top)
			Dest.top = rcClient.top;
		if (Zone.x >= Img.x)
		{
			Dest.right = Img.x;
			Source.left = 0;
		}
		else
		{
			Dest.right = Zone.x;
			Source.left = (Img.x - Zone.x) / 2;
		}

		if (Zone.y >= Img.y)
		{
			Dest.bottom = Img.y;
			Source.top = 0;
		}
		else
		{
			Dest.bottom = Zone.y;
			Source.top = (Img.y - Zone.y) / 2;
		}
		Source.right = Dest.right;
		Source.bottom = Dest.bottom;
	}
	Dest.right += Dest.left;
	Dest.bottom += Dest.top;
	Source.right += Source.left;
	Source.bottom += Source.top;
}
void CIFIGraphicUtils::DrawIcon(CDC* pDC, LPCRECT LeRect, HICON LIcone)
{
	ICONINFO IconInfo;
	if (!GetIconInfo(LIcone, &IconInfo))
		return;
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	int iSave = pDC->SaveDC();

	BITMAP bmp;
	::GetObject(IconInfo.hbmMask, sizeof(BITMAP), &bmp);

	CRect DestImg(LeRect);
	CRect SrcImg(0, 0, bmp.bmWidth, bmp.bmHeight);
	CalculRectangles(SrcImg, DestImg, FALSE);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg( 0, 0 );
	pDC->SetWindowExt(bmp.bmWidth, bmp.bmHeight ); 
	pDC->SetViewportOrg( 0, 0);
	pDC->SetViewportExt(DestImg.Width(),DestImg.Height()); 

	COLORREF OldTColor = pDC->SetTextColor(CLR_BLACK);
	COLORREF OldBColor = pDC->SetBkColor(CLR_WHITE);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(dc.m_hDC, IconInfo.hbmMask);
	pDC->BitBlt(DestImg.left, DestImg.top, DestImg.Width(), DestImg.Height(), 
				&dc, SrcImg.left, SrcImg.top, SRCAND);
	SelectObject(dc.m_hDC, IconInfo.hbmColor);
	pDC->BitBlt(DestImg.left, DestImg.top, DestImg.Width(), DestImg.Height(), 
				&dc, SrcImg.left, SrcImg.top, SRCPAINT);
	SelectObject(dc.m_hDC, OldBitmap);
	DeleteObject(IconInfo.hbmMask);
	DeleteObject(IconInfo.hbmColor);
	pDC->SetTextColor(OldTColor);
	pDC->SetBkColor(OldBColor);
	pDC->RestoreDC(iSave);
	//DestroyIcon(LIcone);
} 
COLORREF CIFIGraphicUtils::InvertColor(COLORREF clrIn)
{
	COLORREF clrOut = RGB(255-GetRValue(clrIn),255-GetGValue(clrIn),255-GetBValue(clrIn));
	return clrOut;
}
COLORREF CIFIGraphicUtils::IdealTextColor(COLORREF clrIn)
{
    int nThreshold = 105;
	int bRed = GetRValue(clrIn);
	int bBlue = GetBValue(clrIn);
	int bGreen = GetGValue(clrIn);

    int bgDelta = (int)((bRed * 0.299) + (bGreen * 0.587) + 
                                  (bBlue * 0.114));

    COLORREF foreColor = (255 - bgDelta < nThreshold) ? RGB(0,0,0): RGB(255,255,255);
    return foreColor;
}