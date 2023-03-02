//////////////////////////////////////////////////////////////////////////////////////////////////
// File: GdiPlusUtils.cpp																		//  
// Class: CGdiPlusUtils																			//
// Description: Implement of class CGdiPlusUtils												//
// Date: 2018/08																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GdiPlusUtils.h"

short ar[256];
short ag[256];
short ab[256];

CGdiPlusUtils::CGdiPlusUtils(void)
{
}


CGdiPlusUtils::~CGdiPlusUtils(void)
{
}

bool CGdiPlusUtils::LoadBitmapFromResource(UINT uResourceID, Bitmap** ppBitmapOut, HINSTANCE hInstance /*= NULL*/)
{
    bool bRet = false;

    if (!hInstance)
	{
		hInstance = gDllIFIUIStyle201Ins;
	}

    HRSRC hResourceHandle = ::FindResource(hInstance, MAKEINTRESOURCE(uResourceID), _T("PNG"));
    if (0 == hResourceHandle)
    {
        return bRet;
    }

    DWORD nImageSize = ::SizeofResource(hInstance, hResourceHandle);
    if (0 == nImageSize)
    {
        return bRet;
    }

    HGLOBAL hResourceInstance = ::LoadResource(hInstance, hResourceHandle);
    if (0 == hResourceInstance)
    {
        return bRet;
    }

    const void* pResourceData = ::LockResource(hResourceInstance);
    if (0 == pResourceData)
    {
        FreeResource(hResourceInstance);
        return bRet;
    }

    HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
    if (0 == hBuffer)
    {
        FreeResource(hResourceInstance);
        return bRet;
    }

    void* pBuffer = ::GlobalLock(hBuffer);
    if (0 != pBuffer)
    {
        CopyMemory(pBuffer, pResourceData, nImageSize);
        IStream* pStream = 0;
        if (S_OK == ::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream))
        {
			Bitmap* pBmp = Gdiplus::Bitmap::FromStream(pStream);
			pStream->Release();
			if(pBmp->GetLastStatus() == Gdiplus::Ok)
			{
				*ppBitmapOut = new Bitmap(pBmp->GetWidth(), pBmp->GetHeight(),pBmp->GetPixelFormat());
				//*ppBitmapOut = pBmp->Clone(Rect(0,0,pBmp->GetWidth(), pBmp->GetHeight()),pBmp->GetPixelFormat());
				Graphics* pMemGraphics = Graphics::FromImage(*ppBitmapOut);
				pMemGraphics->DrawImage(pBmp,Rect(0,0,pBmp->GetWidth(), pBmp->GetHeight()));
				delete pMemGraphics;
				bRet = true;
			}
			else
			{
				*ppBitmapOut = NULL;
			}
			delete pBmp;			
        }
        ::GlobalUnlock(hBuffer);		
    }
    ::GlobalFree(hBuffer);

    UnlockResource(hResourceInstance);
    FreeResource(hResourceInstance);

    return bRet;
}
bool CGdiPlusUtils::LoadBitmapFromFile(CString strFilePath, Bitmap** ppBitmapOut)
{
	bool bRet = false;
	Bitmap* pBmp = Bitmap::FromFile(strFilePath.GetBuffer());
	strFilePath.ReleaseBuffer();
	if(pBmp->GetLastStatus() == Gdiplus::Ok)
	{
		*ppBitmapOut = new Bitmap(pBmp->GetWidth(), pBmp->GetHeight(),pBmp->GetPixelFormat());
		//*ppBitmapOut = pBmp->Clone(Rect(0,0,pBmp->GetWidth(), pBmp->GetHeight()),pBmp->GetPixelFormat());
		Graphics* pMemGraphics = Graphics::FromImage(*ppBitmapOut);
		pMemGraphics->DrawImage(pBmp,Rect(0,0,pBmp->GetWidth(), pBmp->GetHeight()));
		delete pMemGraphics;
		bRet = true;
	}
	else
	{
		*ppBitmapOut = NULL;
	}
	delete pBmp;		
	return bRet;
}

void CGdiPlusUtils::GetGraphicsPath(Bitmap& bitmap,GraphicsPath& graphicsPath)
{
	Color colorAt;

	for(UINT row = 1; row < bitmap.GetHeight()-1; row ++)
    {
		for(UINT col = 1; col < bitmap.GetWidth()-1; col ++)
        {
			bitmap.GetPixel(col, row,&colorAt);

			if(colorAt.GetA() > 0)
			{
                graphicsPath.AddRectangle(Rect(col, 
                                           row, 1, 1));
			}
        }
    }
}
BYTE CGdiPlusUtils::GetBrightestColor(Bitmap& bmp)
{
	BYTE byRes = 0;
	if(bmp.GetLastStatus() == 0)
	{
		Color clrAt;

		UINT32* pixels = NULL;
		BitmapData bitmapData;
		
		if(Ok == bmp.LockBits(&Rect(0,0,bmp.GetWidth(),bmp.GetHeight()),
			ImageLockModeRead ,bmp.GetPixelFormat(),&bitmapData))
		{
			pixels = (UINT32*)bitmapData.Scan0;

			UINT row, col;
			BYTE clr;
			for(row = 0; row < bmp.GetHeight(); ++row)
			{
				for(col = 0; col < bmp.GetWidth(); ++col)
				{
					clrAt.SetValue(pixels[row * bitmapData.Stride / 4 + col]);
					if(clrAt.GetA() > 0)
					{
						clr = ar[clrAt.GetR()] + ag[clrAt.GetG()] + ab[clrAt.GetB()];
						if(clr > byRes)
							byRes = clr;
					}
				}
			}

			//Unlock
			bmp.UnlockBits(&bitmapData);
		}
	}
	return byRes;
}
void CGdiPlusUtils::ReplaceColor(Bitmap* pBmp, Color colorRpl)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == 0)
	{
		BYTE brMax = 255;//GetBrightestColor(bmp);

		UINT32* pixels;

		BitmapData bitmapData;

		if(Ok == pBmp->LockBits(&Rect(0,0,pBmp->GetWidth(),pBmp->GetHeight()),
			ImageLockModeRead|ImageLockModeWrite
			,pBmp->GetPixelFormat(),&bitmapData))
		{
			pixels = (UINT32*)bitmapData.Scan0;

			Color clrAt;		
			BYTE clr;
			BYTE newR;
			BYTE newG;
			BYTE newB;	

			UINT row, col;
			for(row = 0; row < pBmp->GetHeight(); ++row)
			{
				for(col = 0; col < pBmp->GetWidth(); ++col)
				{
					newR = colorRpl.GetR();
					newG = colorRpl.GetG();
					newB = colorRpl.GetB();

					clrAt.SetValue(pixels[row * bitmapData.Stride / 4 + col]);
					if(clrAt.GetA() > 0)
					{
						clr = ar[clrAt.GetR()] + ag[clrAt.GetG()] + ab[clrAt.GetB()];
						//if(clr != 0)
						//{
							newR = (BYTE)(colorRpl.GetR()* (float)clr/(float)brMax);
							newG = (BYTE)(colorRpl.GetG()* (float)clr/(float)brMax);
							newB = (BYTE)(colorRpl.GetB()* (float)clr/(float)brMax);
						//}
						pixels[row * bitmapData.Stride / 4 + col] = Color::MakeARGB(clrAt.GetA(), newR,newG,newB);
					}
				}
			}
			
			//Unlock and Commit the changes 
			pBmp->UnlockBits(&bitmapData);
		}
	}	
}
void CGdiPlusUtils::ReplaceColorMatrix(Bitmap* pBmp, Color clr)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		Graphics* pGraph = Graphics::FromImage(pBmp);

		float a = clr.GetA()/255.0f;
		float r = clr.GetR()/255.0f;
		float g = clr.GetG()/255.0f;
		float b = clr.GetB()/255.0f;		

		//ColorMatrix colorMatrix = {0, 0, 0, 0, 0,
		//						   0, 0, 0, 0, 0,
		//						   r, g, b, 0, 0,
		//						   0, 0, 0, 1.0f, 0,
		//						   0, 0, 0, 0, 1.0f};

		ColorMatrix colorMatrix = {-r*0.10f, -g*0.10f, -b*0.10f, 0, 0,
								   r*0.2f, g*0.2f, b*0.2f, 0, 0,
								   r, g, b, a, 0,
								   0, 0, 0, a, 0,
								   0, 0, 0, 0, 1.0f};

		//create some image attributes
		ImageAttributes attributes;

		//set the color matrix attribute
		attributes.SetColorMatrix(&colorMatrix);

		//draw the original image on the new image		
		pGraph->DrawImage(pBmp, RectF(0.0f, 0.0f, (REAL)pBmp->GetWidth(),(REAL)pBmp->GetHeight()),
			0.0, 0.0, (REAL)pBmp->GetWidth(),(REAL)pBmp->GetHeight(), UnitPixel, &attributes);

		//dispose the Graphics object
		delete pGraph;	
	}
}
void CGdiPlusUtils::ReplaceGrayColor(Bitmap* pBmp)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		Graphics* pGraph = Graphics::FromImage(pBmp);

		ColorMatrix matrix =
		{
			.3f, .3f, .3f,   0,   0,
			.6f, .6f, .6f,   0,   0,
			.1f, .1f, .1f,   0,   0,
			0,   0,   0,   1,   0,
			0,   0,   0,   0,   1
		};
		ImageAttributes attr;
		attr.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);	

		//draw the original image on the new image		
		pGraph->DrawImage(pBmp, RectF(0.0f, 0.0f, (REAL)pBmp->GetWidth(),(REAL)pBmp->GetHeight()),
			0.0, 0.0, (REAL)pBmp->GetWidth(),(REAL)pBmp->GetHeight(), UnitPixel, &attr);

		//dispose the Graphics object
		delete pGraph;	
	}
}
void CGdiPlusUtils::GetRegion(Bitmap *pBmp, double Angle, RectF ScaleInRect, bool bFit, CRgn& rgn)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		UINT nBmpWidth = pBmp->GetWidth();
		UINT nBmpHeight = pBmp->GetHeight();

		float fWRatio = ((float) nBmpWidth) / ((float) ScaleInRect.Width);
		float fHRatio = ((float) nBmpHeight) / ((float) ScaleInRect.Height);

		Bitmap* pBmpClone = NULL;
		INT iWidthRes = nBmpWidth;
		INT iHeightRes = nBmpHeight;

		if(bFit)
		{
			float fRate = fHRatio;
			if(fWRatio > fHRatio)
			{
				fRate = fWRatio;
			}
			pBmpClone	= CGdiPlusUtils::CloneBitmap(pBmp,1/fRate,1/fRate);
			iWidthRes	= (INT)(nBmpWidth/fRate);
			iHeightRes	= (INT)(nBmpHeight/fRate);
		}
		else
		{
			pBmpClone	= CGdiPlusUtils::CloneBitmap(pBmp,1/fWRatio,1/fHRatio);
			iWidthRes	= (INT)(nBmpWidth/fWRatio);
			iHeightRes	= (INT)(nBmpHeight/fHRatio);
		}
		
		if(pBmpClone != NULL)
		{
			if(Angle != 0)
			{
				Bitmap* pBmpClone2 = RotateBitmap(pBmpClone,Angle,ScaleInRect.Width/2.0f,ScaleInRect.Height/2.0f);
				delete pBmpClone;
				pBmpClone = pBmpClone2;
			}
		}

		GraphicsPath path;
		
		CGdiPlusUtils::GetGraphicsPath(*pBmpClone,path);
		
		Region region(&path);
		Graphics* pMemGraphics = Graphics::FromImage(pBmpClone);
		HRGN hrgn = region.GetHRGN(pMemGraphics);
		CRgn rgnTemp;
		rgnTemp.Attach(hrgn);
		if(!rgn.GetSafeHandle())
		{
			rgn.CreateRectRgn(0,0,0,0);
		}
		rgn.CopyRgn(&rgnTemp);
		CRect rcOut;
		rgn.GetRgnBox(rcOut);
		rgn.OffsetRgn(-rcOut.left,-rcOut.top);
		rgn.OffsetRgn((INT)(ScaleInRect.Width/2) - (INT)(rcOut.Width()/2),(INT)(ScaleInRect.Height/2) - (INT)(rcOut.Height()/2));
		rgnTemp.DeleteObject();
		delete pMemGraphics;
		if(pBmpClone)
		{
			delete pBmpClone;
		}
	}
}
void CGdiPlusUtils::GetRegion(Bitmap *pBmp, double Angle, CRect ScaleInRect, bool bFit, CRgn& rgn)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		RectF rf((REAL)ScaleInRect.left-1,(REAL)ScaleInRect.top-1,(REAL)ScaleInRect.Width()+1,(REAL)ScaleInRect.Height()+1);
		CGdiPlusUtils::GetRegion(pBmp,Angle, rf,bFit, rgn);
	}
}

void CGdiPlusUtils::GetRegion(Bitmap *pBmp, CRect ScaleInRect, bool bFit, CRgn& rgn)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		RectF rf((REAL)ScaleInRect.left-1,(REAL)ScaleInRect.top-1,(REAL)ScaleInRect.Width()+1,(REAL)ScaleInRect.Height()+1);
		CGdiPlusUtils::GetRegion(pBmp, 0, rf,bFit, rgn);
	}
}

void CGdiPlusUtils::GetRect(Bitmap *pBmp, CRect& rcOut)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		CRect rc(0,0,pBmp->GetWidth(),pBmp->GetHeight());
		rcOut = rc;
	}
}
void CGdiPlusUtils::RecalcRect(Bitmap *pBmp, double Angle, CRect ScaleInRect, CRect& rcOut)
{	
	if(pBmp != NULL)
	{
		CRect rc(ScaleInRect.left,ScaleInRect.top,ScaleInRect.left+ScaleInRect.Width(),(int)(ScaleInRect.top + ScaleInRect.Width()*((float)pBmp->GetHeight()/(float)pBmp->GetWidth())));
		rcOut = rc;
	}
}

Bitmap* CGdiPlusUtils::MirrorBitmap(Bitmap *pBmp)
{
	RectF rc((REAL)0,(REAL)0,(REAL)pBmp->GetWidth(),(REAL)pBmp->GetHeight());
	RectF rcSrc((REAL)pBmp->GetWidth(),(REAL)0,(REAL)(-1)*pBmp->GetWidth(),(REAL)pBmp->GetHeight());
	Bitmap* pNewBitmap = new Bitmap(pBmp->GetWidth(), pBmp->GetHeight(),pBmp->GetPixelFormat());	
	Graphics* pMemGraphics = Graphics::FromImage(pNewBitmap);
	pMemGraphics->DrawImage(pBmp,rc,rcSrc.X,rcSrc.Y,rcSrc.Width,rcSrc.Height,UnitPixel);	
	delete pMemGraphics;	
	return pNewBitmap;
}
Bitmap* CGdiPlusUtils::RotateBitmap(Bitmap* pBmp,double Angle, float originX, float originY)
{
	Rect rc(0,0,pBmp->GetWidth(),pBmp->GetHeight());
	int max = pBmp->GetWidth()>pBmp->GetHeight()?pBmp->GetWidth():pBmp->GetHeight();
	Bitmap* pNewBitmap = new Bitmap(max, max,pBmp->GetPixelFormat());
	Graphics* pMemGraphics = Graphics::FromImage(pNewBitmap);		
	pMemGraphics->TranslateTransform(originX, originX);	
	pMemGraphics->RotateTransform((REAL)Angle);
	pMemGraphics->TranslateTransform(-originX, -originX);
	pMemGraphics->DrawImage(pBmp,rc);	
	delete pMemGraphics;
	return pNewBitmap;
}
Bitmap* CGdiPlusUtils::CloneBitmap(Bitmap *pBmp)
{
	Bitmap* pNewBmp = new Bitmap(pBmp->GetWidth(),pBmp->GetHeight(),pBmp->GetPixelFormat());
	Graphics *graph = Graphics::FromImage(pNewBmp);
	RectF rcf((REAL)0,(REAL)0,(REAL)pBmp->GetWidth(),(REAL)pBmp->GetHeight());
	graph->DrawImage(pBmp,rcf);		
	delete graph;		
	return pNewBmp;
}
Bitmap* CGdiPlusUtils::CloneBitmap(Bitmap *pBmp, float fWidthScaleRatio,float fHeighScaleRatio)
{
	Bitmap* pNewBmp = new Bitmap((INT)(pBmp->GetWidth()*fWidthScaleRatio),(INT)(pBmp->GetHeight()*fHeighScaleRatio),pBmp->GetPixelFormat());
	Graphics *graph = Graphics::FromImage(pNewBmp);		
	RectF rcf((REAL)0,(REAL)0,(REAL)pBmp->GetWidth()*fWidthScaleRatio,(REAL)pBmp->GetHeight()*fHeighScaleRatio);
	graph->DrawImage(pBmp,rcf);		
	delete graph;		
	return pNewBmp;
}
Bitmap* CGdiPlusUtils::ScaleBitmapFitInRect(Bitmap *pBmp, CRect rcBound)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		UINT nBmpWidth = pBmp->GetWidth();
		UINT nBmpHeigh = pBmp->GetHeight();

		float fWRatio = ((float) nBmpWidth) / ((float) rcBound.Width());
		float fHRatio = ((float) nBmpHeigh) / ((float) rcBound.Height());

		float fRate = fHRatio;
		if(fWRatio > fHRatio)
		{
			fRate = fWRatio;
		}

		return CGdiPlusUtils::CloneBitmap(pBmp,1/fWRatio,1/fHRatio);
	}
	return NULL;
}
void CGdiPlusUtils::DrawBitmap(CDC* pDC,Bitmap *pBmp, COLORREF clr, double Angle, CRect rcDisplay)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{		
		Bitmap* pBmpClone = CGdiPlusUtils::CloneBitmap(pBmp);
		if(clr != (COLORREF)-1 && clr != 0x00ecffff &&  clr != 0x00ffffff)
		{
			Color rplColor(255,GetRValue(clr),GetGValue(clr),GetBValue(clr));
			
			CGdiPlusUtils::ReplaceColorMatrix(pBmpClone,rplColor);
		}
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		RectF rf((REAL)rcDisplay.left,(REAL)rcDisplay.top,(REAL)rcDisplay.Width(),(REAL)rcDisplay.Height());

		if(Angle != 0)
		{						
			graph->RotateTransform((REAL)Angle);
			graph->DrawImage(pBmpClone,rf);						
		}
		else
		{
			graph->DrawImage(pBmpClone,rf);
		}

		delete graph;
		delete pBmpClone;
	}
}

void CGdiPlusUtils::DrawBitmapFitInRect(CDC* pDC,Bitmap *pBmp, COLORREF clr, BYTE alpha, CRect rcDisplay)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		UINT nBmpWidth = pBmp->GetWidth();
		UINT nBmpHeigh = pBmp->GetHeight();

		float fWRatio = ((float) nBmpWidth) / ((float) rcDisplay.Width());
		float fHRatio = ((float) nBmpHeigh) / ((float) rcDisplay.Height());

		float fRate = fHRatio;
		if(fWRatio > fHRatio)
		{
			fRate = fWRatio;
		}

		Bitmap* pBmpClone = CGdiPlusUtils::CloneBitmap(pBmp,1/fRate,1/fRate);

		if(clr != (COLORREF)-1 && clr != 0x00ecffff &&  clr != 0x00ffffff)
		{
			Color rplColor(alpha,GetRValue(clr),GetGValue(clr),GetBValue(clr));
			CGdiPlusUtils::ReplaceColorMatrix(pBmpClone,rplColor);
		}
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());

		float fWSpace = rcDisplay.Width() - nBmpWidth/fRate;
		float fHSpace = rcDisplay.Height() - nBmpHeigh/fRate;

		RectF rf((REAL)rcDisplay.left + fWSpace/2,(REAL)rcDisplay.top + fHSpace/2,(REAL)nBmpWidth/fRate,(REAL)nBmpHeigh/fRate);
		 
		graph->DrawImage(pBmpClone,rf);		

		delete graph;
		delete pBmpClone;
	}
}
void CGdiPlusUtils::DrawGrayBitmapFitInRect(CDC* pDC,Bitmap *pBmp, CRect rcDisplay)
{
	if(pBmp != NULL && pBmp->GetLastStatus() == Ok)
	{
		UINT nBmpWidth = pBmp->GetWidth();
		UINT nBmpHeigh = pBmp->GetHeight();

		float fWRatio = ((float) nBmpWidth) / ((float) rcDisplay.Width());
		float fHRatio = ((float) nBmpHeigh) / ((float) rcDisplay.Height());

		float fRate = fHRatio;
		if(fWRatio > fHRatio)
		{
			fRate = fWRatio;
		}

		Bitmap* pBmpClone = CGdiPlusUtils::CloneBitmap(pBmp,1/fRate,1/fRate);

		CGdiPlusUtils::ReplaceGrayColor(pBmpClone);

		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());

		float fWSpace = rcDisplay.Width() - nBmpWidth/fRate;
		float fHSpace = rcDisplay.Height() - nBmpHeigh/fRate;

		RectF rf((REAL)rcDisplay.left + fWSpace/2,(REAL)rcDisplay.top + fHSpace/2,(REAL)nBmpWidth/fRate,(REAL)nBmpHeigh/fRate);
		 
		graph->DrawImage(pBmpClone,rf);		

		delete graph;
		delete pBmpClone;
	}
}

void CGdiPlusUtils::BuildRoundRectPath(CRect rect, int iRoundCornerRadiusTopLeft, int iRoundCornerRadiusTopRight, int iRoundCornerRadiusBottomLeft, int iRoundCornerRadiusBottomRight, GraphicsPath &path)
{		
	path.AddLine(rect.left + iRoundCornerRadiusTopLeft, rect.top, rect.right - iRoundCornerRadiusTopRight, rect.top);		
	path.AddArc(Rect(rect.right-iRoundCornerRadiusTopRight*2, rect.top, iRoundCornerRadiusTopRight*2, iRoundCornerRadiusTopRight*2), 270, 90);
	path.AddLine(rect.right, rect.top + iRoundCornerRadiusTopRight, rect.right, rect.bottom - iRoundCornerRadiusBottomRight);
	path.AddArc(Rect(rect.right-iRoundCornerRadiusBottomRight*2, rect.bottom-iRoundCornerRadiusBottomRight*2, iRoundCornerRadiusBottomRight*2, iRoundCornerRadiusBottomRight*2), 0, 90);
	path.AddLine(rect.right - iRoundCornerRadiusBottomRight, rect.bottom, rect.left + iRoundCornerRadiusBottomLeft, rect.bottom);
	path.AddArc(Rect(rect.left, rect.bottom-iRoundCornerRadiusBottomLeft*2, iRoundCornerRadiusBottomLeft*2, iRoundCornerRadiusBottomLeft*2), 90, 90);
	path.AddLine(rect.left, rect.bottom - iRoundCornerRadiusBottomLeft, rect.left, rect.top + iRoundCornerRadiusTopLeft);
	path.AddArc(Rect(rect.left, rect.top, iRoundCornerRadiusTopLeft*2, iRoundCornerRadiusTopLeft*2), 180, 90);

	path.CloseFigure(); 
}
void CGdiPlusUtils::DrawSmoothBorder(CDC* pDC, CRect rect, COLORREF clrBorder, int iRoundCornerRadius/* = 0*/, int iBorderWidth/* = 0*/)
{
	if(pDC != NULL)
	{
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeHighQuality);
		
		Gdiplus::Color clr;
		clr.SetFromCOLORREF(clrBorder);

		Pen pen(clr, (REAL)iBorderWidth);

		GraphicsPath path;

		BuildRoundRectPath(rect,iRoundCornerRadius,iRoundCornerRadius,iRoundCornerRadius,iRoundCornerRadius,path);
		
		graph->DrawPath(&pen, &path);

		delete graph;
	}
}
void CGdiPlusUtils::DrawSmoothBorder(CDC* pDC, CRect rect, COLORREF clrBorder, int iRoundCornerRadiusTopLeft, int iRoundCornerRadiusTopRight, int iRoundCornerRadiusBottomLeft, int iRoundCornerRadiusBottomRight , int iBorderWidth/* = 0*/)
{
	if(pDC != NULL)
	{
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeAntiAlias);
		
		Gdiplus::Color clr;
		clr.SetFromCOLORREF(clrBorder);

		Pen pen(clr, (REAL)iBorderWidth);

		GraphicsPath path;

		BuildRoundRectPath(rect,iRoundCornerRadiusTopLeft,iRoundCornerRadiusTopRight,iRoundCornerRadiusBottomLeft,iRoundCornerRadiusBottomRight,path);
		
		graph->DrawPath(&pen, &path);

		delete graph;
	}
}
void CGdiPlusUtils::GetRgn(CDC* pDC, CRect rect, int iRoundCornerRadiusTopLeft, int iRoundCornerRadiusTopRight, int iRoundCornerRadiusBottomLeft, int iRoundCornerRadiusBottomRight,CRgn &rgn)
{
	if(pDC != NULL)
	{		
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeAntiAlias);

		GraphicsPath path;

		BuildRoundRectPath(rect,iRoundCornerRadiusTopLeft,iRoundCornerRadiusTopRight,iRoundCornerRadiusBottomLeft,iRoundCornerRadiusBottomRight,path);

		Region region(&path);
		
		HRGN hrgn = region.GetHRGN(graph);

		CRgn rgnTemp;
		rgnTemp.Attach(hrgn);
		if(!rgn.GetSafeHandle())
		{
			rgn.CreateRectRgn(0,0,0,0);
		}
		rgn.CopyRgn(&rgnTemp);

		rgnTemp.DeleteObject();

		delete graph;
	}
}
void CGdiPlusUtils::GetRgn(CDC* pDC, CRect rect, int iRoundCornerRadius,CRgn &rgn)
{
	if(pDC != NULL)
	{		
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeAntiAlias);

		GraphicsPath path;

		BuildRoundRectPath(rect,iRoundCornerRadius,iRoundCornerRadius,iRoundCornerRadius,iRoundCornerRadius,path);

		Region region(&path);
		
		HRGN hrgn = region.GetHRGN(graph);

		CRgn rgnTemp;
		rgnTemp.Attach(hrgn);
		if(!rgn.GetSafeHandle())
		{
			rgn.CreateRectRgn(0,0,0,0);
		}
		rgn.CopyRgn(&rgnTemp);

		rgnTemp.DeleteObject();

		delete graph;
	}
}
void CGdiPlusUtils::GetEllipseRgn(CDC* pDC, CRect rect,CRgn &rgn)
{
	if(pDC != NULL)
	{		
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeAntiAlias);

		GraphicsPath path;
		path.AddEllipse(rect.left,rect.top,rect.Width(),rect.Height());
		
		Region region(&path);
		
		HRGN hrgn = region.GetHRGN(graph);

		CRgn rgnTemp;
		rgnTemp.Attach(hrgn);
		if(!rgn.GetSafeHandle())
		{
			rgn.CreateRectRgn(0,0,0,0);
		}
		rgn.CopyRgn(&rgnTemp);

		rgnTemp.DeleteObject();

		delete graph;
	}
}
void CGdiPlusUtils::FillSmoothRect(CDC* pDC, CRect rect, COLORREF clrFill, COLORREF clrBorder/* = -1*/, int iRoundCornerRadius/* = 0*/, int iBorderWidth/* = 0*/)
{
	if(pDC != NULL)
	{
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeAntiAlias);
		
		GraphicsPath path;

		BuildRoundRectPath(rect,iRoundCornerRadius,iRoundCornerRadius,iRoundCornerRadius,iRoundCornerRadius,path);
		if(clrFill != -1)
		{
			Gdiplus::Color clr;
			clr.SetFromCOLORREF(clrFill);
		
			SolidBrush brush(clr);
			graph->FillPath(&brush,&path);
			Pen pen(clrFill,(REAL)1);      
			graph->DrawPath(&pen, &path);
		}
		if(clrBorder != -1 && iBorderWidth > 0)
		{
			Gdiplus::Color clr;
			clr.SetFromCOLORREF(clrBorder);
			Pen pen(clr,(REAL)iBorderWidth);      
			graph->DrawPath(&pen, &path);
			//DrawSmoothBorder(pDC,rect,clrBorder,iRoundCornerRadius,iBorderWidth);
		}
		//CRgn rgn;
		//rgn.CreateRoundRectRgn(rect.left,rect.top,rect.Width(),rect.Height(),iRoundCornerRadius*2,iRoundCornerRadius*2);
		//Region region((HRGN)rgn.GetSafeHandle());
		//graph->FillRegion(&brush,&region);
		delete graph;
	}
}
void CGdiPlusUtils::DrawImage(CDC* pDC, CRect rect, UINT uResourceID, HINSTANCE hInstance /*= NULL*/)
{
	Bitmap *pBmp;

	if( LoadBitmapFromResource(uResourceID,&pBmp) )
	{
		DrawBitmap(pDC,pBmp,-1,0,rect);
		delete pBmp;
	}
}
void CGdiPlusUtils::DrawImage(CDC* pDC, CRect rect, CString strFilePath)
{
	Bitmap *pBmp;

	if( LoadBitmapFromFile(strFilePath,&pBmp) )
	{
		DrawBitmap(pDC,pBmp,-1,0,rect);
		delete pBmp;
	}
}
void CGdiPlusUtils::DrawTriangle(CDC* pDC, CRect rect, COLORREF clrFill, COLORREF clrBorder/* = -1*/, BOOL bInvert/* =  FALSE*/)
{
	if(pDC)
	{
		GraphicsPath path;
		if(bInvert == FALSE)
		{
			path.AddLine(rect.left + rect.Width()/2, rect.top, rect.right, rect.bottom);		
			path.AddLine(rect.right, rect.bottom, rect.left, rect.bottom);
			path.AddLine(rect.left, rect.bottom, rect.left + rect.Width()/2, rect.top);	
			path.CloseFigure();
		}
		else
		{
			path.AddLine(rect.left, rect.top, rect.right, rect.top);		
			path.AddLine(rect.right, rect.top, rect.left + rect.Width()/2, rect.bottom);
			path.AddLine(rect.left + rect.Width()/2, rect.bottom, rect.left, rect.top);	
			path.CloseFigure();
		}
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		graph->SetSmoothingMode(SmoothingModeAntiAlias);
		if(clrFill != -1)
		{
			Gdiplus::Color clr;
			clr.SetFromCOLORREF(clrFill);
		
			SolidBrush brush(clr);
			graph->FillPath(&brush,&path);
			Pen pen(clrFill,(REAL)1);      
			graph->DrawPath(&pen, &path);
		}
		if(clrBorder != -1)
		{
			Gdiplus::Color clr;
			clr.SetFromCOLORREF(clrBorder);
			Pen pen(clr,(REAL)1);      
			graph->DrawPath(&pen, &path);
		}

		delete graph;
	}
}
void CGdiPlusUtils::DrawDashLine(CDC* pDC, COLORREF clr, int iWidth, int iWeight, CPoint pt1, CPoint pt2)
{
	if(pDC)
	{
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		if(graph)
		{
			graph->SetSmoothingMode(SmoothingModeAntiAlias);
			REAL dashValues[4] = {iWidth, iWidth*2, iWidth, iWidth*2};
			Gdiplus::Color color;
			color.SetFromCOLORREF(clr);

			Pen pen(color, iWeight);
			pen.SetDashPattern(dashValues, 4);
			graph->DrawLine(&pen, pt1.x,pt1.y,pt2.x,pt2.y);
		}
	}
}
void CGdiPlusUtils::DrawFrameEllipse(CDC* pDC, CRect rect, COLORREF clr, int iWeight)
{
	if(pDC)
	{
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		if(graph)
		{
			graph->SetSmoothingMode(SmoothingModeAntiAlias);
			Gdiplus::Color color;
			color.SetFromCOLORREF(clr);

			Pen pen(color, iWeight);
			graph->DrawEllipse(&pen, rect.left,rect.top,rect.Width(),rect.Height());
		}
	}
}
void CGdiPlusUtils::FillEllipse(CDC* pDC, CRect rect, COLORREF clr)
{
	if(pDC)
	{
		Graphics *graph = Graphics::FromHDC(pDC->GetSafeHdc());
		if(graph)
		{
			graph->SetSmoothingMode(SmoothingModeAntiAlias);
			Gdiplus::Color color;
			color.SetFromCOLORREF(clr);

			SolidBrush brush(color);
			graph->FillEllipse(&brush, rect.left,rect.top,rect.Width(),rect.Height());
		}
	}
}