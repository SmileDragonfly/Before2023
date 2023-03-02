//////////////////////////////////////////////////////////////////////////////////////////////////
// File: GdiPlusUtils.h																			//
// Class: CGdiPlusUtils																			//
// Description: Define class CGdiPlusUtils														//
// Date: 2018/08																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CGDIPLUSUTILS_H_
#define _CGDIPLUSUTILS_H_

#pragma once

class CGdiPlusUtils
{
public:
	CGdiPlusUtils(void);
	~CGdiPlusUtils(void);

	static bool LoadBitmapFromResource(UINT uResourceID, Bitmap** ppBitmapOut, HINSTANCE hInstance = NULL);
	static bool LoadBitmapFromFile(CString strFilePath, Bitmap** ppBitmapOut);

	static BYTE GetBrightestColor(Bitmap& bmp);
	static void GetGraphicsPath(Bitmap& bitmap,GraphicsPath& graphicsPath);

	static void ReplaceColor(Bitmap* pBmp, Color colorRpl);
	static void ReplaceColorMatrix(Bitmap* pBmp, Color clr);
	static void ReplaceGrayColor(Bitmap* pBmp);
	static void GetRegion(Bitmap *pBmp, double Angle, RectF ScaleInRect, bool bFit, CRgn& rgn);
	static void GetRegion(Bitmap *pBmp, double Angle, CRect ScaleInRect, bool bFit, CRgn& rgn);
	static void GetRegion(Bitmap *pBmp, CRect ScaleInRect, bool bFit, CRgn& rgn);
	static void GetRect(Bitmap *pBmp, CRect& rcOut);
	static void RecalcRect(Bitmap *pBmp, double Angle, CRect ScaleInRect, CRect& rcOut);

	static Bitmap* MirrorBitmap(Bitmap *pBmp);
	static Bitmap* RotateBitmap(Bitmap* pBmp,double Angle, float originX, float originY);
	static Bitmap* CloneBitmap(Bitmap *pBmp);
	static Bitmap* CloneBitmap(Bitmap *pBmp, float fWidthScaleRatio,float fHeighScaleRatio);
	static Bitmap* ScaleBitmapFitInRect(Bitmap *pBmp, CRect rcBound);

	static void DrawBitmap(CDC* pDC,Bitmap *pBmp, COLORREF clr, double Angle, CRect rcDisplay);	
	static void DrawBitmapFitInRect(CDC* pDC,Bitmap *pBmp, COLORREF clr, BYTE alpha, CRect rcDisplay);
	static void DrawGrayBitmapFitInRect(CDC* pDC,Bitmap *pBmp, CRect rcDisplay);
	static void BuildRoundRectPath(CRect rect, int iRoundCornerRadiusTopLeft, int iRoundCornerRadiusTopRight, int iRoundCornerRadiusBottomLeft, int iRoundCornerRadiusBottomRight, GraphicsPath &path);
	static void GetRgn(CDC* pDC, CRect rect, int iRoundCornerRadiusTopLeft, int iRoundCornerRadiusTopRight, int iRoundCornerRadiusBottomLeft, int iRoundCornerRadiusBottomRight,CRgn &rgn);	
	static void GetRgn(CDC* pDC, CRect rect, int iRoundCornerRadius,CRgn &rgn);
	static void GetEllipseRgn(CDC* pDC, CRect rect,CRgn &rgn);
	static void DrawSmoothBorder(CDC* pDC, CRect rect, COLORREF clrBorder, int iRoundCornerRadius = 0, int iBorderWidth = 0);
	static void DrawSmoothBorder(CDC* pDC, CRect rect, COLORREF clrBorder, int iRoundCornerRadiusTopLeft, int iRoundCornerRadiusTopRight, int iRoundCornerRadiusBottomLeft, int iRoundCornerRadiusBottomRight , int iBorderWidth = 0);
	static void FillSmoothRect(CDC* pDC, CRect rect, COLORREF clrFill, COLORREF clrBorder = -1, int iRoundCornerRadius = 0, int iBorderWidth = 0);
	static void DrawImage(CDC* pDC, CRect rect, UINT uResourceID, HINSTANCE hInstance = NULL);
	static void DrawImage(CDC* pDC, CRect rect, CString strFilePath);
	static void DrawTriangle(CDC* pDC, CRect rect, COLORREF clrFill, COLORREF clrBorder = -1, BOOL bInvert =  FALSE);
	static void DrawDashLine(CDC* pDC, COLORREF clr, int iWidth, int iWeight, CPoint pt1, CPoint pt2);
	static void DrawFrameEllipse(CDC* pDC, CRect rect, COLORREF clr, int iWeight);
	static void FillEllipse(CDC* pDC, CRect rect, COLORREF clr);
	
};

#endif

