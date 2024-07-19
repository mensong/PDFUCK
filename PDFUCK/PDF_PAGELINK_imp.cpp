#include "pch.h"
#include "PDF_PAGELINK_imp.h"

int PDF_PAGELINK_imp::CountWebLinks()
{
    return FPDFLink_CountWebLinks(m_pageLink);
}

int PDF_PAGELINK_imp::GetURL(int link_index, wchar_t* buffer, int buflen)
{
    return FPDFLink_GetURL(m_pageLink, link_index, (unsigned short*)buffer, buflen);
}

int PDF_PAGELINK_imp::CountRects(int link_index)
{
    return FPDFLink_CountRects(m_pageLink, link_index);
}

bool PDF_PAGELINK_imp::GetRect(int link_index, int rect_index, double* left, double* top, double* right, double* bottom)
{
    return FPDFLink_GetRect(m_pageLink, link_index, rect_index, left, top, right, bottom);
}

bool PDF_PAGELINK_imp::GetTextRange(int link_index, int* start_char_index, int* char_count)
{
    return FPDFLink_GetTextRange(m_pageLink, link_index, start_char_index, char_count);
}
