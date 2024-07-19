#include "pch.h"
#include "PDF_DOCUMENT_imp.h"
#include <fpdf_edit.h>
#include "PDF_PAGEOBJECT_imp.h"
#include "PDF_PAGE_imp.h"
#include "PDF_FONT_imp.h"
#include "PDF_BITMAP_imp.h"
#include <fpdf_ppo.h>

bool PDF_DOCUMENT_imp::GetFileVersion(int* outVer)
{
	return FPDF_GetFileVersion(m_doc, outVer);
}

// 通过 PDF_DOCUMENT 继承
bool PDF_DOCUMENT_imp::HasValidCrossReferenceTable()
{
	return FPDF_DocumentHasValidCrossReferenceTable(m_doc);
}

// 通过 PDF_DOCUMENT 继承
unsigned long PDF_DOCUMENT_imp::GetPermissions()
{
	return FPDF_GetDocPermissions(m_doc);
}

// 通过 PDF_DOCUMENT 继承
int PDF_DOCUMENT_imp::GetSecurityHandlerRevision()
{
	return FPDF_GetSecurityHandlerRevision(m_doc);
}

// 通过 PDF_DOCUMENT 继承
int PDF_DOCUMENT_imp::CountPages()
{
	return FPDF_GetPageCount(m_doc);
}

// 通过 PDF_DOCUMENT 继承
PDF_PAGE* PDF_DOCUMENT_imp::OpenPage(int pageIdx)
{
	FPDF_PAGE page = FPDF_LoadPage(m_doc, pageIdx);
	if (!page)
		return NULL;
	return new PDF_PAGE_imp(page);
}

void PDF_DOCUMENT_imp::ClosePage(PDF_PAGE** page)
{
	PDF_PAGE_imp* p = dynamic_cast<PDF_PAGE_imp*>(*page);
	delete p;
	*page = NULL;
}

// 通过 PDF_DOCUMENT 继承
PDF_PAGE* PDF_DOCUMENT_imp::NewPage(int page_index, double width, double height)
{
	auto page = FPDFPage_New(m_doc, page_index, width, height);
	if (!page)
		return NULL;
	return new PDF_PAGE_imp(page);
}

// 通过 PDF_DOCUMENT 继承
void PDF_DOCUMENT_imp::DeletePage(int pageIdx)
{
}
PDF_PAGEOBJECT* PDF_DOCUMENT_imp::NewImagePageObject()
{
	FPDF_PAGEOBJECT po = FPDFPageObj_NewImageObj(m_doc);
	if (!po)
		return NULL;

	return new PDF_PAGEOBJECT_imp(po);
}
PDF_PAGEOBJECT* PDF_DOCUMENT_imp::NewPathPageObject(float x, float y)
{
	auto o = FPDFPageObj_CreateNewPath(x, y);
	if (!o)
		return NULL;
	auto ret = new PDF_PAGEOBJECT_imp(o);

	//防止默认情况下不出来
	ret->Path_SetDrawMode(PDF_PAGEOBJECT::PDF_FILLMODE_NONE, true);

	return ret;
}
PDF_PAGEOBJECT* PDF_DOCUMENT_imp::NewRectPageObject(float x, float y, float w, float h)
{
	auto o = FPDFPageObj_CreateNewRect(x, y, w, h);
	if (!o)
		return NULL;
	PDF_PAGEOBJECT_imp* ret = new PDF_PAGEOBJECT_imp(o);

	//防止默认情况下不出来	
	ret->Path_SetDrawMode(PDF_PAGEOBJECT::PDF_FILLMODE_ALTERNATE, false);

	return ret;
}
PDF_PAGEOBJECT* PDF_DOCUMENT_imp::NewTextPageObject(const char* font_withoutspaces, float font_size)
{
	FPDF_PAGEOBJECT o = FPDFPageObj_NewTextObj(m_doc, font_withoutspaces, font_size);
	if (!o)
		return NULL;
	return new PDF_PAGEOBJECT_imp(o);
}
PDF_PAGEOBJECT* PDF_DOCUMENT_imp::NewTextPageObject(PDF_FONT* font, float font_size)
{
	PDF_FONT_imp* ifont = dynamic_cast<PDF_FONT_imp*>(font);
	auto o = FPDFPageObj_CreateTextObj(m_doc, ifont->m_font, font_size);
	if (!o)
		return NULL;
	return new PDF_PAGEOBJECT_imp(o);
}

void PDF_DOCUMENT_imp::DestroyNotYetManagedPageObject(PDF_PAGEOBJECT* pageObj)
{
	PDF_PAGEOBJECT_imp* ipageObj = IMP(PDF_PAGEOBJECT, pageObj);
	FPDFPageObj_Destroy(ipageObj->m_obj);
}

PDF_FONT* PDF_DOCUMENT_imp::LoadFontFromMemory(const uint8_t* data, uint32_t size,
	PDF_FONT::FONT_TYPE font_type, bool cid)
{
	auto font = FPDFText_LoadFont(m_doc, data, size, font_type, cid);
	if (!font)
		return NULL;
	return new PDF_FONT_imp(font);
}
PDF_FONT* PDF_DOCUMENT_imp::LoadStandardFont(const char* fontWithoutSpaces)
{
	auto font = FPDFText_LoadStandardFont(m_doc, fontWithoutSpaces);
	if (!font)
		return NULL;
	return new PDF_FONT_imp(font);
}
void PDF_DOCUMENT_imp::CloseFont(PDF_FONT** font)
{
	PDF_FONT_imp* ifont = dynamic_cast<PDF_FONT_imp*>(*font);
	delete ifont;
	*font = NULL;
}

// 通过 PDF_DOCUMENT 继承
static int _WriteBlock(struct FPDF_FILEWRITE_* pThis, const void* pData, unsigned long size)
{
	PDF_DOCUMENT_imp* This = (PDF_DOCUMENT_imp*)pThis;
	This->m_pdfFile.write((const char*)pData, size);
	return 1;
}
bool PDF_DOCUMENT_imp::SaveTo(const char* filePath, SAVE_FLAG flag)
{
	m_pdfFile.open(filePath, std::ios::out | std::ios::binary);
	if (!m_pdfFile.is_open())
		return false;

	this->version = 1;
	this->WriteBlock = _WriteBlock;
	bool res = FPDF_SaveAsCopy(m_doc, this, flag);
	m_pdfFile.close();
	return res;
}

PDF_BITMAP* PDF_DOCUMENT_imp::NewBitmap(int width, int height, int alpha)
{
	return new PDF_BITMAP_imp(width, height, alpha);
}

void PDF_DOCUMENT_imp::CloseBitmap(PDF_BITMAP** bitmap)
{
	auto o = IMP(PDF_BITMAP, bitmap);
	delete o;
	*bitmap = NULL;
}

bool PDF_DOCUMENT_imp::ImportPagesFrom(PDF_DOCUMENT* src_doc, const char* page_range, int insertAt)
{
	return FPDF_ImportPages(m_doc, IMP(PDF_DOCUMENT, src_doc)->m_doc, page_range, insertAt);
}

PDF_DOCUMENT* PDF_DOCUMENT_imp::ExportNPagesToOne(
	float output_width, float output_height, 
	size_t num_pages_on_x_axis, size_t num_pages_on_y_axis)
{
	auto doc = FPDF_ImportNPagesToOne(m_doc, output_width, output_height,
		num_pages_on_x_axis, num_pages_on_y_axis);
	if (!doc)
		return NULL;
	return new PDF_DOCUMENT_imp(doc);
}

bool PDF_DOCUMENT_imp::CopyViewerPreferencesFrom(PDF_DOCUMENT* src_doc)
{
	return FPDF_CopyViewerPreferences(m_doc, IMP(PDF_DOCUMENT, src_doc)->m_doc);
}

// 通过 PDF_DOCUMENT 继承
void PDF_DOCUMENT_imp::ClosePageObject(PDF_PAGEOBJECT** pageObj)
{
	PDF_PAGEOBJECT_imp* ipageObj = IMP(PDF_PAGEOBJECT, *pageObj);
	delete ipageObj;
	*pageObj = NULL;
}

// 通过 PDF_DOCUMENT 继承
bool PDF_DOCUMENT_imp::GetPageSizeByIndex(int page_index, double* width, double* height)
{
	return FPDF_GetPageSizeByIndex(m_doc, page_index, width, height);
}