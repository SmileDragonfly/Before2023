
// MFCApplication1Doc.h : interface of the CMFCApplication1Doc class
//


#pragma once
#include "GraphicItem.h"

class CMFCApplication1Doc : public CDocument
{
protected: // create from serialization only
	CMFCApplication1Doc() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMFCApplication1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    CString GetData();
    void SetData(CString);
protected:
    CString m_strData;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

// Custom
public:
    // Functions
    BOOL GridLeftView(unsigned int row, unsigned int column);
    void UpdateLeftFrameRect(CRect size);
    void UpdateData();
    unsigned int GetLeftFrameRow();
    unsigned int GetLefttFrameColumn();

    // Variables
    CArray<CGraphicItem*>   m_arrGraphicItem;;

protected:
    // Function

    // Variables
    unsigned int            m_row;
    unsigned int            m_column;
    CRect                   m_rectLeftFrame;
};
