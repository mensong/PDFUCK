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

	// ͨ�� PDF_BITMAP �̳�
	virtual FORMAT GetFormat() override;

	virtual void FillRect(int left, int top, int width, int height, unsigned long color) override;

	virtual uint8_t* GetBuffer() override;

	virtual int GetWidth() override;

	virtual int GetHeight() override;

	virtual int GetStride() override;


	// ͨ�� PDF_BITMAP �̳�
	virtual bool WriteToFile(const char* filePath) override;


	// ͨ�� PDF_BITMAP �̳�
	virtual int GetNumChannels() override;


	// ͨ�� PDF_BITMAP �̳�
	virtual void ClearWidthColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;


	// ͨ�� PDF_BITMAP �̳�
	virtual bool CopyFrom(PDF_BITMAP* src) override;


	// ͨ�� PDF_BITMAP �̳�
	virtual bool ConvertFormat(FORMAT format) override;

	// ͨ�� PDF_BITMAP �̳�
	virtual void SetPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;


	// ͨ�� PDF_BITMAP �̳�
	uint32_t GetPixel(int x, int y);
	virtual void GetPixel(int x, int y, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A) override;

};


