#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdf_text.h>

class PDF_TEXTPAGE_imp;

class PDF_FINDER_imp
	: public PDF_FINDER
{
public:
	FPDF_SCHHANDLE m_finder;

	PDF_FINDER_imp()
		: m_finder(NULL)
	{

	}
	PDF_FINDER_imp(FPDF_SCHHANDLE finder)
		: m_finder(finder)
	{

	}

	~PDF_FINDER_imp()
	{
		if (m_finder)
		{
			FPDFText_FindClose(m_finder);
			m_finder = NULL;
		}
	}


	// Í¨¹ý PDF_FINDER ¼Ì³Ð
	virtual int GetCurrentResultCharIndex() override;

	virtual int GetCurrentResultCharCount() override;

	virtual bool FindNext() override;

	virtual bool FindPrev() override;

};
