#include "pch.h"
#include "PDF_PAGE_imp.h"
#include "PDF_BITMAP_imp.h"
#include "PDF_PAGEOBJECT_imp.h"
#include "PDF_TEXTPAGE_imp.h"
#include "PDF_PAGEOBJECT_RTREE_imp.h"
#include "PDF_CLIPPATH_imp.h"
#include <fpdf_edit.h>
#include <cpdfsdk_helpers.h>
#include <fpdf_transformpage.h>


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

void PDF_PAGE_imp::SetMediaBox(float left, float bottom, float right, float top)
{
	FPDFPage_SetMediaBox(m_page, left, bottom, right, top);
}

void PDF_PAGE_imp::SetCropBox(float left, float bottom, float right, float top)
{
	FPDFPage_SetCropBox(m_page, left, bottom, right, top);
}

void PDF_PAGE_imp::SetBleedBox(float left, float bottom, float right, float top)
{
	FPDFPage_SetBleedBox(m_page, left, bottom, right, top);
}

void PDF_PAGE_imp::SetTrimBox(float left, float bottom, float right, float top)
{
	FPDFPage_SetTrimBox(m_page, left, bottom, right, top);
}

void PDF_PAGE_imp::SetArtBox(float left, float bottom, float right, float top)
{
	FPDFPage_SetArtBox(m_page, left, bottom, right, top);
}

bool PDF_PAGE_imp::GetMediaBox(float* left, float* bottom, float* right, float* top)
{
	return FPDFPage_GetMediaBox(m_page, left, bottom, right, top);
}

bool PDF_PAGE_imp::GetCropBox(float* left, float* bottom, float* right, float* top)
{
	return FPDFPage_GetCropBox(m_page, left, bottom, right, top);
}

bool PDF_PAGE_imp::GetBleedBox(float* left, float* bottom, float* right, float* top)
{
	return FPDFPage_GetBleedBox(m_page, left, bottom, right, top);
}

bool PDF_PAGE_imp::GetTrimBox(float* left, float* bottom, float* right, float* top)
{
	return FPDFPage_GetTrimBox(m_page, left, bottom, right, top);
}

bool PDF_PAGE_imp::GetArtBox(float* left, float* bottom, float* right, float* top)
{
	return FPDFPage_GetArtBox(m_page, left, bottom, right, top);
}

bool PDF_PAGE_imp::TransformWithClip(const PDF_MATRIX* matrix, const PDF_RECT* clipRect)
{
	FS_MATRIX mat;
	FS_RECTF rect;
	FS_MATRIX* pMap = NULL;
	FS_RECTF* pRect = NULL;
	if (matrix)
	{
		mat.a = matrix->a;
		mat.b = matrix->b;
		mat.c = matrix->c;
		mat.d = matrix->d;
		mat.e = matrix->e;
		mat.f = matrix->f;
		pMap = &mat;
	}
	if (clipRect)
	{
		rect.left = clipRect->left;
		rect.right = clipRect->right;
		rect.bottom = clipRect->bottom;
		rect.top = clipRect->top;
		pRect = &rect;
	}

	return FPDFPage_TransFormWithClip(m_page, pMap, pRect);
}

PDF_CLIPPATH* PDF_PAGE_imp::CreateClipPath(float left, float bottom, float right, float top)
{
	auto o = FPDF_CreateClipPath(left, bottom, right, top);
	if (!o)
		return NULL;
	return new PDF_CLIPPATH_imp(o);
}

void PDF_PAGE_imp::InsertClipPath(PDF_CLIPPATH* clipPath)
{
	FPDFPage_InsertClipPath(m_page, IMP(PDF_CLIPPATH, clipPath)->m_clipPath);
}

void PDF_PAGE_imp::CloseClipPath(PDF_CLIPPATH** clipPath)
{
	delete IMP(PDF_CLIPPATH, *clipPath);
	*clipPath = NULL;
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