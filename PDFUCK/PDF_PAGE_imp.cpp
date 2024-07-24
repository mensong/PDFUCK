#include "pch.h"
#include "PDF_PAGE_imp.h"
#include "PDF_BITMAP_imp.h"
#include "PDF_PAGEOBJECT_imp.h"
#include "PDF_TEXTPAGE_imp.h"
#include "PDF_PAGEOBJECT_RTREE_imp.h"
#include <fpdf_edit.h>
#include <cpdfsdk_helpers.h>


// 通过 PDF_PAGE 继承
float PDF_PAGE_imp::GetWidth()
{
	//PAGE_RATEION rotate = GetRotation();
	//if (rotate == PAGE_RATEION::PAGE_RATEION_90 ||
	//	rotate == PAGE_RATEION::PAGE_RATEION_270)
	//{
	//	return FPDF_GetPageHeight(m_page);
	//}

	return FPDF_GetPageWidth(m_page);
}


// 通过 PDF_PAGE 继承
float PDF_PAGE_imp::GetHeight()
{
	//int rotate = GetRotation();
	//if (rotate == PAGE_RATEION::PAGE_RATEION_90 ||
	//	rotate == PAGE_RATEION::PAGE_RATEION_270)
	//{
	//	return FPDF_GetPageWidth(m_page);
	//}

	return FPDF_GetPageHeight(m_page);
}


// 通过 PDF_PAGE 继承
void PDF_PAGE_imp::RenderToDC(HDC dc,
	int x_inDC, int y_inDC, int size_x_inDC, int size_y_inDC, PAGE_RATEION rotate_inDC, int flags)
{
	FPDF_RenderPage(dc, m_page, x_inDC, y_inDC, size_x_inDC, size_y_inDC, rotate_inDC, flags);
}

void PDF_PAGE_imp::RenderToBitmap(PDF_BITMAP* bitmap,
	int x_inBitmap, int y_inBitmap, int size_x_inBitmap, int size_y_inBitmap, 
	PAGE_RATEION rotate_inBitmap, int flags)
{
	PDF_BITMAP_imp* bp = dynamic_cast<PDF_BITMAP_imp*>(bitmap);
	FPDF_RenderPageBitmap(bp->m_bitmap, m_page, x_inBitmap, y_inBitmap, 
		size_x_inBitmap, size_y_inBitmap, rotate_inBitmap, flags);
}

void PDF_PAGE_imp::RenderToBitmapEx(PDF_BITMAP* bitmap,
	float a, float b, float c, float d, float e, float f,
	float left_inBitmap, float top_inBitmap, float right_inBitmap, float bottom_inBitmap, int flags)
{
	FS_MATRIX mat;
	mat.a = a;
	mat.b = b;
	mat.c = c;
	mat.d = d;
	mat.e = e;
	mat.f = f;
	FS_RECTF clipping;
	clipping.left = left_inBitmap;
	clipping.top = top_inBitmap;
	clipping.right = right_inBitmap;
	clipping.bottom = bottom_inBitmap;
	FPDF_RenderPageBitmapWithMatrix(IMP(PDF_BITMAP, bitmap)->m_bitmap,
		m_page, &mat, &clipping, flags);
}

PDF_PAGEOBJECT_RTREE* PDF_PAGE_imp::NewRTree()
{
	return new PDF_PAGEOBJECT_RTREE_imp();
}

void PDF_PAGE_imp::CloseRTree(PDF_PAGEOBJECT_RTREE** rt)
{
	delete IMP(PDF_PAGEOBJECT_RTREE, *rt);
	*rt = NULL;
}

// 通过 PDF_PAGE 继承
PDF_PAGE::PAGE_RATEION PDF_PAGE_imp::GetRotation()
{
	return (PAGE_RATEION)FPDFPage_GetRotation(m_page);
}

void PDF_PAGE_imp::SetRotation(PAGE_RATEION rotate)
{
	FPDFPage_SetRotation(m_page, (int)rotate);
}

bool PDF_PAGE_imp::HasTransparency()
{
	return FPDFPage_HasTransparency(m_page);
}

void PDF_PAGE_imp::TransformAllAnnots(float a, float b, float c, float d, float e, float f)
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