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

	virtual void RenderToDC(HDC dc, 
		int x_inDC, int y_inDC, int size_x_inDC, int size_y_inDC,
		PAGE_RATEION rotate_inDC, int flags = 0) override;

	virtual void RenderToDC_DoubleBuffer(HDC dc, int x_inDC, int y_inDC, int size_x_inDC, int size_y_inDC,
		PAGE_RATEION rotate_inDC, COLORREF colorTransparent, int flags = 0) override;

	virtual void RenderToBitmap(PDF_BITMAP* bitmap, 
		int x_inBitmap, int y_inBitmap, int size_x_inBitmap, int size_y_inBitmap,
		PAGE_RATEION rotate_inBitmap, int flags = 0) override;

	virtual void RenderToBitmapEx(PDF_BITMAP* bitmap,
		float a, float b, float c, float d, float e, float f,
		float left_inBitmap, float top_inBitmap, float right_inBitmap, float bottom_inBitmap,
		int flags = 0) override;

	virtual PDF_PAGEOBJECT_RTREE* NewRTree() override;

	virtual void CloseRTree(PDF_PAGEOBJECT_RTREE** rt) override;
		
	virtual void SetMediaBox(float left, float bottom, float right, float top) override;

	virtual void SetCropBox(float left, float bottom, float right, float top) override;

	virtual void SetBleedBox(float left, float bottom, float right, float top) override;

	virtual void SetTrimBox(float left, float bottom, float right, float top) override;

	virtual void SetArtBox(float left, float bottom, float right, float top) override;

	virtual bool GetMediaBox(float* left, float* bottom, float* right, float* top) override;

	virtual bool GetCropBox(float* left, float* bottom, float* right, float* top) override;

	virtual bool GetBleedBox(float* left, float* bottom, float* right, float* top) override;

	virtual bool GetTrimBox(float* left, float* bottom, float* right, float* top) override;

	virtual bool GetArtBox(float* left, float* bottom, float* right, float* top) override;
		
	virtual bool TransformWithClip(const PDF_MATRIX* matrix, const PDF_RECT* clipRect) override;
			
	virtual PDF_CLIPPATH* CreateClipPath(float left, float bottom, float right, float top) override;

	virtual void InsertClipPath(PDF_CLIPPATH* clipPath) override;

	virtual void CloseClipPath(PDF_CLIPPATH** clipPath) override;

};


