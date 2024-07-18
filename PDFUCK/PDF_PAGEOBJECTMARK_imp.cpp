#include "pch.h"
#include "PDF_PAGEOBJECTMARK_imp.h"
#include <fpdf_edit.h>


PDF_PAGEOBJECTMARK_imp::PDF_PAGEOBJECTMARK_imp()
	: m_mark(NULL)
{

}
PDF_PAGEOBJECTMARK_imp::PDF_PAGEOBJECTMARK_imp(FPDF_PAGEOBJECTMARK mark)
	: m_mark(mark)
{

}

PDF_PAGEOBJECTMARK_imp::~PDF_PAGEOBJECTMARK_imp()
{
}

// Í¨¹ý PDF_PAGEOBJECTMARK ¼Ì³Ð
bool PDF_PAGEOBJECTMARK_imp::GetName(wchar_t* buffer, unsigned long buflen, unsigned long* out_buflen)
{
	return FPDFPageObjMark_GetName(m_mark, buffer, buflen, out_buflen);
}

int PDF_PAGEOBJECTMARK_imp::CountParams()
{
	return FPDFPageObjMark_CountParams(m_mark);
}

bool PDF_PAGEOBJECTMARK_imp::GetParamKey(unsigned long index, wchar_t* buffer, unsigned long buflen, unsigned long* out_buflen)
{
	return FPDFPageObjMark_GetParamKey(m_mark, index, buffer, buflen, out_buflen);
}

PDF_OBJECT_TYPE PDF_PAGEOBJECTMARK_imp::GetParamValueType(const char* key)
{
	return (PDF_OBJECT_TYPE)FPDFPageObjMark_GetParamValueType(m_mark, key);
}

bool PDF_PAGEOBJECTMARK_imp::GetParamIntValue(const char* key, int* out_value)
{
	return FPDFPageObjMark_GetParamIntValue(m_mark, key, out_value);
}

bool PDF_PAGEOBJECTMARK_imp::GetParamStringValue(const char* key, void* buffer, unsigned long buflen, unsigned long* out_buflen)
{
	return FPDFPageObjMark_GetParamStringValue(m_mark, key, buffer, buflen, out_buflen);
}

bool PDF_PAGEOBJECTMARK_imp::GetParamBlobValue(const char* key, void* buffer, unsigned long buflen, unsigned long* out_buflen)
{
	return FPDFPageObjMark_GetParamBlobValue(m_mark, key, buffer, buflen, out_buflen);
}

bool PDF_PAGEOBJECTMARK_imp::SetIntParam(const char* key, int value)
{
	//return FPDFPageObjMark_SetIntParam(m_mark, key, value);
	return NULL;
}

bool PDF_PAGEOBJECTMARK_imp::SetStringParam(const char* key, const char* out_value)
{
	return false;
}

bool PDF_PAGEOBJECTMARK_imp::SetBlobParam(const char* key, void* value, unsigned long value_len)
{
	return false;
}

bool PDF_PAGEOBJECTMARK_imp::RemoveParam(const char* key)
{
	return false;
}
