#include "pch.h"
#include "PDF_PAGEOBJECT_imp.h"
#include <fpdf_edit.h>
#include <font/cpdf_font.h>
#include <cpdfsdk_helpers.h>
#include <page/cpdf_textobject.h>
#include "PDF_PAGEOBJECTMARK_imp.h"
#include "PDF_TEXTPAGE_imp.h"
#include "PDF_FONT_imp.h"
#include "PDF_BITMAP_imp.h"
#include "PDF_DOCUMENT_imp.h"
#include "PDF_PAGE_imp.h"
#include "PDF_PATHSEGMENT_imp.h"
#include "PDF_CLIPPATH_imp.h"
#include <fpdf_editpath.cpp>
#include <fpdf_transformpage.h>


bool PDF_PAGEOBJECT_imp::HasTransparency()
{
	return FPDFPageObj_HasTransparency(m_obj);
}
PDF_PAGE_OBJECT_TYPE PDF_PAGEOBJECT_imp::GetType()
{
	return (PDF_PAGE_OBJECT_TYPE)FPDFPageObj_GetType(m_obj);
}
void PDF_PAGEOBJECT_imp::Transform(float a, float b, float c, float d, float e, float f)
{
	FPDFPageObj_Transform(m_obj, a, b, c, d, e, f);
}

int PDF_PAGEOBJECT_imp::CountMarks()
{
	return FPDFPageObj_CountMarks(m_obj);
}
PDF_PAGEOBJECTMARK* PDF_PAGEOBJECT_imp::OpenMark(int index)
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
	return FPDFPageObj_RemoveMark(m_obj, IMP(PDF_PAGEOBJECTMARK, mark)->m_mark);
}

void PDF_PAGEOBJECT_imp::CloseMark(PDF_PAGEOBJECTMARK** mark)
{
	delete IMP(PDF_PAGEOBJECTMARK, *mark);
	*mark = NULL;
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

bool PDF_PAGEOBJECT_imp::Image_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	double _a, _b, _c, _d, _e, _f;
	if (!FPDFImageObj_GetMatrix(m_obj, &_a, &_b, &_c, &_d, &_e, &_f))
		return false;
	*a = (float)_a;
	*b = (float)_b;
	*c = (float)_c;
	*d = (float)_d;
	*e = (float)_e;
	*f = (float)_f;
	return true;
}
bool PDF_PAGEOBJECT_imp::Image_SetMatrix(float a, float b, float c, float d, float e, float f)
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
bool PDF_PAGEOBJECT_imp::SetStrokeColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
	return FPDFPageObj_SetStrokeColor(m_obj, R, G, B, A);
}
bool PDF_PAGEOBJECT_imp::GetStrokeColor(uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A)
{
	unsigned int _R, _G, _B, _A;
	if (!FPDFPageObj_GetStrokeColor(m_obj, &_R, &_G, &_B, &_A))
		return false;
	*R = _R;
	*G = _G;
	*B = _B;
	*A = _A;
	return true;
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
	if (linejoin == PDF_LINEJOIN::PDF_LINEJOIN_UNKNOW)
		return false;

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
bool PDF_PAGEOBJECT_imp::SetFillColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
	return FPDFPageObj_SetFillColor(m_obj, R, G, B, A);
}
bool PDF_PAGEOBJECT_imp::GetFillColor(uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A)
{
	unsigned int _R, _G, _B, _A;
	if (!FPDFPageObj_GetFillColor(m_obj, &_R, &_G, &_B, &_A))
		return false;
	*R = _R;
	*G = _G;
	*B = _B;
	*A = _A;
	return true;
}
bool PDF_PAGEOBJECT_imp::Image_SetBitmap(PDF_BITMAP* bitmap)
{
	return FPDFImageObj_SetBitmap(NULL, 0, m_obj, IMP(PDF_BITMAP, bitmap)->m_bitmap);
}
PDF_BITMAP* PDF_PAGEOBJECT_imp::Image_OpenBitmap()
{
	auto o = FPDFImageObj_GetBitmap(m_obj);
	if (!o)
		return NULL;
	return new PDF_BITMAP_imp(o);
}
PDF_BITMAP* PDF_PAGEOBJECT_imp::Image_OpenRenderedBitmap(PDF_DOCUMENT* doc, PDF_PAGE* page)
{
	auto o = FPDFImageObj_GetRenderedBitmap(IMP(PDF_DOCUMENT, doc)->m_doc, IMP(PDF_PAGE, page)->m_page, m_obj);
	if (!o)
		return NULL;
	return new PDF_BITMAP_imp(o);
}
void PDF_PAGEOBJECT_imp::Image_CloseBitmap(PDF_BITMAP** pBitmap)
{
	auto o = IMP(PDF_BITMAP, *pBitmap);
	delete o;
	*pBitmap = NULL;
}
unsigned long PDF_PAGEOBJECT_imp::Image_GetDataDecoded(void* buffer, unsigned long buflen)
{
	return FPDFImageObj_GetImageDataDecoded(m_obj, buffer, buflen);
}
unsigned long PDF_PAGEOBJECT_imp::Image_GetDataRaw(void* buffer, unsigned long buflen)
{
	return FPDFImageObj_GetImageDataRaw(m_obj, buffer, buflen);
}
int PDF_PAGEOBJECT_imp::Image_GetFilterCount()
{
	return FPDFImageObj_GetImageFilterCount(m_obj);
}
unsigned long PDF_PAGEOBJECT_imp::Image_GetFilter(int index, void* buffer, unsigned long buflen)
{
	return FPDFImageObj_GetImageFilter(m_obj, index, buffer, buflen);
}
bool PDF_PAGEOBJECT_imp::Image_GetMetadata(PDF_PAGE* page, 
	unsigned int* width, 
	unsigned int* height, 
	float* horizontal_dpi, 
	float* vertical_dpi, 
	unsigned int* bits_per_pixel, 
	PDF_COLORSPACE* colorspace, 
	int* marked_content_id)
{
	FPDF_IMAGEOBJ_METADATA md;
	bool b = FPDFImageObj_GetImageMetadata(m_obj, IMP(PDF_PAGE, page)->m_page, &md);
	if (!b)
		return false;

	*width = md.width;
	*height = md.height;
	*horizontal_dpi = md.horizontal_dpi;
	*vertical_dpi = md.vertical_dpi;
	*bits_per_pixel = md.bits_per_pixel;
	*colorspace = (PDF_COLORSPACE)md.colorspace;
	*marked_content_id = md.marked_content_id;
	return true;
}
int PDF_PAGEOBJECT_imp::Path_CountSegments()
{
	return FPDFPath_CountSegments(m_obj);
}
PDF_PATHSEGMENT* PDF_PAGEOBJECT_imp::Path_OpenSegment(int index)
{
	auto o = FPDFPath_GetPathSegment(m_obj, index);
	if (!o)
		return NULL;
	return new PDF_PATHSEGMENT_imp(o);
}
bool PDF_PAGEOBJECT_imp::Path_MoveTo(float x, float y)
{
	return FPDFPath_MoveTo(m_obj, x, y);
}
bool PDF_PAGEOBJECT_imp::Path_LineTo(float x, float y)
{
	return FPDFPath_LineTo(m_obj, x, y);
}
bool PDF_PAGEOBJECT_imp::Path_BezierTo(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return FPDFPath_BezierTo(m_obj, x1, y1, x2, y2, x3, y3);
}
bool PDF_PAGEOBJECT_imp::Path_SplitBezierTo(float x, float y)
{
	auto* pPathObj = CPDFPathObjectFromFPDFPageObject(m_obj);
	if (!pPathObj)
		return false;

	CPDF_Path& cpath = pPathObj->path();
	cpath.AppendPoint(CFX_PointF(x, y), FXPT_TYPE::BezierTo);
	pPathObj->SetDirty(true);
	return true;
}
bool PDF_PAGEOBJECT_imp::Path_SetClosed()
{
	return FPDFPath_Close(m_obj);
}
bool PDF_PAGEOBJECT_imp::Path_SetDrawMode(PDF_FILLMODE fillmode, bool drawLine)
{
	return FPDFPath_SetDrawMode(m_obj, fillmode, drawLine);
}
bool PDF_PAGEOBJECT_imp::Path_GetDrawMode(PDF_FILLMODE* fillmode, bool* drawLine)
{
	FPDF_BOOL _drawLine;
	bool b = FPDFPath_GetDrawMode(m_obj, (int*)fillmode, &_drawLine);
	*drawLine = _drawLine;
	return b;
}
bool PDF_PAGEOBJECT_imp::Path_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	FS_MATRIX mat;
	bool res = FPDFPath_GetMatrix(m_obj, &mat);
	if (!res)
		return false;
	*a = mat.a;
	*b = mat.b;
	*c = mat.c;
	*d = mat.d;
	*e = mat.e;
	*f = mat.f;
	return true;
}
bool PDF_PAGEOBJECT_imp::Path_SetMatrix(float a, float b, float c, float d, float e, float f)
{
	FS_MATRIX mat;
	mat.a = a;
	mat.b = b;
	mat.c = c;
	mat.d = d;
	mat.e = e;
	mat.f = f;
	return FPDFPath_SetMatrix(m_obj, &mat);
}
bool PDF_PAGEOBJECT_imp::Text_SetText(const wchar_t* text)
{
	if (!text || text[0] == '\0')
		return FPDFText_SetText(m_obj, (FPDF_WIDESTRING)L" ");//·ÀÖ¹±ÀÀ£
	
	return FPDFText_SetText(m_obj, (FPDF_WIDESTRING)text);
}
bool PDF_PAGEOBJECT_imp::Text_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	FS_MATRIX mat;
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
	return FPDFTextObj_GetFontSize(m_obj);
}
PDF_TEXT_RENDERMODE PDF_PAGEOBJECT_imp::Text_GetRenderMode()
{
	return (PDF_TEXT_RENDERMODE)FPDFTextObj_GetTextRenderMode(m_obj);
}
bool PDF_PAGEOBJECT_imp::Text_SetRenderMode(PDF_TEXT_RENDERMODE render_mode)
{
	if (render_mode == PDF_TEXT_RENDERMODE::PDF_TEXTRENDERMODE_UNKNOWN)
		return false;
	return FPDFTextObj_SetTextRenderMode(m_obj, (FPDF_TEXT_RENDERMODE)render_mode);
}
unsigned long PDF_PAGEOBJECT_imp::Text_GetFontName(char* bufferUtf8, unsigned long length)
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
	return FPDFFormObj_CountObjects(m_obj);
}
PDF_PAGEOBJECT* PDF_PAGEOBJECT_imp::Form_OpenObject(unsigned long index)
{
	auto o = FPDFFormObj_GetObject(m_obj, index);
	if (!o)
		return NULL;
	return new PDF_PAGEOBJECT_imp(o);
}
bool PDF_PAGEOBJECT_imp::FormObject_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f)
{
	FS_MATRIX mat;
	if (!FPDFFormObj_GetMatrix(m_obj, &mat))
		return false;
	*a = mat.a;
	*b = mat.b;
	*c = mat.c;
	*d = mat.d;
	*e = mat.e;
	*f = mat.f;
	return true;
}

void PDF_PAGEOBJECT_imp::CloseFormObject(PDF_PAGEOBJECT** formObj)
{
	PDF_PAGEOBJECT_imp* o = IMP(PDF_PAGEOBJECT, *formObj);
	delete o;
	*formObj = NULL;
}

void PDF_PAGEOBJECT_imp::Path_CloseSegment(PDF_PATHSEGMENT** segment)
{
	delete IMP(PDF_PATHSEGMENT, *segment);
	*segment = NULL;
}

bool PDF_PAGEOBJECT_imp::Path_IsClosed()
{
	auto* pPathObj = CPDFPathObjectFromFPDFPageObject(m_obj);
	if (!pPathObj)
		return false;

	CPDF_Path& cpath = pPathObj->path();
	if (cpath.GetPoints().empty())
		return false;

	return cpath.GetPoints().back().m_CloseFigure;
}

PDF_PAGEOBJECT* PDF_PAGEOBJECT_imp::Clone(PDF_DOCUMENT* doc, PDF_PAGE* page)
{
	PDF_PAGEOBJECT* newPageObj = NULL;

	auto funcFillCommon = [&]() 
	{
		uint8_t R, G, B, A;
		if (this->GetFillColor(&R, &G, &B, &A))
			newPageObj->SetFillColor(R, G, B, A);

		newPageObj->SetLineCap(this->GetLineCap());

		newPageObj->SetLineJoin(this->GetLineJoin());

		if (this->GetStrokeColor(&R, &G, &B, &A))
			newPageObj->SetStrokeColor(R, G, B, A);

		float strokeWidth = 1;
		if (this->GetStrokeWidth(&strokeWidth))
			newPageObj->SetStrokeWidth(strokeWidth);
	};

	auto t = GetType();
	switch (t)
	{
	case PDF_PAGEOBJ_TEXT:
	{
		float fontSize = this->Text_GetFontSize();
		char fontName[255] = { 0 };
		this->Text_GetFontName(fontName, 254);
		newPageObj = doc->NewTextPageObject(fontSize, fontName);		
		if (newPageObj)
		{
			funcFillCommon();

			newPageObj->Text_SetRenderMode(this->Text_GetRenderMode());

			PDF_TEXTPAGE* textPage = page->OpenTextPage();
			if (textPage)
			{
				wchar_t text[512] = { 0 };
				auto len = this->Text_GetText(textPage, text, 512);
				newPageObj->Text_SetText(text);
				page->CloseTextPage(&textPage);
			}
		}		
	}
		break;
	case PDF_PAGEOBJ_PATH:
	{
		newPageObj = doc->NewPathPageObject();
		if (newPageObj)
		{
			funcFillCommon();

			int countSegs = this->Path_CountSegments();
			for (int i = 0; i < countSegs; i++)
			{
				auto seg = this->Path_OpenSegment(i);
				auto segType = seg->GetType();
				float x, y;
				if (seg->GetPoint(&x, &y))
				{
					switch (segType)
					{
					case PDF_PATHSEGMENT::PDF_SEGMENT_LINETO:
						newPageObj->Path_LineTo(x, y);
						break;
					case PDF_PATHSEGMENT::PDF_SEGMENT_BEZIERTO:
						newPageObj->Path_SplitBezierTo(x, y);
						break;
					case PDF_PATHSEGMENT::PDF_SEGMENT_MOVETO:
						newPageObj->Path_MoveTo(x, y);
						break;
					default:
						break;
					}
				}
				this->Path_CloseSegment(&seg);
			}

			PDF_FILLMODE fillmode;
			bool drawLine;
			if (this->Path_GetDrawMode(&fillmode, &drawLine))
				newPageObj->Path_SetDrawMode(fillmode, drawLine);

			if (this->Path_IsClosed())
				newPageObj->Path_SetClosed();
		}
	}
		break;
	case PDF_PAGEOBJ_IMAGE:
	{
		newPageObj = doc->NewImagePageObject();
		if (newPageObj)
		{
			funcFillCommon();

			auto bitmap = this->Image_OpenBitmap();
			if (bitmap)
				newPageObj->Image_SetBitmap(bitmap);
		}
	}
		break;
	case PDF_PAGEOBJ_SHADING:
	{

	}
		break;
	case PDF_PAGEOBJ_FORM:
	{
		
	}
		break;
	default:
		return NULL;
		break;
	}
	
	return newPageObj;
}

void PDF_PAGEOBJECT_imp::TransformClipPath(
	float a, float b,
	float c, float d,
	float e, float f)
{
	FPDFPageObj_TransformClipPath(m_obj, a, b, c, d, e, f);
}

PDF_CLIPPATH* PDF_PAGEOBJECT_imp::OpenClipPath()
{
	auto o = FPDFPageObj_GetClipPath(m_obj);
	if (!o)
		return NULL;
	return new PDF_CLIPPATH_imp(o);
}

void PDF_PAGEOBJECT_imp::CloseClipPath(PDF_CLIPPATH** clipPath)
{
	delete IMP(PDF_CLIPPATH, *clipPath);
	*clipPath = NULL;
}

