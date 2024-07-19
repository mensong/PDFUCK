#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdf_save.h>
#include <fstream>

class PDF_DOCUMENT_imp
	: public PDF_DOCUMENT
	, public FPDF_FILEWRITE
{
public:
	FPDF_DOCUMENT m_doc;

	std::ofstream m_pdfFile;

public:
	PDF_DOCUMENT_imp()
		: m_doc(NULL)
	{

	}
	PDF_DOCUMENT_imp(FPDF_DOCUMENT doc)
		: m_doc(doc)
	{

	}
	~PDF_DOCUMENT_imp()
	{

	}

	

	// 通过 PDF_DOCUMENT 继承
	virtual bool GetFileVersion(int* outVer) override;

	virtual bool HasValidCrossReferenceTable() override;

	virtual unsigned long GetPermissions() override;

	virtual int GetSecurityHandlerRevision() override;

	virtual int CountPages() override;

	virtual PDF_PAGE* OpenPage(int pageIdx) override;

	virtual PDF_PAGE* NewPage(int page_index, double width, double height) override;

	virtual void ClosePage(PDF_PAGE** page) override;

	virtual void DeletePage(int pageIdx) override;

	virtual bool GetPageSizeByIndex(int page_index, double* width, double* height) override;

	virtual PDF_PAGEOBJECT* NewImagePageObject() override;

	virtual PDF_PAGEOBJECT* NewPathPageObject(float x, float y) override;

	virtual PDF_PAGEOBJECT* NewRectPageObject(float x, float y, float w, float h) override;

	virtual PDF_PAGEOBJECT* NewTextPageObject(const char* font_withoutspaces, float font_size) override;

	virtual PDF_PAGEOBJECT* NewTextPageObject(PDF_FONT* font, float font_size) override;

	virtual void DestroyNotYetManagedPageObject(PDF_PAGEOBJECT* pageObj) override;

	virtual void ClosePageObject(PDF_PAGEOBJECT** pageObj) override;

	virtual PDF_FONT* LoadFontFromMemory(const uint8_t* data, uint32_t size, PDF_FONT::FONT_TYPE font_type, bool cid) override;

	virtual PDF_FONT* LoadStandardFont(const char* fontWithoutSpaces) override;

	virtual void CloseFont(PDF_FONT** font) override;

	virtual bool SaveTo(const char* filePath, SAVE_FLAG flag) override;


	// 通过 PDF_DOCUMENT 继承
	virtual PDF_BITMAP* NewBitmap(int width, int height, int alpha) override;


	// 通过 PDF_DOCUMENT 继承
	virtual void CloseBitmap(PDF_BITMAP** bitmap) override;


	// 通过 PDF_DOCUMENT 继承
	virtual bool ImportPagesFrom(PDF_DOCUMENT* src_doc, const char* page_range, int insertAt) override;

	virtual PDF_DOCUMENT* ExportNPagesToOne(float output_width, float output_height, size_t num_pages_on_x_axis, size_t num_pages_on_y_axis) override;

	virtual bool CopyViewerPreferencesFrom(PDF_DOCUMENT* src_doc) override;

};