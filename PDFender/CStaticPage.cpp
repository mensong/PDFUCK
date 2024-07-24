#include "pch.h"
#include "CStaticPage.h"

IMPLEMENT_DYNAMIC(CStaticPage, CStatic)

BEGIN_MESSAGE_MAP(CStaticPage, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CStaticPage::CStaticPage()
	: m_pdfPage(NULL)
	, m_drawPageBackgroup(true)
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
	dc.FillSolidRect(rect, RGB(0xcc, 0xcc, 0xcc));

	if (m_pdfPage)
	{
		int renderX, renderY, renderWidth, renderHeight;

		int pageWidth = m_pdfPage->GetWidth();
		int pageHeight = m_pdfPage->GetHeight();
		int dcWidth = rect.Width();
		int dcHeight = rect.Height();
		float pageFactor = pageWidth / (float)pageHeight;
		float dcFactor = dcWidth / (float)dcHeight;
		if (pageFactor > dcFactor)
		{
			renderWidth = dcWidth;
			renderHeight = pageHeight / ((float)pageWidth / dcWidth );
		}
		else
		{
			renderWidth = pageWidth / ((float)pageHeight / dcHeight);
			renderHeight = dcHeight;
		}

		renderX = (dcWidth - renderWidth) / 2.0f;
		renderY = (dcHeight - renderHeight) / 2.0f;

		//画布
		if (m_drawPageBackgroup)
		{
			CRect rectCanvas;
			rectCanvas.left = renderX;
			rectCanvas.right = renderX + renderWidth;
			rectCanvas.top = renderY;
			rectCanvas.bottom = renderY + renderHeight;
			dc.FillSolidRect(rectCanvas, RGB(255, 255, 255));
		}		

		//内容
		m_pdfPage->RenderToDC(dc.GetSafeHdc(),
			renderX, renderY, renderWidth, renderHeight,
			PDF_PAGE::PAGE_RATEION_0, 0);
	}
}
