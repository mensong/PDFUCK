#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdfview.h>

class PDF_PAGEOBJECT_imp;

class PDF_PATHSEGMENT_imp
	: public PDF_PATHSEGMENT
{
public:
	FPDF_PATHSEGMENT m_segment;

	PDF_PATHSEGMENT_imp();
	PDF_PATHSEGMENT_imp(FPDF_PATHSEGMENT segment);
	~PDF_PATHSEGMENT_imp();

	// Í¨¹ý PDF_PATHSEGMENT ¼Ì³Ð
	virtual bool GetPoint(float* x, float* y) override;

	virtual PDF_SEGMENT_TYPE GetType() override;

	virtual bool GetIsClose() override;

};

