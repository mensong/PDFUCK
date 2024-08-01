#include "pch.h"
#include "PDF_TEXTPAGE_imp.h"
#include "PDF_FINDER_imp.h"
#include "PDF_PAGELINK_imp.h"
#include "PDF_TEXTCHAR_RTREE_imp.h"
#include <fpdf_searchex.h>

// Í¨¹ý PDF_TEXTPAGE ¼Ì³Ð
int PDF_TEXTPAGE_imp::CountChars()
{
	return FPDFText_CountChars(m_textPage);
}

int PDF_TEXTPAGE_imp::GetCharIndexAtPos(float x, float y, float xTolerance, float yTolerance)
{
	return FPDFText_GetCharIndexAtPos(m_textPage, x, y, xTolerance, yTolerance);
}

wchar_t PDF_TEXTPAGE_imp::GetChar(int index)
{
	return FPDFText_GetUnicode(m_textPage, index);
}

float PDF_TEXTPAGE_imp::GetCharFontSize(int index)
{
	return FPDFText_GetFontSize(m_textPage, index);
}

unsigned long PDF_TEXTPAGE_imp::GetCharFontInfo(int index, char* bufferFontNameUtf8, unsigned long buflen, int* flags)
{
	return FPDFText_GetFontInfo(m_textPage, index, bufferFontNameUtf8, buflen, flags);
}

int PDF_TEXTPAGE_imp::GetCharFontWeight(int index)
{
	return FPDFText_GetFontWeight(m_textPage, index);
}

PDF_TEXT_RENDERMODE PDF_TEXTPAGE_imp::GetCharTextRenderMode(int index)
{
	return (PDF_TEXT_RENDERMODE)FPDFText_GetTextRenderMode(m_textPage, index);
}

bool PDF_TEXTPAGE_imp::GetCharFillColor(int index, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A)
{
	unsigned int _R, _G, _B, _A;
	if (!FPDFText_GetFillColor(m_textPage, index, &_R, &_G, &_B, &_A))
		return false;
	*R = _R;
	*G = _G;
	*B = _B;
	*A = _A;
	return true;
}

bool PDF_TEXTPAGE_imp::GetCharStrokeColor(int index, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A)
{
	unsigned int _R, _G, _B, _A;
	if (!FPDFText_GetStrokeColor(m_textPage, index, &_R, &_G, &_B, &_A))
		return false;
	*R = _R;
	*G = _G;
	*B = _B;
	*A = _A;
	return true;
}

float PDF_TEXTPAGE_imp::GetCharAngle(int index)
{
	return FPDFText_GetCharAngle(m_textPage, index);
}

bool PDF_TEXTPAGE_imp::GetCharBox(int index, float* left, float* right, float* bottom, float* top)
{
	double _left, _right, _bottom, _top;
	if (!FPDFText_GetCharBox(m_textPage, index, &_left, &_right, &_bottom, &_top))
		return false;
	*left = _left;
	*right = _right;
	*bottom = _bottom;
	*top = _top;
	return true;
}

bool PDF_TEXTPAGE_imp::GetCharLooseCharBox(int index, float* left, float* right, float* bottom, float* top)
{
	FS_RECTF rect;
	memset(&rect, 0, sizeof(rect));
	if (!FPDFText_GetLooseCharBox(m_textPage, index, &rect))
		return false;
	*left = rect.left;
	*right = rect.right;
	*bottom = rect.bottom;
	*top = rect.top;
	return true;
}

bool PDF_TEXTPAGE_imp::GetCharMatrix(int index, float* a, float* b, float* c, float* d, float* e, float* f)
{
	FS_MATRIX mat;
	if (!FPDFText_GetMatrix(m_textPage, index, &mat))
		return false;
	*a = mat.a;
	*b = mat.b;
	*c = mat.c;
	*d = mat.d;
	*e = mat.e;
	*f = mat.f;
	return true;
}

bool PDF_TEXTPAGE_imp::GetCharOrigin(int index, float* x, float* y)
{
	double _x, _y;
	if (!FPDFText_GetCharOrigin(m_textPage, index, &_x, &_y))
		return false;
	*x = _x;
	*y = _y;
	return true;
}

int PDF_TEXTPAGE_imp::GetText(int start_index, int count, wchar_t* resultBuff)
{
	return FPDFText_GetText(m_textPage, start_index, count, (unsigned short*)resultBuff);
}

int PDF_TEXTPAGE_imp::CountRects(int start_index/* = 0*/, int count/* = 0*/)
{
	if (count <= 0)
		count = INT_MAX;
	return FPDFText_CountRects(m_textPage, start_index, count);
}

bool PDF_TEXTPAGE_imp::GetRect(int rect_index, float* left, float* top, float* right, float* bottom)
{
	double _left, _top, _right, _bottom;
	if (!FPDFText_GetRect(m_textPage, rect_index, &_left, &_top, &_right, &_bottom))
		return false;

	*left = _left;
	*top = _top;
	*right = _right;
	*bottom = _bottom;
	return true;
}

int PDF_TEXTPAGE_imp::GetTextByRect(float left, float top, float right, float bottom, wchar_t* buffer, int buflen)
{
	return FPDFText_GetBoundedText(m_textPage, left, top, right, bottom, (unsigned short*)buffer, buflen);
}

PDF_FINDER* PDF_TEXTPAGE_imp::FindStart(wchar_t* findwhat, unsigned long flags, int start_index)
{
	auto f = FPDFText_FindStart(m_textPage, (FPDF_WIDESTRING)findwhat, flags, start_index);
	if (!f)
		return NULL;
	return new PDF_FINDER_imp(f);
}

void PDF_TEXTPAGE_imp::FindClose(PDF_FINDER** finder)
{
	PDF_FINDER_imp* ifinder = IMP(PDF_FINDER, *finder);
	FPDFText_FindClose(ifinder->m_finder);
	*finder = NULL;
}

PDF_PAGELINK* PDF_TEXTPAGE_imp::OpenWebLinks()
{
	auto pl = FPDFLink_LoadWebLinks(m_textPage);
	if (!pl)
		return NULL;
	return new PDF_PAGELINK_imp(pl);
}

void PDF_TEXTPAGE_imp::CloseWebLinks(PDF_PAGELINK** link_page)
{
	PDF_PAGELINK_imp* ilink_page = IMP(PDF_PAGELINK, *link_page);
	FPDFLink_CloseWebLinks(ilink_page->m_pageLink);
	*link_page = NULL;
}

PDF_TEXTCHAR_RTREE* PDF_TEXTPAGE_imp::NewTextCharRTree()
{
	return new PDF_TEXTCHAR_RTREE_imp(this);
}

void PDF_TEXTPAGE_imp::CloseTextCharRTree(PDF_TEXTCHAR_RTREE** rt)
{
	delete IMP(PDF_TEXTCHAR_RTREE, *rt);
	*rt = NULL;
}

int PDF_TEXTPAGE_imp::GetCharIndexFromTextIndex(int nTextIndex)
{
	return FPDFText_GetCharIndexFromTextIndex(m_textPage, nTextIndex);
}

int PDF_TEXTPAGE_imp::GetTextIndexFromCharIndex(int nCharIndex)
{
	return FPDFText_GetTextIndexFromCharIndex(m_textPage, nCharIndex);
}
