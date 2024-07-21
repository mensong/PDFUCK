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

float PDF_FONT_imp::CalcStringWidth(const wchar_t* str, float fontSize)
{
	float strWidth = 0;
	size_t strLen = wcslen(str);
	for (size_t i = 0; i < strLen; i++)
	{
		float lc = CalcCharWidth(str[i], fontSize);
		if (lc <= 0)
			continue;
		strWidth += lc;
	}
	return strWidth;
}

int PDF_FONT_imp::GetFontName(char* buffer, int buffLen)
{
	CPDF_Font* pFont = CPDFFontFromFPDFFont(m_font);
	ByteString sname = pFont->GetBaseFontName();
	strcpy_s(buffer, buffLen, sname.c_str());
	return sname.GetStringLength();
}

