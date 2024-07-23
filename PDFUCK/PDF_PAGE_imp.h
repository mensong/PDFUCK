#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdfview.h>

class PDF_DOCUMENT_imp;

class PDF_PAGE_imp
	: public PDF_PAGE
{
public:
	FPDF_PAGE m_page;

public:
	PDF_PAGE_imp(FPDF_PAGE page)
		: m_page(page)
	{

	}
	~PDF_PAGE_imp()
	{
		if (m_page)
		{
			FPDF_ClosePage(m_page);
			m_page = NULL;
		}
	}


	// 通过 PDF_PAGE 继承
	virtual float GetWidth() override;

	virtual float GetHeight() override;

	virtual PAGE_RATEION GetRotation() override;

	virtual void SetRotation(PAGE_RATEION rotate) override;

	virtual bool HasTransparency() override;

	virtual bool GetBoundingBox(float* left, float* top, float* right, float* bottom) override;

	virtual void TransformAllAnnots(float a, float b, float c, float d, float e, float f) override;

	virtual void InsertPageObject(PDF_PAGEOBJECT* pageObj) override;

	virtual bool RemovePageObject(PDF_PAGEOBJECT* pageObj) override;

	virtual int CountPageObjects() override;

	virtual PDF_PAGEOBJECT* OpenPageObject(int idx) override;

	virtual void ClosePageObject(PDF_PAGEOBJECT** pageObj) override;

	virtual PDF_TEXTPAGE* OpenTextPage() override;

	virtual void CloseTextPage(PDF_TEXTPAGE** text_page) override;

	virtual bool CommitChange() override;

	virtual void RenderToDC(HDC dc, int start_x, int start_y, int size_x, int size_y, int rotate, int flags) override;

	virtual void RenderToBitmap(PDF_BITMAP* bitmap, int start_x, int start_y, int size_x, int size_y, int rotate, int flags) override;

	virtual PDF_PAGEOBJECT_RTREE* NewRTree() override;

	virtual void CloseRTree(PDF_PAGEOBJECT_RTREE** rt) override;


	// 通过 PDF_PAGE 继承
	virtual void RenderToBitmapEx(PDF_BITMAP* bitmap, float a, float b, float c, float d, float e, float f, float left, float top, float right, float bottom, int flags) override;

};


