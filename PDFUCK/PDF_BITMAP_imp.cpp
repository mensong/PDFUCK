#include "pch.h"
#include "PDF_BITMAP_imp.h"
#include <cpdfsdk_helpers.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
////#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

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

int _write_bmp(char const* filename, int x, int y, int comp, const void* data)
{
	stbi__write_context s = { 0 };
	if (stbi__start_write_file(&s, filename)) {
		int r = 0;
		//int r = stbi_write_bmp_core(&s, x, y, comp, data);
		{
			if (comp != 4) {
				// write RGB bitmap
				int pad = (-x * 3) & 3;
				r = stbiw__outfile(&s, 1/*mensong*/, -1, x, y, comp, 1, (void*)data, 0, pad,
					"11 4 22 4" "4 44 22 444444",
					'B', 'M', 14 + 40 + (x * 3 + pad) * y, 0, 0, 14 + 40,  // file header
					40, x, y, 1, 24, 0, 0, 0, 0, 0, 0);             // bitmap header
			}
			else {
				// RGBA bitmaps need a v4 header
				// use BI_BITFIELDS mode with 32bpp and alpha mask
				// (straight BI_RGB with alpha mask doesn't work in most readers)
				r = stbiw__outfile(&s, 1/*mensong*/, -1, x, y, comp, 1, (void*)data, 1, 0,
					"11 4 22 4" "4 44 22 444444 4444 4 444 444 444 444",
					'B', 'M', 14 + 108 + x * y * 4, 0, 0, 14 + 108, // file header
					108, x, y, 1, 32, 3, 0, 0, 0, 0, 0, 0xff0000, 0xff00, 0xff, 0xff000000u, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // bitmap V4 header
			}
		}
		stbi__end_write_file(&s);
		return r;
	}
	else
		return 0;
}

unsigned char* _png_to_mem(const unsigned char* pixels, int stride_bytes, int x, int y, int n, int* out_len)
{
	int force_filter = stbi_write_force_png_filter;
	int ctype[5] = { -1, 0, 4, 2, 6 };
	unsigned char sig[8] = { 137,80,78,71,13,10,26,10 };
	unsigned char* out, * o, * filt, * zlib;
	signed char* line_buffer;
	int j, zlen;

	if (stride_bytes == 0)
		stride_bytes = x * n;

	if (force_filter >= 5) {
		force_filter = -1;
	}

	filt = (unsigned char*)STBIW_MALLOC((x * n + 1) * y); if (!filt) return 0;
	line_buffer = (signed char*)STBIW_MALLOC(x * n); if (!line_buffer) { STBIW_FREE(filt); return 0; }
	for (j = 0; j < y; ++j) {
		int filter_type;
		if (force_filter > -1) {
			filter_type = force_filter;
			stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, force_filter, line_buffer);
		}
		else { // Estimate the best filter by running through all of them:
			int best_filter = 0, best_filter_val = 0x7fffffff, est, i;
			for (filter_type = 0; filter_type < 5; filter_type++) {
				stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, filter_type, line_buffer);

				// Estimate the entropy of the line using this filter; the less, the better.
				est = 0;
				for (i = 0; i < x * n; ++i) {
					est += abs((signed char)line_buffer[i]);
				}
				if (est < best_filter_val) {
					best_filter_val = est;
					best_filter = filter_type;
				}
			}
			if (filter_type != best_filter) {  // If the last iteration already got us the best filter, don't redo it
				stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, best_filter, line_buffer);
				filter_type = best_filter;
			}
		}

		//·´×ªRB//mensong
		for (int i = 0; i < x; i++)
		{
			int idx = i * n;
			if (idx + 2 >= x * n)
				break;
			std::swap(line_buffer[idx], line_buffer[idx + 2]);
		}

		// when we get here, filter_type contains the filter type, and line_buffer contains the data
		filt[j * (x * n + 1)] = (unsigned char)filter_type;
		STBIW_MEMMOVE(filt + j * (x * n + 1) + 1, line_buffer, x * n);
	}
	STBIW_FREE(line_buffer);
	zlib = stbi_zlib_compress(filt, y * (x * n + 1), &zlen, stbi_write_png_compression_level);
	STBIW_FREE(filt);
	if (!zlib) return 0;

	// each tag requires 12 bytes of overhead
	out = (unsigned char*)STBIW_MALLOC(8 + 12 + 13 + 12 + zlen + 12);
	if (!out) return 0;
	*out_len = 8 + 12 + 13 + 12 + zlen + 12;

	o = out;
	STBIW_MEMMOVE(o, sig, 8); o += 8;
	stbiw__wp32(o, 13); // header length
	stbiw__wptag(o, "IHDR");
	stbiw__wp32(o, x);
	stbiw__wp32(o, y);
	*o++ = 8;
	*o++ = STBIW_UCHAR(ctype[n]);
	*o++ = 0;
	*o++ = 0;
	*o++ = 0;
	stbiw__wpcrc(&o, 13);

	stbiw__wp32(o, zlen);
	stbiw__wptag(o, "IDAT");
	STBIW_MEMMOVE(o, zlib, zlen);
	o += zlen;
	STBIW_FREE(zlib);
	stbiw__wpcrc(&o, zlen);

	stbiw__wp32(o, 0);
	stbiw__wptag(o, "IEND");
	stbiw__wpcrc(&o, 0);

	STBIW_ASSERT(o == out + *out_len);

	return out;
}

int _write_png(char const* filename, int x, int y, int comp, const void* data, int stride_bytes)
{
	FILE* f;
	int len;
	unsigned char* png = _png_to_mem((const unsigned char*)data, stride_bytes, x, y, comp, &len);
	if (png == NULL) return 0;

	f = stbiw__fopen(filename, "wb");
	if (!f) { STBIW_FREE(png); return 0; }
	fwrite(png, 1, len, f);
	fclose(f);
	STBIW_FREE(png);
	return 1;
}

static int _write_jpg_core(stbi__write_context* s, int width, int height, int comp, const void* data, int quality) {
	// Constants that don't pollute global namespace
	static const unsigned char std_dc_luminance_nrcodes[] = { 0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };
	static const unsigned char std_dc_luminance_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	static const unsigned char std_ac_luminance_nrcodes[] = { 0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d };
	static const unsigned char std_ac_luminance_values[] = {
	   0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
	   0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
	   0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
	   0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
	   0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
	   0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
	   0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
	};
	static const unsigned char std_dc_chrominance_nrcodes[] = { 0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };
	static const unsigned char std_dc_chrominance_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	static const unsigned char std_ac_chrominance_nrcodes[] = { 0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77 };
	static const unsigned char std_ac_chrominance_values[] = {
	   0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
	   0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
	   0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
	   0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
	   0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
	   0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
	   0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
	};
	// Huffman tables
	static const unsigned short YDC_HT[256][2] = { {0,2},{2,3},{3,3},{4,3},{5,3},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9} };
	static const unsigned short UVDC_HT[256][2] = { {0,2},{1,2},{2,2},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9},{1022,10},{2046,11} };
	static const unsigned short YAC_HT[256][2] = {
	   {10,4},{0,2},{1,2},{4,3},{11,4},{26,5},{120,7},{248,8},{1014,10},{65410,16},{65411,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {12,4},{27,5},{121,7},{502,9},{2038,11},{65412,16},{65413,16},{65414,16},{65415,16},{65416,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {28,5},{249,8},{1015,10},{4084,12},{65417,16},{65418,16},{65419,16},{65420,16},{65421,16},{65422,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {58,6},{503,9},{4085,12},{65423,16},{65424,16},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {59,6},{1016,10},{65430,16},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {122,7},{2039,11},{65438,16},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {123,7},{4086,12},{65446,16},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {250,8},{4087,12},{65454,16},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {504,9},{32704,15},{65462,16},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {505,9},{65470,16},{65471,16},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {506,9},{65479,16},{65480,16},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {1017,10},{65488,16},{65489,16},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {1018,10},{65497,16},{65498,16},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {2040,11},{65506,16},{65507,16},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {65515,16},{65516,16},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {2041,11},{65525,16},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
	};
	static const unsigned short UVAC_HT[256][2] = {
	   {0,2},{1,2},{4,3},{10,4},{24,5},{25,5},{56,6},{120,7},{500,9},{1014,10},{4084,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {11,4},{57,6},{246,8},{501,9},{2038,11},{4085,12},{65416,16},{65417,16},{65418,16},{65419,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {26,5},{247,8},{1015,10},{4086,12},{32706,15},{65420,16},{65421,16},{65422,16},{65423,16},{65424,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {27,5},{248,8},{1016,10},{4087,12},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{65430,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {58,6},{502,9},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{65438,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {59,6},{1017,10},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{65446,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {121,7},{2039,11},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{65454,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {122,7},{2040,11},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{65462,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {249,8},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{65470,16},{65471,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {503,9},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{65479,16},{65480,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {504,9},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{65488,16},{65489,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {505,9},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{65497,16},{65498,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {506,9},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{65506,16},{65507,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {2041,11},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{65515,16},{65516,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {16352,14},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{65525,16},{0,0},{0,0},{0,0},{0,0},{0,0},
	   {1018,10},{32707,15},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
	};
	static const int YQT[] = { 16,11,10,16,24,40,51,61,12,12,14,19,26,58,60,55,14,13,16,24,40,57,69,56,14,17,22,29,51,87,80,62,18,22,
							  37,56,68,109,103,77,24,35,55,64,81,104,113,92,49,64,78,87,103,121,120,101,72,92,95,98,112,100,103,99 };
	static const int UVQT[] = { 17,18,24,47,99,99,99,99,18,21,26,66,99,99,99,99,24,26,56,99,99,99,99,99,47,66,99,99,99,99,99,99,
							   99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
	static const float aasf[] = { 1.0f * 2.828427125f, 1.387039845f * 2.828427125f, 1.306562965f * 2.828427125f, 1.175875602f * 2.828427125f,
								  1.0f * 2.828427125f, 0.785694958f * 2.828427125f, 0.541196100f * 2.828427125f, 0.275899379f * 2.828427125f };

	int row, col, i, k, subsample;
	float fdtbl_Y[64], fdtbl_UV[64];
	unsigned char YTable[64], UVTable[64];

	if (!data || !width || !height || comp > 4 || comp < 1) {
		return 0;
	}

	quality = quality ? quality : 90;
	subsample = quality <= 90 ? 1 : 0;
	quality = quality < 1 ? 1 : quality > 100 ? 100 : quality;
	quality = quality < 50 ? 5000 / quality : 200 - quality * 2;

	for (i = 0; i < 64; ++i) {
		int uvti, yti = (YQT[i] * quality + 50) / 100;
		YTable[stbiw__jpg_ZigZag[i]] = (unsigned char)(yti < 1 ? 1 : yti > 255 ? 255 : yti);
		uvti = (UVQT[i] * quality + 50) / 100;
		UVTable[stbiw__jpg_ZigZag[i]] = (unsigned char)(uvti < 1 ? 1 : uvti > 255 ? 255 : uvti);
	}

	for (row = 0, k = 0; row < 8; ++row) {
		for (col = 0; col < 8; ++col, ++k) {
			fdtbl_Y[k] = 1 / (YTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
			fdtbl_UV[k] = 1 / (UVTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
		}
	}

	// Write Headers
	{
		static const unsigned char head0[] = { 0xFF,0xD8,0xFF,0xE0,0,0x10,'J','F','I','F',0,1,1,0,0,1,0,1,0,0,0xFF,0xDB,0,0x84,0 };
		static const unsigned char head2[] = { 0xFF,0xDA,0,0xC,3,1,0,2,0x11,3,0x11,0,0x3F,0 };
		const unsigned char head1[] = { 0xFF,0xC0,0,0x11,8,(unsigned char)(height >> 8),STBIW_UCHAR(height),(unsigned char)(width >> 8),STBIW_UCHAR(width),
										3,1,(unsigned char)(subsample ? 0x22 : 0x11),0,2,0x11,1,3,0x11,1,0xFF,0xC4,0x01,0xA2,0 };
		s->func(s->context, (void*)head0, sizeof(head0));
		s->func(s->context, (void*)YTable, sizeof(YTable));
		stbiw__putc(s, 1);
		s->func(s->context, UVTable, sizeof(UVTable));
		s->func(s->context, (void*)head1, sizeof(head1));
		s->func(s->context, (void*)(std_dc_luminance_nrcodes + 1), sizeof(std_dc_luminance_nrcodes) - 1);
		s->func(s->context, (void*)std_dc_luminance_values, sizeof(std_dc_luminance_values));
		stbiw__putc(s, 0x10); // HTYACinfo
		s->func(s->context, (void*)(std_ac_luminance_nrcodes + 1), sizeof(std_ac_luminance_nrcodes) - 1);
		s->func(s->context, (void*)std_ac_luminance_values, sizeof(std_ac_luminance_values));
		stbiw__putc(s, 1); // HTUDCinfo
		s->func(s->context, (void*)(std_dc_chrominance_nrcodes + 1), sizeof(std_dc_chrominance_nrcodes) - 1);
		s->func(s->context, (void*)std_dc_chrominance_values, sizeof(std_dc_chrominance_values));
		stbiw__putc(s, 0x11); // HTUACinfo
		s->func(s->context, (void*)(std_ac_chrominance_nrcodes + 1), sizeof(std_ac_chrominance_nrcodes) - 1);
		s->func(s->context, (void*)std_ac_chrominance_values, sizeof(std_ac_chrominance_values));
		s->func(s->context, (void*)head2, sizeof(head2));
	}

	// Encode 8x8 macroblocks
	{
		static const unsigned short fillBits[] = { 0x7F, 7 };
		int DCY = 0, DCU = 0, DCV = 0;
		int bitBuf = 0, bitCnt = 0;
		// comp == 2 is grey+alpha (alpha is ignored)
		int ofsG = comp > 2 ? 1 : 0, ofsR = comp > 2 ? 2 : 0;//mensong
		const unsigned char* dataB = (const unsigned char*)data;//mensong
		const unsigned char* dataG = dataB + ofsG;//mensong
		const unsigned char* dataR = dataB + ofsR;//mensong
		int x, y, pos;
		if (subsample) {
			for (y = 0; y < height; y += 16) {
				for (x = 0; x < width; x += 16) {
					float Y[256], U[256], V[256];
					for (row = y, pos = 0; row < y + 16; ++row) {
						// row >= height => use last input row
						int clamped_row = (row < height) ? row : height - 1;
						int base_p = (stbi__flip_vertically_on_write ? (height - 1 - clamped_row) : clamped_row) * width * comp;
						for (col = x; col < x + 16; ++col, ++pos) {
							// if col >= width => use pixel from last input column
							int p = base_p + ((col < width) ? col : (width - 1)) * comp;
							float r = dataR[p], g = dataG[p], b = dataB[p];
							Y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
							U[pos] = -0.16874f * r - 0.33126f * g + 0.50000f * b;
							V[pos] = +0.50000f * r - 0.41869f * g - 0.08131f * b;
						}
					}
					DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 0, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
					DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 8, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
					DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 128, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
					DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 136, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);

					// subsample U,V
					{
						float subU[64], subV[64];
						int yy, xx;
						for (yy = 0, pos = 0; yy < 8; ++yy) {
							for (xx = 0; xx < 8; ++xx, ++pos) {
								int j = yy * 32 + xx * 2;
								subU[pos] = (U[j + 0] + U[j + 1] + U[j + 16] + U[j + 17]) * 0.25f;
								subV[pos] = (V[j + 0] + V[j + 1] + V[j + 16] + V[j + 17]) * 0.25f;
							}
						}
						DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, subU, 8, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
						DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, subV, 8, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
					}
				}
			}
		}
		else {
			for (y = 0; y < height; y += 8) {
				for (x = 0; x < width; x += 8) {
					float Y[64], U[64], V[64];
					for (row = y, pos = 0; row < y + 8; ++row) {
						// row >= height => use last input row
						int clamped_row = (row < height) ? row : height - 1;
						int base_p = (stbi__flip_vertically_on_write ? (height - 1 - clamped_row) : clamped_row) * width * comp;
						for (col = x; col < x + 8; ++col, ++pos) {
							// if col >= width => use pixel from last input column
							int p = base_p + ((col < width) ? col : (width - 1)) * comp;
							float r = dataR[p], g = dataG[p], b = dataB[p];
							Y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
							U[pos] = -0.16874f * r - 0.33126f * g + 0.50000f * b;
							V[pos] = +0.50000f * r - 0.41869f * g - 0.08131f * b;
						}
					}

					DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y, 8, fdtbl_Y, DCY, YDC_HT, YAC_HT);
					DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, U, 8, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
					DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, V, 8, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
				}
			}
		}

		// Do the bit alignment of the EOI marker
		stbiw__jpg_writeBits(s, &bitBuf, &bitCnt, fillBits);
	}

	// EOI
	stbiw__putc(s, 0xFF);
	stbiw__putc(s, 0xD9);

	return 1;
}

int _write_jpg(char const* filename, int x, int y, int comp, const void* data, int quality)
{
	stbi__write_context s = { 0 };
	if (stbi__start_write_file(&s, filename)) {
		int r = _write_jpg_core(&s, x, y, comp, data, quality);
		stbi__end_write_file(&s);
		return r;
	}
	else
		return 0;
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
		result = _write_bmp(filePath, wd, hgt, num_channels, data);
		break;
	case 1:
		result = _write_png(filePath, wd, hgt, num_channels, data, stride);
		break;
	case 2:
		result = _write_jpg(filePath, wd, hgt, num_channels, data, 85);
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

void PDF_BITMAP_imp::ClearWidthColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
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

void PDF_BITMAP_imp::SetPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
	int height = GetHeight();
	int width = GetWidth();
	int pitch = GetStride();
	uint8_t* pBuffer = GetBuffer();
	CFX_DIBitmap* pBitmap = CFXDIBitmapFromFPDFBitmap(m_bitmap);
	//uint32_t color = (A << 24) | (B << 16) | (G << 8) | R;
	uint32_t color = ArgbEncode(A, R, G, B);

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

void PDF_BITMAP_imp::GetPixel(int x, int y, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A)
{
	uint32_t color = GetPixel(x, y);
	*A = FXARGB_A(color);
	*R = FXARGB_R(color);
	*G = FXARGB_G(color);
	*B = FXARGB_B(color);
}

