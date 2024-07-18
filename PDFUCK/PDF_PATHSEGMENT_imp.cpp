#include "pch.h"
#include "PDF_PATHSEGMENT_imp.h"
#include <fpdf_edit.h>

PDF_PATHSEGMENT_imp::PDF_PATHSEGMENT_imp()
	: m_segment(NULL)
{

}

PDF_PATHSEGMENT_imp::PDF_PATHSEGMENT_imp(FPDF_PATHSEGMENT segment)
	: m_segment(segment)
{

}

PDF_PATHSEGMENT_imp::~PDF_PATHSEGMENT_imp()
{

}

// Í¨¹ý PDF_PATHSEGMENT ¼Ì³Ð
bool PDF_PATHSEGMENT_imp::GetPoint(float* x, float* y)
{
	return false;
}

PDF_PATHSEGMENT_imp::PDF_SEGMENT_TYPE PDF_PATHSEGMENT_imp::GetType()
{
	return PDF_SEGMENT_TYPE();
}

bool PDF_PATHSEGMENT_imp::GetIsClose()
{
	return FPDFPathSegment_GetClose(m_segment);
}