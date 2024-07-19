#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdf_text.h>

class PDF_DOCUMENT_imp;

class PDF_FONT_imp
	: public PDF_FONT
{
public:
	FPDF_FONT m_font;

	PDF_FONT_imp();
	PDF_FONT_imp(FPDF_FONT font);
	~PDF_FONT_imp();


	// ͨ�� PDF_FONT �̳�
	virtual float CalcCharWidth(wchar_t c, float fontSize) override;


	// ͨ�� PDF_FONT �̳�
	virtual float CalcStringWidth(const wchar_t* str, float fontSize) override;

};
