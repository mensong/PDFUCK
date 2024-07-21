#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdf_text.h>

class PDF_DOCUMENT_imp;

class PDF_PAGEOBJECT_imp
	: public PDF_PAGEOBJECT
{
public:
	FPDF_PAGEOBJECT m_obj;

public:
	PDF_PAGEOBJECT_imp()
		: m_obj(NULL)
	{

	}
	PDF_PAGEOBJECT_imp(FPDF_PAGEOBJECT obj)
		: m_obj(obj)
	{

	}
	~PDF_PAGEOBJECT_imp()
	{

	}
		
	// 通过 PDF_PAGEOBJECT 继承
	virtual PDF_PAGE_OBJECT_TYPE GetType() override;

	virtual bool HasTransparency() override;

	virtual void Transform(float a, float b, float c, float d, float e, float f) override;

	virtual bool GetBounds(float* left, float* bottom, float* right, float* top) override;

	virtual void SetBlendMode(BLEND_MODE bm) override;

	virtual bool SetStrokeColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A) override;

	virtual bool GetStrokeColor(unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A) override;

	virtual bool SetStrokeWidth(float width) override;

	virtual bool GetStrokeWidth(float* width) override;

	virtual bool SetFillColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A) override;

	virtual bool GetFillColor(unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A) override;

	virtual PDF_LINEJOIN GetLineJoin() override;

	virtual bool SetLineJoin(PDF_LINEJOIN linejoin) override;

	virtual PDF_LINECAP GetLineCap() override;

	virtual bool SetLineCap(PDF_LINECAP line_cap) override;

	virtual bool Image_SetBitmap(PDF_BITMAP* bitmap) override;

	virtual PDF_BITMAP* Image_GetBitmap() override;

	virtual PDF_BITMAP* Image_GetRenderedBitmap(PDF_DOCUMENT* doc, PDF_PAGE* page) override;

	virtual unsigned long Image_GetDataDecoded(void* buffer, unsigned long buflen) override;

	virtual unsigned long Image_GetDataRaw(void* buffer, unsigned long buflen) override;

	virtual int Image_GetFilterCount() override;

	virtual unsigned long Image_GetFilter(int index, void* buffer, unsigned long buflen) override;

	virtual bool Image_GetMetadata(PDF_PAGE* page, unsigned int* width, unsigned int* height, float* horizontal_dpi, float* vertical_dpi, unsigned int* bits_per_pixel, PDF_COLORSPACE* colorspace, int* marked_content_id) override;

	virtual bool Image_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f) override;

	virtual bool Image_SetMatrix(float a, float b, float c, float d, float e, float f) override;

	virtual int Path_CountSegments() override;

	virtual PDF_PATHSEGMENT* Path_OpenSegment(int index) override;

	virtual bool Path_MoveTo(float x, float y) override;

	virtual bool Path_LineTo(float x, float y) override;

	virtual bool Path_BezierTo(float x1, float y1, float x2, float y2, float x3, float y3) override;

	virtual bool Path_SplitBezierTo(float x, float y) override;

	virtual bool Path_SetClosed() override;

	virtual bool Path_SetDrawMode(PDF_FILLMODE fillmode, bool drawLine) override;

	virtual bool Path_GetDrawMode(PDF_FILLMODE* fillmode, bool* drawLine) override;

	virtual bool Path_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f) override;

	virtual bool Path_SetMatrix(float a, float b, float c, float d, float e, float f) override;

	virtual bool Text_SetText(const wchar_t* text) override;

	virtual bool Text_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f) override;

	virtual float Text_GetFontSize() override;

	virtual PDF_TEXT_RENDERMODE Text_GetRenderMode() override;

	virtual bool Text_SetRenderMode(PDF_TEXT_RENDERMODE render_mode) override;

	virtual unsigned long Text_GetFontName(char* bufferUtf8, unsigned long length) override;

	virtual unsigned long Text_GetText(PDF_TEXTPAGE* text_page, wchar_t* buffer, unsigned long length) override;

	virtual int Form_CountObjects() override;

	virtual PDF_PAGEOBJECT* Form_OpenObject(unsigned long index) override;

	virtual bool FormObject_GetMatrix(float* a, float* b, float* c, float* d, float* e, float* f) override;

	virtual int CountMarks() override;

	virtual PDF_PAGEOBJECTMARK* OpenMark(int index) override;

	virtual PDF_PAGEOBJECTMARK* AddMark(const char* tag) override;

	virtual bool RemoveMark(PDF_PAGEOBJECTMARK* mark) override;

	virtual void CloseMark(PDF_PAGEOBJECTMARK** mark) override;

	virtual float Text_CalcCharWidth(PDF_FONT* font, wchar_t c) override;


	// 通过 PDF_PAGEOBJECT 继承
	virtual void CloseFormObject(PDF_PAGEOBJECT** formObj) override;


	// 通过 PDF_PAGEOBJECT 继承
	virtual void Path_CloseSegment(PDF_PATHSEGMENT** segment) override;


	// 通过 PDF_PAGEOBJECT 继承
	virtual bool Path_IsClosed() override;


	// 通过 PDF_PAGEOBJECT 继承
	virtual PDF_PAGEOBJECT* Clone(PDF_DOCUMENT* doc, PDF_PAGE* page) override;

};
