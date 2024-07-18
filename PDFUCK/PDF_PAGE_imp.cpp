#include "pch.h"
#include "PDF_PAGE_imp.h"
#include "PDF_BITMAP_imp.h"
#include "PDF_PAGEOBJECT_imp.h"
#include "PDF_TEXTPAGE_imp.h"
#include <fpdf_edit.h>


// 通过 PDF_PAGE 继承
double PDF_PAGE_imp::GetWidth()
{
	return FPDF_GetPageWidth(m_page);
}


// 通过 PDF_PAGE 继承
double PDF_PAGE_imp::GetHeight()
{
	return FPDF_GetPageHeight(m_page);
}


// 通过 PDF_PAGE 继承
void PDF_PAGE_imp::RenderToDC(HDC dc,
	int start_x, int start_y, int size_x, int size_y, int rotate, int flags)
{
	FPDF_RenderPage(dc, m_page, start_x, start_y, size_x, size_y, rotate, flags);
}

void PDF_PAGE_imp::RenderToBitmap(PDF_BITMAP* bitmap,
	int start_x, int start_y, int size_x, int size_y, int rotate, int flags)
{
	PDF_BITMAP_imp* bp = dynamic_cast<PDF_BITMAP_imp*>(bitmap);
	FPDF_RenderPageBitmap(bp->m_bitmap, m_page, start_x, start_y, size_x, size_y, rotate, flags);
}

// 通过 PDF_PAGE 继承
int PDF_PAGE_imp::GetRotation()
{
	return FPDFPage_GetRotation(m_page);
}

void PDF_PAGE_imp::SetRotation(int rotate)
{
	FPDFPage_SetRotation(m_page, rotate);
}

bool PDF_PAGE_imp::HasTransparency()
{
	return FPDFPage_HasTransparency(m_page);
}

void PDF_PAGE_imp::TransformAllAnnots(double a, double b, double c, double d, double e, double f)
{
	FPDFPage_TransformAnnots(m_page, a, b, c, d, e, f);
}

void PDF_PAGE_imp::InsertPageObject(PDF_PAGEOBJECT* pageObj)
{
	auto i = IMP(PDF_PAGEOBJECT, pageObj);
	FPDFPage_InsertObject(m_page, i->m_obj);
}

int PDF_PAGE_imp::CountPageObjects()
{
	return FPDFPage_CountObjects(m_page);
}

PDF_PAGEOBJECT* PDF_PAGE_imp::OpenPageObject(int idx)
{
	auto o = FPDFPage_GetObject(m_page, idx);
	if (!o)
		return NULL;
	return new PDF_PAGEOBJECT_imp(o);
}

void PDF_PAGE_imp::ClosePageObject(PDF_PAGEOBJECT** pageObj)
{
	PDF_PAGEOBJECT_imp* ipageObj = IMP(PDF_PAGEOBJECT, *pageObj);
	delete ipageObj;
	*pageObj = NULL;
}

// 通过 PDF_PAGE 继承
bool PDF_PAGE_imp::RemovePageObject(PDF_PAGEOBJECT* pageObj)
{
	return FPDFPage_RemoveObject(m_page, IMP(PDF_PAGEOBJECT, pageObj)->m_obj);
}

PDF_TEXTPAGE* PDF_PAGE_imp::OpenTextPage()
{
	auto textPage = FPDFText_LoadPage(m_page);
	if (!textPage)
		return NULL;
	return new PDF_TEXTPAGE_imp(textPage);
}

void PDF_PAGE_imp::CloseTextPage(PDF_TEXTPAGE** text_page)
{
	auto itextPage = IMP(PDF_TEXTPAGE, *text_page);
	delete itextPage;
	*text_page = NULL;
}


// 通过 PDF_PAGE 继承
bool PDF_PAGE_imp::GetBoundingBox(float* left, float* top, float* right, float* bottom)
{
	FS_RECTF rect;
	memset(&rect, 0, sizeof(rect));
	if (!FPDF_GetPageBoundingBox(m_page, &rect))
		return false;
	*left = rect.left;
	*top = rect.top;
	*right = rect.right;
	*bottom = rect.bottom;
	return true;
}


// 通过 PDF_PAGE 继承
bool PDF_PAGE_imp::CommitChange()
{
	return FPDFPage_GenerateContent(m_page);
}