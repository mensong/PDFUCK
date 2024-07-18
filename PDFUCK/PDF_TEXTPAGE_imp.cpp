#include "pch.h"
#include "PDF_TEXTPAGE_imp.h"
#include "PDF_FINDER_imp.h"
#include "PDF_PAGELINK_imp.h"

// Í¨¹ý PDF_TEXTPAGE ¼Ì³Ð
int PDF_TEXTPAGE_imp::CountChars()
{
	return FPDFText_CountChars(m_textPage);
}

int PDF_TEXTPAGE_imp::GetCharIndexAtPos(double x, double y, double xTolerance, double yTolerance)
{
	return FPDFText_GetCharIndexAtPos(m_textPage, x, y, xTolerance, yTolerance);
}

wchar_t PDF_TEXTPAGE_imp::GetChar(int index)
{
	return FPDFText_GetUnicode(m_textPage, index);
}

double PDF_TEXTPAGE_imp::GetCharFontSize(int index)
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

bool PDF_TEXTPAGE_imp::GetCharFillColor(int index, unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A)
{
	return FPDFText_GetFillColor(m_textPage, index, R, G, B, A);
}

bool PDF_TEXTPAGE_imp::GetCharStrokeColor(int index, unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A)
{
	return FPDFText_GetStrokeColor(m_textPage, index, R, G, B, A);
}

float PDF_TEXTPAGE_imp::GetCharAngle(int index)
{
	return FPDFText_GetCharAngle(m_textPage, index);
}

bool PDF_TEXTPAGE_imp::GetCharBox(int index, double* left, double* right, double* bottom, double* top)
{
	return FPDFText_GetCharBox(m_textPage, index, left, right, bottom, top);
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

bool PDF_TEXTPAGE_imp::GetCharOrigin(int index, double* x, double* y)
{
	return FPDFText_GetCharOrigin(m_textPage, index, x, y);
}

int PDF_TEXTPAGE_imp::GetText(int start_index, int count, wchar_t* resultBuff)
{
	return FPDFText_GetText(m_textPage, start_index, count, (unsigned short*)resultBuff);
}

int PDF_TEXTPAGE_imp::CountRects(int start_index, int count)
{
	return FPDFText_CountRects(m_textPage, start_index, count);
}

bool PDF_TEXTPAGE_imp::GetRect(int rect_index, double* left, double* top, double* right, double* bottom)
{
	return FPDFText_GetRect(m_textPage, rect_index, left, top, right, bottom);
}

int PDF_TEXTPAGE_imp::GetTextByBoundary(double left, double top, double right, double bottom, wchar_t* buffer, int buflen)
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
