#include "pch.h"
#include "PDF_PAGEOBJECT_imp.h"
#include "PDF_PAGEOBJECTMARK_imp.h"
#include "PDF_TEXTPAGE_imp.h"
#include "PDF_FONT_imp.h"
#include <fpdf_edit.h>
#include <font/cpdf_font.h>
#include <cpdfsdk_helpers.h>
#include <page/cpdf_textobject.h>

// Í¨¹ý PDF_PAGEOBJECT ¼Ì³Ð
bool PDF_PAGEOBJECT_imp::HasTransparency()
{
	return FPDFPageObj_HasTransparency(m_obj);
}
PDF_PAGE_OBJECT_TYPE PDF_PAGEOBJECT_imp::GetType()
{
	return (PDF_PAGE_OBJECT_TYPE)FPDFPageObj_GetType(m_obj);
}
void PDF_PAGEOBJECT_imp::Transform(double a, double b, double c, double d, double e, double f)
{
	FPDFPageObj_Transform(m_obj, a, b, c, d, e, f);
}

int PDF_PAGEOBJECT_imp::CountMarks()
{
	return FPDFPageObj_CountMarks(m_obj);
}
PDF_PAGEOBJECTMARK* PDF_PAGEOBJECT_imp::GetMark(int index)
{
	auto o = FPDFPageObj_GetMark(m_obj, index);
	if (!o)
		return NULL;
	return new PDF_PAGEOBJECTMARK_imp(o);
}
PDF_PAGEOBJECTMARK* PDF_PAGEOBJECT_imp::AddMark(const char* tag)
{
	auto o = FPDFPageObj_AddMark(m_obj, tag);
	if (!o)
		return NULL;
	return new PDF_PAGEOBJECTMARK_imp(o);
}
bool PDF_PAGEOBJECT_imp::RemoveMark(PDF_PAGEOBJECTMARK* mark)
{
	return false;
}
float PDF_PAGEOBJECT_imp::Text_CalcCharWidth(PDF_FONT* font, wchar_t c)
{
	CPDF_Font* cfont = CPDFFontFromFPDFFont(IMP(PDF_FONT, font)->m_font);
	uint32_t charCode = cfont->CharCodeFromUnicode(c);
	if (!charCode)
		return 0;
	CPDF_TextObject* ctextObj = reinterpret_cast<CPDF_TextObject*>(m_obj);
	return (ctextObj->GetCharWidth)(charCode);
}

bool PDF_PAGEOBJECT_imp::Image_GetMatrix(double* a, double* b, double* c, double* d, double* e, double* f)
{
	return FPDFImageObj_GetMatrix(m_obj, a, b, c, d, e, f);
}
bool PDF_PAGEOBJECT_imp::Image_SetMatrix(double a, double b, double c, double d, double e, double f)
{
	return FPDFImageObj_SetMatrix(m_obj, a, b, c, d, e, f);
}
bool PDF_PAGEOBJECT_imp::GetBounds(float* left, float* bottom, float* right, float* top)
{
	return FPDFPageObj_GetBounds(m_obj, left, bottom, right, top);
}
void PDF_PAGEOBJECT_imp::SetBlendMode(BLEND_MODE bm)
{
#define CASE_TO_STR(outStr, caseItem) case caseItem: outStr = #caseItem; break;

	std::string sbm;
	switch (bm)
	{
		CASE_TO_STR(sbm, Color);
		CASE_TO_STR(sbm, ColorBurn);
		CASE_TO_STR(sbm, ColorDodge);
		CASE_TO_STR(sbm, Darken);
		CASE_TO_STR(sbm, Difference);
		CASE_TO_STR(sbm, Exclusion);
		CASE_TO_STR(sbm, HardLight);
		CASE_TO_STR(sbm, Hue);
		CASE_TO_STR(sbm, Lighten);
		CASE_TO_STR(sbm, Luminosity);
		CASE_TO_STR(sbm, Multiply);
		CASE_TO_STR(sbm, Normal);
		CASE_TO_STR(sbm, Overlay);
		CASE_TO_STR(sbm, Saturation);
		CASE_TO_STR(sbm, Screen);
		CASE_TO_STR(sbm, SoftLight);
	}
	if (sbm.empty())
		return;

	FPDFPageObj_SetBlendMode(m_obj, sbm.c_str());

#undef CASE_TO_STR
}
bool PDF_PAGEOBJECT_imp::SetStrokeColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{
	return FPDFPageObj_SetStrokeColor(m_obj, R, G, B, A);
}
bool PDF_PAGEOBJECT_imp::GetStrokeColor(unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A)
{
	return FPDFPageObj_GetStrokeColor(m_obj, R, G, B, A);
}
bool PDF_PAGEOBJECT_imp::SetStrokeWidth(float width)
{
	return FPDFPageObj_SetStrokeWidth(m_obj, width);
}
bool PDF_PAGEOBJECT_imp::GetStrokeWidth(float* width)
{
	return FPDFPageObj_GetStrokeWidth(m_obj, width);
}
PDF_PAGEOBJECT::PDF_LINEJOIN PDF_PAGEOBJECT_imp::GetLineJoin()
{
	return (PDF_LINEJOIN)FPDFPageObj_GetLineJoin(m_obj);
}
bool PDF_PAGEOBJECT_imp::SetLineJoin(PDF_LINEJOIN linejoin)
{
	return FPDFPageObj_SetLineJoin(m_obj, linejoin);
}
PDF_PAGEOBJECT::PDF_LINECAP PDF_PAGEOBJECT_imp::GetLineCap()
{
	return (PDF_LINECAP)FPDFPageObj_GetLineCap(m_obj);
}
bool PDF_PAGEOBJECT_imp::SetLineCap(PDF_LINECAP line_cap)
{
	return FPDFPageObj_SetLineCap(m_obj, line_cap);
}
bool PDF_PAGEOBJECT_imp::SetFillColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
{
	return FPDFPageObj_SetFillColor(m_obj, R, G, B, A);
}
bool PDF_PAGEOBJECT_imp::GetFillColor(unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A)
{
	return FPDFPageObj_GetFillColor(m_obj, R, G, B, A);
}
bool PDF_PAGEOBJECT_imp::Image_SetBitmap(PDF_BITMAP* bitmap)
{
	return false;
}
PDF_BITMAP* PDF_PAGEOBJECT_imp::Image_GetBitmap()
{
	return nullptr;
}
PDF_BITMAP* PDF_PAGEOBJECT_imp::Image_GetRenderedBitmap()
{
	return nullptr;
}
unsigned long PDF_PAGEOBJECT_imp::Image_GetImageDataDecoded(void* buffer, unsigned long buflen)
{
	return 0;
}
unsigned long PDF_PAGEOBJECT_imp::Image_GetImageDataRaw(void* buffer, unsigned long buflen)
{
	return 0;
}
int PDF_PAGEOBJECT_imp::Image_GetImageFilterCount()
{
	return 0;
}
unsigned long PDF_PAGEOBJECT_imp::Image_GetImageFilter(int index, void* buffer, unsigned long buflen)
{
	return 0;
}
bool PDF_PAGEOBJECT_imp::Image_GetImageMetadata(PDF_PAGE* page, unsigned int* width, unsigned int* height, float* horizontal_dpi, float* vertical_dpi, unsigned int* bits_per_pixel, PDF_COLORSPACE* colorspace, int* marked_content_id)
{
	return false;
}
int PDF_PAGEOBJECT_imp::Path_CountSegments()
{
	return 0;
}
PDF_PATHSEGMENT* PDF_PAGEOBJECT_imp::Path_GetPathSegment(int index)
{
	return nullptr;
}
bool PDF_PAGEOBJECT_imp::Path_MoveTo(float x, float y)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Path_LineTo(float x, float y)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Path_BezierTo(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Path_SetClose()
{
	return FPDFPath_Close(m_obj);
}
bool PDF_PAGEOBJECT_imp::Path_SetDrawMode(PDF_FILLMODE fillmode, bool stroke)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Path_GetDrawMode(PDF_FILLMODE* fillmode, bool* stroke)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Path_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Path_SetMatrix(float a, float b, float c, float d, float e, float f)
{
	return false;
}
bool PDF_PAGEOBJECT_imp::Text_SetText(const wchar_t* text)
{
	return FPDFText_SetText(m_obj, (FPDF_WIDESTRING)text);
}
bool PDF_PAGEOBJECT_imp::Text_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	FS_MATRIX mat;
	memset(&mat, 0, sizeof(mat));
	if (!FPDFTextObj_GetMatrix(m_obj, &mat))
		return false;

	*a = mat.a;
	*b = mat.b;
	*c = mat.c;
	*d = mat.d;
	*e = mat.e;
	*f = mat.f;
	return true;
}
float PDF_PAGEOBJECT_imp::Text_GetFontSize()
{
	return 0.0f;
}
PDF_TEXT_RENDERMODE PDF_PAGEOBJECT_imp::Text_GetTextRenderMode()
{
	return PDF_TEXT_RENDERMODE();
}
bool PDF_PAGEOBJECT_imp::Text_SetTextRenderMode(PDF_TEXT_RENDERMODE render_mode)
{
	return false;
}
unsigned long PDF_PAGEOBJECT_imp::Text_GetFontName(void* bufferUtf8, unsigned long length)
{
	return FPDFTextObj_GetFontName(m_obj, bufferUtf8, length);
}
unsigned long PDF_PAGEOBJECT_imp::Text_GetText(PDF_TEXTPAGE* text_page, wchar_t* buffer, unsigned long length)
{
	unsigned long readed = FPDFTextObj_GetText(m_obj, IMP(PDF_TEXTPAGE, text_page)->m_textPage,
		(void*)buffer, length * sizeof(wchar_t));
	readed /= sizeof(wchar_t);
	return readed;
}
int PDF_PAGEOBJECT_imp::Form_CountObjects()
{
	return 0;
}
PDF_PAGEOBJECT* PDF_PAGEOBJECT_imp::Form_GetObject(unsigned long index)
{
	return nullptr;
}
bool PDF_PAGEOBJECT_imp::Form_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	return false;
}