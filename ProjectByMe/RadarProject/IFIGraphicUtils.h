//////////////////////////////////////////////////////////////////////////////////////////////////
// File: IFIGraphicUtils.h																		//
// Class: CIFIGraphicUtils																		//
// Description: Define of class CIFIGraphicUtils												//
// Date: 2018/08																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _IFIGRAPHICUTILS_H
#define _IFIGRAPHICUTILS_H

#pragma once
#ifdef _DLL_IFI_UISTYLE_2018CONTROLS
class __declspec(dllexport) CIFIGraphicUtils
#else
class __declspec(dllimport) CIFIGraphicUtils
#endif
{
public:
	CIFIGraphicUtils(void);
	~CIFIGraphicUtils(void);
	static void CalculRectangles(RECT &Source, RECT &Dest, BOOL Maximize);
	static void DrawGradient(CDC *pDC, CRect r, COLORREF clrBegin, COLORREF clrEnd, BOOL bVertical);
	static COLORREF Darker(COLORREF crBase, float fFactor);
	static COLORREF Lighter(COLORREF crBase, float fFactor);
	static COLORREF Gray(COLORREF crBase);
	static HICON LoadIcon(UINT nIDIcon, CSize size=CSize(0,0));
	static void DrawIcon(CDC* pDC, LPCRECT LeRect, HICON LIcone);
	static COLORREF InvertColor(COLORREF clrIn);
	static COLORREF IdealTextColor(COLORREF clrIn);	
};

#endif