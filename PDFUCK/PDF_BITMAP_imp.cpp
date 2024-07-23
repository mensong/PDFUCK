#include "pch.h"
#include "PDF_BITMAP_imp.h"
#include <cpdfsdk_helpers.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

PDF_BITMAP_imp::PDF_BITMAP_imp()
	: m_bitmap(NULL)
{

}

PDF_BITMAP_imp::PDF_BITMAP_imp(FPDF_BITMAP mp)
	: m_bitmap(mp)
{

}

PDF_BITMAP_imp::~PDF_BITMAP_imp()
{
	if (m_bitmap)
	{
		FPDFBitmap_Destroy(m_bitmap);
		m_bitmap = NULL;
	}
}

PDF_BITMAP::FORMAT PDF_BITMAP_imp::GetFormat()
{
	//return FPDFBitmap_GetFormat(m_bitmap);
	FXDIB_Format format = CFXDIBitmapFromFPDFBitmap(m_bitmap)->GetFormat();
	return (FORMAT)format;
	//switch (format) {
	//case FXDIB_Format::k8bppRgb:
	//case FXDIB_Format::k8bppMask:
	//	return FPDFBitmap_Gray;
	//case FXDIB_Format::kRgb:
	//	return FPDFBitmap_BGR;
	//case FXDIB_Format::kRgb32:
	//	return FPDFBitmap_BGRx;
	//case FXDIB_Format::kArgb:
	//	return FPDFBitmap_BGRA;
	//default:
	//	return FPDFBitmap_Unknown;
	//}
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

bool PDF_BITMAP_imp::WriteToFile(const char* filePath)
{
	int num_channels = GetNumChannels();
	if (num_channels < 0)
		return false;

	int result = 0;
	int wd = GetWidth();
	int hgt = GetHeight();	
	int stride = GetStride();
	auto data = GetBuffer();

	int format = 0;
	std::string sFilePath = filePath;
	size_t idx = sFilePath.find_last_of('.');
	if (idx != std::string::npos)
	{
		std::string ext = sFilePath.substr(idx + 1);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext == "png")
			format = 1;
		else if (ext == "jpg" || ext == "jpeg")
			format = 2;
		else if (ext == "bmp")
			format = 0;
	}

	switch (format) 
	{
	case 0:
		result = stbi_write_bmp(filePath, wd, hgt, num_channels, data);
		break;
	case 1:
		result = stbi_write_png(filePath, wd, hgt, num_channels, data, stride);
		break;
	case 2:
		result = stbi_write_jpg(filePath, wd, hgt, num_channels, data, 85);
		break;
	}
	
	return result;
}

int PDF_BITMAP_imp::GetNumChannels()
{
	//switch (format) {
	//case FXDIB_Format::k8bppRgb:
	//case FXDIB_Format::k8bppMask:
	//	return FPDFBitmap_Gray;
	//case FXDIB_Format::kRgb:
	//	return FPDFBitmap_BGR;
	//case FXDIB_Format::kRgb32:
	//	return FPDFBitmap_BGRx;
	//case FXDIB_Format::kArgb:
	//	return FPDFBitmap_BGRA;
	//default:
	//	return FPDFBitmap_Unknown;
	//}

	//switch (GetFormat()) 
	//{
	//case FPDFBitmap_Gray:
	//	return 1;
	//case FPDFBitmap_BGR:
	//	return 3;
	//case FPDFBitmap_BGRx:
	//case FPDFBitmap_BGRA:
	//	return 4;
	//case FPDFBitmap_Unknown:
	//	return -1;
	//}
	//return -1;

	auto f = GetFormat();
	switch (f)
	{
	case PDF_BITMAP::k8bppRgb:
	case PDF_BITMAP::k8bppMask:
		return 1;
	case PDF_BITMAP::kRgb:
		return 3;
	case PDF_BITMAP::kRgb32:
	case PDF_BITMAP::kArgb:
		return 4;
	}
	return -1;
}

