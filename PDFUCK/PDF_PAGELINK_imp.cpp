#include "pch.h"
#include "PDF_PAGELINK_imp.h"

int PDF_PAGELINK_imp::CountWebLinks()
{
    return 0;
}

int PDF_PAGELINK_imp::GetURL(int link_index, unsigned short* buffer, int buflen)
{
    return 0;
}

int PDF_PAGELINK_imp::CountRects(int link_index)
{
    return 0;
}

bool PDF_PAGELINK_imp::GetRect(int link_index, int rect_index, double* left, double* top, double* right, double* bottom)
{
    return false;
}

bool PDF_PAGELINK_imp::GetTextRange(int link_index, int* start_char_index, int* char_count)
{
    return false;
}
