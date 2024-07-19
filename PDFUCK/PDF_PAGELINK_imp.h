#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdf_text.h>

class PDF_TEXTPAGE_imp;

class PDF_PAGELINK_imp
	: public PDF_PAGELINK
{
public:
	FPDF_PAGELINK m_pageLink;

	PDF_PAGELINK_imp()
		: m_pageLink(NULL)
	{

	}
	PDF_PAGELINK_imp(FPDF_PAGELINK pageLink)
		: m_pageLink(pageLink)
	{

	}

	~PDF_PAGELINK_imp()
	{
		if (m_pageLink)
		{
			FPDFLink_CloseWebLinks(m_pageLink);
			m_pageLink = NULL;
		}
	}


	// Í¨¹ý PDF_PAGELINK ¼Ì³Ð
	virtual int CountWebLinks() override;

	virtual int GetURL(int link_index, wchar_t* buffer, int buflen) override;

	virtual int CountRects(int link_index) override;

	virtual bool GetRect(int link_index, int rect_index, double* left, double* top, double* right, double* bottom) override;

	virtual bool GetTextRange(int link_index, int* start_char_index, int* char_count) override;

};

