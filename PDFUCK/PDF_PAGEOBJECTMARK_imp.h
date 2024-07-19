#pragma once
#include "PDFuck.h"
#include "PDF_imp.h"
#include <fpdfview.h>

class PDF_PAGEOBJECT_imp;

class PDF_PAGEOBJECTMARK_imp
	: public PDF_PAGEOBJECTMARK
{
public:
	FPDF_PAGEOBJECTMARK m_mark;

public:

	PDF_PAGEOBJECTMARK_imp();
	PDF_PAGEOBJECTMARK_imp(FPDF_PAGEOBJECTMARK mark);
	~PDF_PAGEOBJECTMARK_imp();

	// Í¨¹ý PDF_PAGEOBJECTMARK ¼Ì³Ð
	virtual bool GetName(wchar_t* buffer, unsigned long buflen, unsigned long* out_buflen) override;

	virtual int CountParams() override;

	virtual bool GetParamKey(unsigned long index, wchar_t* buffer, unsigned long buflen, unsigned long* out_buflen) override;

	virtual PDF_OBJECT_TYPE GetParamValueType(const char* key) override;

	virtual bool GetParamIntValue(const char* key, int* out_value) override;

	virtual bool GetParamStringValue(const char* key, wchar_t* buffer, unsigned long buflen, unsigned long* out_buflen) override;

	virtual bool GetParamBlobValue(const char* key, void* buffer, unsigned long buflen, unsigned long* out_buflen) override;

	virtual bool SetIntParam(PDF_DOCUMENT* doc, PDF_PAGEOBJECT* pageObj, const char* key, int value) override;

	virtual bool SetStringParam(PDF_DOCUMENT* doc, PDF_PAGEOBJECT* pageObj, const char* key, const char* value) override;

	virtual bool SetBlobParam(PDF_DOCUMENT* doc, PDF_PAGEOBJECT* pageObj, const char* key, void* value, unsigned long value_len) override;

	virtual bool RemoveParam(PDF_PAGEOBJECT* pageObj, const char* key) override;

};
