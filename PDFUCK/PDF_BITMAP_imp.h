#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdfview.h>

class PDF_BITMAP_imp
	: public PDF_BITMAP
{
public:
	FPDF_BITMAP m_bitmap;

public:
	PDF_BITMAP_imp();
	PDF_BITMAP_imp(FPDF_BITMAP mp);
	~PDF_BITMAP_imp();

	// 通过 PDF_BITMAP 继承
	virtual FORMAT GetFormat() override;

	virtual void FillRect(int left, int top, int width, int height, unsigned long color) override;

	virtual void* GetBuffer() override;

	virtual int GetWidth() override;

	virtual int GetHeight() override;

	virtual int GetStride() override;


	// 通过 PDF_BITMAP 继承
	virtual bool WriteToFile(const char* filePath) override;


	// 通过 PDF_BITMAP 继承
	virtual int GetNumChannels() override;

};


