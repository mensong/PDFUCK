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


	// Í¨¹ý PDF_FONT ¼Ì³Ð
	virtual float CalcCharWidth(wchar_t c, float fontSize) override;

};
