#include "pch.h"
#include "PDF_FONT_imp.h"
#include <fpdf_edit.h>
#include <cpdfsdk_helpers.h>
#include <font/cpdf_font.h>
#include <font/cpdf_cidfont.h>

PDF_FONT_imp::PDF_FONT_imp()
	: m_font(NULL)
{

}

PDF_FONT_imp::PDF_FONT_imp(FPDF_FONT font)
	: m_font(font)
{

}

PDF_FONT_imp::~PDF_FONT_imp()
{
	if (m_font)
	{
		FPDFFont_Close(m_font);
		m_font = NULL;
	}
}

float PDF_FONT_imp::CalcCharWidth(wchar_t c, float fontSize)
{
	CPDF_Font* pFont = CPDFFontFromFPDFFont(m_font);
	uint32_t charCode = pFont->CharCodeFromUnicode(c);
	if (!charCode)
		return -1;

	float _fontsize = fontSize / 1000;
	bool bVertWriting = false;
	CPDF_CIDFont* pCIDFont = pFont->AsCIDFont();
	if (pCIDFont)
		bVertWriting = pCIDFont->IsVertWriting();
	if (!bVertWriting)
		return pFont->GetCharWidthF(charCode) * _fontsize;

	uint16_t cid = pCIDFont->CIDFromCharCode(charCode);
	return pCIDFont->GetVertWidth(cid) * _fontsize;
}

