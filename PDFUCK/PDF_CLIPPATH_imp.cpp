#include "pch.h"
#include "PDF_CLIPPATH_imp.h"
#include "PDF_PATHSEGMENT_imp.h"
#include <fpdf_transformpage.h>

int PDF_CLIPPATH_imp::CountPaths()
{
	return FPDFClipPath_CountPaths(m_clipPath);
}

int PDF_CLIPPATH_imp::CountPathSegments(int path_index)
{
	return FPDFClipPath_CountPathSegments(m_clipPath, path_index);
}

PDF_PATHSEGMENT* PDF_CLIPPATH_imp::OpenPathSegment(int path_index, int segment_index)
{
	auto o = FPDFClipPath_GetPathSegment(m_clipPath, path_index, segment_index);
	if (!o)
		return NULL;
	return new PDF_PATHSEGMENT_imp(o);
}

void PDF_CLIPPATH_imp::ClosePathSegment(PDF_PATHSEGMENT** pathSegment)
{
	delete IMP(PDF_PATHSEGMENT, *pathSegment);
	*pathSegment = NULL;
}
