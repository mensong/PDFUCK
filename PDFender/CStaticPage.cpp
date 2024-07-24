#include "pch.h"
#include "CStaticPage.h"

IMPLEMENT_DYNAMIC(CStaticPage, CStatic)

BEGIN_MESSAGE_MAP(CStaticPage, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CStaticPage::CStaticPage()
	: m_pdfPage(NULL)
{

}

CStaticPage::~CStaticPage()
{
	m_pdfPage = NULL;
}

void CStaticPage::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	//绘制背景
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	if (m_pdfPage)
	{
		m_pdfPage->RenderToDC(dc.GetSafeHdc(),
			0, 0, rect.Width(), rect.Height(), 
			PDF_PAGE::PAGE_RATEION_0, 0);
	}
}
