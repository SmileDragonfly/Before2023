//////////////////////////////////////////////////////////////////////////////////////////////////
// File: IFIStatic.h																			//
// Class: IFIStatic																				//
// Description: Define of class CIFIStatic														//
// Date: 2018/11																				//
// Created: tcbinh																				//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _IFISTATIC_H
#define _IFISTATIC_H

#pragma once

#include "IFISkinProperties.h"

#ifdef _DLL_IFI_UISTYLE_2018CONTROLS
class __declspec(dllexport) CIFIStatic : public CStatic
#else
class __declspec(dllimport) CIFIStatic : public CStatic
#endif
{
	CBitmap m_bgrBitmap;
	DECLARE_DYNAMIC(CIFIStatic)

public:
	class CSkinProperties : public CIFISkinProperties
	{
	public:
		UINT		m_nRoundCornerRadius;		
		INT			m_nPaddingLeft;
		INT			m_nPaddingTop;
		INT			m_nPaddingRight;
		INT			m_nPaddingBottom;	

		CSkinProperties(CWnd* pWndOwner);
		~CSkinProperties();
		void SetRoundCornerRadius(UINT nRadius);		
		void SetPadding(UINT left, UINT top, UINT right, UINT bottom);
	} m_SkinProperties;

	enum ESkinType
	{
		Label,
		Label_HighLight,
		Label_Subtitle,
		Label_Big,
		Static_Value,
		Static_Value_Big,
		Group_Subtitle,
		Description,
		TypeCount
	} m_SkinType;
public:
	CIFIStatic(void);
	~CIFIStatic(void);

	void SetSkinType(ESkinType eSkinType);

	CSize GetContentSize();

	void UpdateSkin(const TSkinLabel* pSkinLabel = NULL);
protected:
	virtual void PreSubclassWindow();
	
public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif