#include "pch.h"
#include "CStaticPage.h"

IMPLEMENT_DYNAMIC(CStaticPage, CStatic)

BEGIN_MESSAGE_MAP(CStaticPage, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CStaticPage::CStaticPage()
	: m_pdfPage(NULL)
	, m_drawPageBackgroup(true)
	, m_doubleBuffer(true)
	, m_bgkColor(RGB(255, 255, 255))
{

}

CStaticPage::~CStaticPage()
{
	m_pdfPage = NULL;
}

void CStaticPage::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	//���Ʊ���
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

		//����
		if (m_drawPageBackgroup)
		{
			CRect rectCanvas;
			rectCanvas.left = renderX;
			rectCanvas.right = renderX + renderWidth;
			rectCanvas.top = renderY;
			rectCanvas.bottom = renderY + renderHeight;
			dc.FillSolidRect(rectCanvas, m_bgkColor);
		}		

		//����
		if (m_doubleBuffer)
		{
			m_pdfPage->RenderToDC_DoubleBuffer(dc.GetSafeHdc(),
				renderX, renderY, renderWidth, renderHeight,
				PDF_PAGE::PAGE_RATEION_0, m_bgkColor, 0);
		}
		else
		{
			m_pdfPage->RenderToDC(dc.GetSafeHdc(),
				renderX, renderY, renderWidth, renderHeight,
				PDF_PAGE::PAGE_RATEION_0, 0);
		}
	}
}

void CStaticPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);
}

void CStaticPage::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);
}

void CStaticPage::OnMouseMove(UINT nFlags, CPoint point)
{

	__super::OnMouseMove(nFlags, point);
}

BOOL CStaticPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
