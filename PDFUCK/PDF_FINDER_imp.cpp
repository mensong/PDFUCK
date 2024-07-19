#include "pch.h"
#include "PDF_FINDER_imp.h"

int PDF_FINDER_imp::GetCurrentResultCharIndex()
{
    return FPDFText_GetSchResultIndex(m_finder);
}

int PDF_FINDER_imp::GetCurrentResultCharCount()
{
    return FPDFText_GetSchCount(m_finder);
}

bool PDF_FINDER_imp::FindNext()
{
    return FPDFText_FindNext(m_finder);
}

bool PDF_FINDER_imp::FindPrev()
{
    return FPDFText_FindPrev(m_finder);
}
