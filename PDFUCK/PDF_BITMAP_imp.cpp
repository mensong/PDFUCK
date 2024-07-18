#include "pch.h"
#include "PDF_BITMAP_imp.h"

PDF_BITMAP_imp::PDF_BITMAP_imp()
	: m_bitmap(NULL)
{

}

PDF_BITMAP_imp::PDF_BITMAP_imp(FPDF_BITMAP mp)
	: m_bitmap(mp)
{

}

PDF_BITMAP_imp::PDF_BITMAP_imp(int width, int height, int alpha)
{
	m_bitmap = FPDFBitmap_Create(width, height, alpha);
}

PDF_BITMAP_imp::~PDF_BITMAP_imp()
{
	if (m_bitmap)
	{
		FPDFBitmap_Destroy(m_bitmap);
		m_bitmap = NULL;
	}
}

int PDF_BITMAP_imp::GetFormat()
{
	return FPDFBitmap_GetFormat(m_bitmap);
}
void PDF_BITMAP_imp::FillRect(int left, int top, int width, int height, unsigned long color)
{
	FPDFBitmap_FillRect(m_bitmap, left, top, width, height, color);
}
void* PDF_BITMAP_imp::GetBuffer()
{
	return FPDFBitmap_GetBuffer(m_bitmap);
}
int PDF_BITMAP_imp::GetWidth()
{
	return FPDFBitmap_GetWidth(m_bitmap);
}
int PDF_BITMAP_imp::GetHeight()
{
	return FPDFBitmap_GetHeight(m_bitmap);
}
int PDF_BITMAP_imp::GetStride()
{
	return FPDFBitmap_GetStride(m_bitmap);
}
