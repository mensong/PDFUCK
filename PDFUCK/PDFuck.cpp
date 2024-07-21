#include "pch.h"
#include "PDFuck.h"
#include <fstream>
#include <fpdfview.h>
#include <fpdf_edit.h>
#include "PDF_DOCUMENT_imp.h"

bool g_Initialized = false;
PDF_API void GlobalInitializeLibrary()
{
	FPDF_LIBRARY_CONFIG config;
	config.m_pIsolate = NULL;
	config.m_pPlatform = NULL;
	config.m_v8EmbedderSlot = NULL;
	config.m_pUserFontPaths = NULL;
	config.version = 2;
	FPDF_InitLibraryWithConfig(&config);
	g_Initialized = true;
}

PDF_API void GlobalDestroyLibrary()
{
	if (!g_Initialized)
		return;
	FPDF_DestroyLibrary();
	g_Initialized = false;
}


PDF_API PDF_DOCUMENT* CreateDocument()
{
	FPDF_DOCUMENT doc = FPDF_CreateNewDocument();
	return new PDF_DOCUMENT_imp(doc);
}

PDF_API PDF_DOCUMENT* LoadDocumentFromFile(const char* file_path, const char* password)
{
	FPDF_DOCUMENT doc = FPDF_LoadDocument(file_path, password);
	return new PDF_DOCUMENT_imp(doc);

}

PDF_API PDF_DOCUMENT* LoadDocumentFromMemory(const void* data_buf, size_t size, const char* password)
{
	FPDF_DOCUMENT doc = FPDF_LoadMemDocument64(data_buf, size, password);
	return new PDF_DOCUMENT_imp(doc);
}

PDF_API void DestroyDocument(PDF_DOCUMENT** doc)
{
	PDF_DOCUMENT_imp* p = dynamic_cast<PDF_DOCUMENT_imp*>(*doc);
	delete p;
	*doc = NULL;
}


