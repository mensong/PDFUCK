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
uint8_t* PDF_BITMAP_imp::GetBuffer()
{
	return (uint8_t*)FPDFBitmap_GetBuffer(m_bitmap);
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

void PDF_BITMAP_imp::ClearWidthColor(int R, int G, int B, int A)
{
	uint32_t intColor = (A << 24) | (B << 16) | (G << 8) | R;

	CFX_DIBitmap* pBitmap = CFXDIBitmapFromFPDFBitmap(m_bitmap);
	pBitmap->Clear(intColor);
}

bool PDF_BITMAP_imp::CopyFrom(PDF_BITMAP* src)
{
	CFX_DIBitmap* pBitmap = CFXDIBitmapFromFPDFBitmap(m_bitmap);
	CFX_DIBitmap* pSrcBitmap = CFXDIBitmapFromFPDFBitmap(IMP(PDF_BITMAP, src)->m_bitmap);
	
	if (!pBitmap->Create(pSrcBitmap->GetWidth(), pSrcBitmap->GetHeight(), pSrcBitmap->GetFormat()))
		return false;

	pBitmap->SetPalette(pSrcBitmap->GetPaletteSpan());
	pBitmap->SetAlphaMask(pSrcBitmap->m_pAlphaMask, nullptr);
	for (int row = 0; row < pSrcBitmap->GetHeight(); row++)
	{
		memcpy(pBitmap->GetBuffer() + row * pBitmap->GetPitch(),
			pSrcBitmap->GetScanline(row), pBitmap->GetPitch());
	}
	return true;
}

bool PDF_BITMAP_imp::ConvertFormat(FORMAT format)
{
	CFX_DIBitmap* pBitmap = CFXDIBitmapFromFPDFBitmap(m_bitmap);
	return pBitmap->ConvertFormat((FXDIB_Format)format);
}

uint32_t PDF_BITMAP_imp::GetPixel(int x, int y)
{
	int height = GetHeight();
	int width = GetWidth();
	int pitch = GetStride();
	uint8_t* pBuffer = GetBuffer();
	CFX_DIBitmap* pBitmap = CFXDIBitmapFromFPDFBitmap(m_bitmap);

	uint8_t* pos = pBuffer + y * pitch + x * pBitmap->GetBPP() / 8;
	switch (GetFormat()) {
	case FORMAT::k1bppMask: {
		if ((*pos) & (1 << (7 - x % 8))) {
			return 0xff000000;
		}
		return 0;
	}
	case FORMAT::k1bppRgb: {
		if ((*pos) & (1 << (7 - x % 8))) {
			return pBitmap->HasPalette() ? pBitmap->GetPaletteSpan()[1] : 0xffffffff;
		}
		return pBitmap->HasPalette() ? pBitmap->GetPaletteSpan()[0] : 0xff000000;
	}
	case FORMAT::k8bppMask:
		return (*pos) << 24;
	case FORMAT::k8bppRgb:
		return pBitmap->HasPalette() ? pBitmap->GetPaletteSpan()[*pos]
			: ArgbEncode(0xff, *pos, *pos, *pos);
	case FORMAT::kRgb:
	case FORMAT::kRgb32:
		return FXARGB_GETDIB(pos) | 0xff000000;
	case FORMAT::kArgb:
		return FXARGB_GETDIB(pos);
	default:
		break;
	}
	return 0;

}

void PDF_BITMAP_imp::SetPixel(int x, int y, int R, int G, int B, int A)
{
	int height = GetHeight();
	int width = GetWidth();
	int pitch = GetStride();
	uint8_t* pBuffer = GetBuffer();
	CFX_DIBitmap* pBitmap = CFXDIBitmapFromFPDFBitmap(m_bitmap);
	uint32_t color = (A << 24) | (B << 16) | (G << 8) | R;

	if (x < 0 || x >= width || y < 0 || y >= height)
		return;

	uint8_t* pos = pBuffer + y * pitch + x * pBitmap->GetBPP() / 8;
	switch (GetFormat()) {
	case FORMAT::k1bppMask:
		if (color >> 24) {
			*pos |= 1 << (7 - x % 8);
		}
		else {
			*pos &= ~(1 << (7 - x % 8));
		}
		break;
	case FORMAT::k1bppRgb:
		if (pBitmap->HasPalette()) {
			if (color == pBitmap->GetPaletteSpan()[1]) {
				*pos |= 1 << (7 - x % 8);
			}
			else {
				*pos &= ~(1 << (7 - x % 8));
			}
		}
		else {
			if (color == 0xffffffff) {
				*pos |= 1 << (7 - x % 8);
			}
			else {
				*pos &= ~(1 << (7 - x % 8));
			}
		}
		break;
	case FORMAT::k8bppMask:
		*pos = (uint8_t)(color >> 24);
		break;
	case FORMAT::k8bppRgb: {
		if (pBitmap->HasPalette()) {
			pdfium::span<const uint32_t> palette = pBitmap->GetPaletteSpan();
			for (int i = 0; i < 256; i++) {
				if (palette[i] == color) {
					*pos = (uint8_t)i;
					return;
				}
			}
			*pos = 0;
		}
		else {
			*pos = FXRGB2GRAY(FXARGB_R(color), FXARGB_G(color), FXARGB_B(color));
		}
		break;
	}
	case FORMAT::kRgb:
	case FORMAT::kRgb32: {
		int alpha = FXARGB_A(color);
		pos[0] = (FXARGB_B(color) * alpha + pos[0] * (255 - alpha)) / 255;
		pos[1] = (FXARGB_G(color) * alpha + pos[1] * (255 - alpha)) / 255;
		pos[2] = (FXARGB_R(color) * alpha + pos[2] * (255 - alpha)) / 255;
		break;
	}
	case FORMAT::kArgb:
		FXARGB_SETDIB(pos, color);
		break;
	default:
		break;
	}
}

