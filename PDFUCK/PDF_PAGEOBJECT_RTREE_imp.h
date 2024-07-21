#pragma once
#include "PDFuck.h"
#include "RTreeEx.h"

class PDF_PAGEOBJECT;
class PDF_PAGE;

class PDF_PAGEOBJECT_RTREE_imp
	: public PDF_PAGEOBJECT_RTREE
{
public:
	RT::RTreeEx<PDF_PAGEOBJECT*, float, 2> m_rtree;

public:
	PDF_PAGEOBJECT_RTREE_imp()
		: m_rtree(NULL)
	{
	}
	~PDF_PAGEOBJECT_RTREE_imp()
	{

	}

	// ͨ�� PDF_PAGEOBJECT_RTREE �̳�
	virtual bool AppendPageObject(PDF_PAGEOBJECT* pageObject) override;

	// ͨ�� PDF_PAGEOBJECT_RTREE �̳�
	virtual int SearchPageObjectsByAABBBox(float minPos[2], float maxPos[2],
		PDF_PAGEOBJECT** arrOutPageObjectBuffer, int bufferLen, float threshold = 0.01) override;
};

