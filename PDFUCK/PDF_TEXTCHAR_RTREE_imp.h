#pragma once
#include "PDFuck.h"
#include "RTreeEx.h"

class PDF_TEXTCHAR_RTREE_imp
	: public PDF_TEXTCHAR_RTREE
{
public:
	RT::RTreeEx<int, float, 2> m_rtree;
	PDF_TEXTPAGE* m_textPage;

	PDF_TEXTCHAR_RTREE_imp(PDF_TEXTPAGE* textPage)
		: m_textPage(textPage)
		, m_rtree(-1)
	{

	}
	~PDF_TEXTCHAR_RTREE_imp()
	{

	}

	// ͨ�� PDF_TEXTCHAR_RTREE �̳�
	virtual bool AppendTextPageAllChars() override;

	// ͨ�� PDF_TEXTCHAR_RTREE �̳�
	virtual bool AppendCharIndex(int charIndex) override;

	virtual int SearchCharIndexByAABBBox(float minPos[2], float maxPos[2], int* arrOutIndexBuffer, int bufferLen, float threshold) override;

};

