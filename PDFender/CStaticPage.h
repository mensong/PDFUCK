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

	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnPaint();
};

