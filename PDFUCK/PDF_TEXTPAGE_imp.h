#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdf_text.h>

class PDF_PAGE_imp;

class PDF_TEXTPAGE_imp
	: public PDF_TEXTPAGE
{
public:
	FPDF_TEXTPAGE m_textPage;

	PDF_TEXTPAGE_imp()
		: m_textPage(NULL)
	{

	}
	PDF_TEXTPAGE_imp(FPDF_TEXTPAGE textPage)
		: m_textPage(textPage)
	{

	}
	~PDF_TEXTPAGE_imp()
	{
		if (m_textPage)
		{
			FPDFText_ClosePage(m_textPage);
			m_textPage = NULL;
		}
	}

	// 通过 PDF_TEXTPAGE 继承
	virtual int CountChars() override;

	virtual int GetCharIndexAtPos(float x, float y, float xTolerance, float yTolerance) override;

	virtual wchar_t GetChar(int index) override;

	virtual float GetCharFontSize(int index) override;

	virtual unsigned long GetCharFontInfo(int index, char* bufferFontNameUtf8, unsigned long buflen, int* flags) override;

	virtual int GetCharFontWeight(int index) override;

	virtual PDF_TEXT_RENDERMODE GetCharTextRenderMode(int index) override;

	virtual bool GetCharFillColor(int index, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A) override;

	virtual bool GetCharStrokeColor(int index, uint8_t* R, uint8_t* G, uint8_t* B, uint8_t* A) override;

	virtual float GetCharAngle(int index) override;

	virtual bool GetCharBox(int index, float* left, float* right, float* bottom, float* top) override;

	virtual bool GetCharLooseCharBox(int index, float* left, float* right, float* bottom, float* top) override;

	virtual bool GetCharMatrix(int index, float* a, float* b, float* c, float* d, float* e, float* f) override;

	virtual bool GetCharOrigin(int index, float* x, float* y) override;

	virtual int GetText(int start_index, int count, wchar_t* resultBuff) override;

	virtual int CountRects(int start_index = 0, int count = 0) override;

	virtual bool GetRect(int rect_index, float* left, float* top, float* right, float* bottom) override;

	virtual int GetTextByRect(float left, float top, float right, float bottom, wchar_t* buffer, int buflen) override;

	virtual PDF_FINDER* FindStart(wchar_t* findwhat, unsigned long flags, int start_index) override;

	virtual void FindClose(PDF_FINDER** finder) override;

	virtual PDF_PAGELINK* OpenWebLinks() override;

	virtual void CloseWebLinks(PDF_PAGELINK** link_page) override;


	// 通过 PDF_TEXTPAGE 继承
	virtual PDF_TEXTCHAR_RTREE* NewTextCharRTree() override;

	virtual void CloseTextCharRTree(PDF_TEXTCHAR_RTREE** rt) override;


	// 通过 PDF_TEXTPAGE 继承
	virtual int GetCharIndexFromTextIndex(int nTextIndex) override;

	virtual int GetTextIndexFromCharIndex(int nCharIndex) override;

};

