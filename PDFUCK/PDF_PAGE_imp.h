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


	// Í¨¹ý PDF_PAGE ¼Ì³Ð
	virtual double GetWidth() override;

	virtual double GetHeight() override;

	virtual int GetRotation() override;

	virtual void SetRotation(int rotate) override;

	virtual bool HasTransparency() override;

	virtual bool GetBoundingBox(float* left, float* top, float* right, float* bottom) override;

	virtual void TransformAllAnnots(double a, double b, double c, double d, double e, double f) override;

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

};


