#pragma once
#include "afxwin.h"
#include "../PDFUCK/PDFuck.h"

class CStaticPage :
    public CStatic
{
    DECLARE_DYNAMIC(CStaticPage)

public:
    CStaticPage();
    ~CStaticPage();

	PDF_PAGE* m_pdfPage;
	void SetPage(PDF_PAGE* page) { m_pdfPage = page; }

	bool m_drawPageBackgroup;
	void SetDrawPageBackgroup(bool draw) { m_drawPageBackgroup = draw; }

	bool m_doubleBuffer;
	void SetEnableDoubleBuffer(bool doubleBuffer) { m_doubleBuffer = doubleBuffer; }

	COLORREF m_bgkColor;
	void SetBackgroundColor(COLORREF color) { m_bgkColor = color; }

	DECLARE_MESSAGE_MAP()

public:	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

