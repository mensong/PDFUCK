// ComparePdf.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include "../PDFUCK/PDFuck.h"
#include "../PDFUCK/RTreeEx.h"
#include "../pystring/pystring.h"

bool get_common_string(const std::string& s1, const std::string& s2, std::string& comStr, int& pos1, int& pos2)
{
	if (s1 == s2)
	{
		comStr = s1;
		pos1 = 0;
		pos2 = 0;
		return true;
	}

	const int len1 = s1.size();
	const int len2 = s2.size();

	std::vector<std::vector<int> > dp(len1 + 1, std::vector<int>(len2 + 1, 0));

	auto max_pos = std::make_pair(0, 0);

	for (int i = 1; i <= len1; ++i) {
		for (int j = 1; j <= len2; ++j) {
			if (s1[i - 1] == s2[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;

				max_pos = (dp[i][j] > dp[max_pos.first][max_pos.second]) ?
					std::make_pair(i, j) : max_pos;
			}
		}
	}

	std::vector<char> result_vec;
	while (dp[max_pos.first][max_pos.second] != 0) {
		result_vec.push_back(s1[max_pos.first - 1]);
		--max_pos.first;
		--max_pos.second;
	}

	if (result_vec.size() == 0)
		return false;

	comStr = std::string(result_vec.rbegin(), result_vec.rend());
	pos1 = max_pos.first;
	pos2 = max_pos.second;
	return true;
}

bool get_common_stringW(const std::wstring& s1, const std::wstring& s2, std::wstring& comStr, int& pos1, int& pos2)
{
	if (s1 == s2)
	{
		comStr = s1;
		pos1 = 0;
		pos2 = 0;
		return true;
	}

	const int len1 = s1.size();
	const int len2 = s2.size();

	std::vector<std::vector<int> > dp(len1 + 1, std::vector<int>(len2 + 1, 0));

	auto max_pos = std::make_pair(0, 0);

	for (int i = 1; i <= len1; ++i) {
		for (int j = 1; j <= len2; ++j) {
			if (s1[i - 1] == s2[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;

				max_pos = (dp[i][j] > dp[max_pos.first][max_pos.second]) ?
					std::make_pair(i, j) : max_pos;
			}
		}
	}

	std::vector<wchar_t> result_vec;
	while (dp[max_pos.first][max_pos.second] != 0) {
		result_vec.push_back(s1[max_pos.first - 1]);
		--max_pos.first;
		--max_pos.second;
	}

	if (result_vec.size() == 0)
		return false;

	comStr = std::wstring(result_vec.rbegin(), result_vec.rend());
	pos1 = max_pos.first;
	pos2 = max_pos.second;
	return true;
}


void renderDiff(PDF_DOCUMENT* doc, PDF_PAGE* page, float left, float top, float right, float bottom)
{
	float diffuse = 1.0;
	auto rectObj = doc->NewRectPageObject(0 - diffuse, 0 - diffuse, 
		right - left + diffuse * 2, top - bottom + diffuse * 2);
	rectObj->SetFillColor(255, 0, 0, 50);
	//rectObj->SetStrokeColor(255, 0, 0, 255);
	rectObj->Transform(1, 0, 0, 1, left, bottom);
	page->InsertPageObject(rectObj);
	doc->ClosePageObject(&rectObj);
}



bool comparePageText(PDF_DOCUMENT* docRender, PDF_PAGE* pageRender, PDF_PAGE* pageOther)
{
	bool pageHasDiff = false;
	wchar_t textBuf1[256];
	wchar_t textBuf2[256];

	int readed;

	auto pageTextRender = pageRender->OpenTextPage();
	auto pageTextOther = pageOther->OpenTextPage();

#if 0
	float left, top, right, bottom;

	int countRectRender = pageTextRender->CountRects();
	int countRectOther = pageTextOther->CountRects();
	for (int r = 0; r < countRectRender; r++)
	{
		textBuf1[0] = 0;
		textBuf2[0] = 0;

		pageTextRender->GetRect(r, &left, &top, &right, &bottom);
		std::wcout << left << " " << top << " " << right << " " << bottom << " ";
		readed = pageTextRender->GetTextByRect(left, top, right, bottom, textBuf1, 256);
		if (readed > 0 && readed < 256)
		{
			std::wcout << textBuf1;
		}
		std::wcout << std::endl;

		bool otherOneHasText = false;
		readed = pageTextOther->GetTextByRect(left, top, right, bottom, textBuf2, 256);
		if (readed > 0 && readed < 256)
		{
			otherOneHasText = true;
			std::wcout << textBuf1;
		}
		std::wcout << std::endl;

		if (otherOneHasText && wcscmp(textBuf1, textBuf2) == 0)
		{//相同

		}
		else
		{//不相同
			renderDiff(docRender, pageRender, left, top, right, bottom);
			pageHasDiff = true;
		}
	}
#elif 0

	int countChars = pageTextRender->CountChars();
	for (int i = 0; i < countChars; i++)
	{
		wchar_t c1 = pageTextRender->GetChar(i);
		float x, y;
		pageTextRender->GetCharOrigin(i, &x, &y);
		int idxCharInOther = pageTextOther->GetCharIndexAtPos(x, y, 0.01, 0.01);
		wchar_t c2 = pageTextOther->GetChar(idxCharInOther);
		if (c1 != c2)
		{
			float left, right, bottom, top;
			pageTextRender->GetCharBox(i, &left, &right, &bottom, &top);
			renderDiff(docRender, pageRender, left, top, right, bottom);
		}
	}

#else

	auto rt = pageTextOther->NewTextCharRTree();
	rt->AppendTextPageAllChars();

	float x, y;
	float left, right, bottom, top;
	int countChars = pageTextRender->CountChars();
	for (int i = 0; i < countChars; i++)
	{
		wchar_t c1 = pageTextRender->GetChar(i);
		wchar_t c2;
		pageTextRender->GetCharBox(i, &left, &right, &bottom, &top);
		float minPos[2] = { left, bottom };
		float maxPos[2] = { right, top };
		int result[2];
		int resultLen = rt->SearchCharIndexByAABBBox(minPos, maxPos, result, 2);
		bool isDiff = true;
		for (int r = 0; r < resultLen; r++)
		{
			c2 = pageTextOther->GetChar(result[r]);
			if (c1 == c2)
			{
				isDiff = false;
				break;
			}
		}
		if (isDiff)
		{
			renderDiff(docRender, pageRender, left, top, right, bottom);
		}
	}

	pageTextOther->CloseTextCharRTree(&rt);

#endif

	pageRender->CloseTextPage(&pageTextRender);
	pageOther->CloseTextPage(&pageTextOther);
	return pageHasDiff;
}



struct COMMON_STRING_INFO
{
	int pos1, pos2;
	std::wstring comStr;
};
void discoveryCommonText(std::vector<COMMON_STRING_INFO>& outComStrs, 
	const std::wstring& str1, const std::wstring& str2, int baseOffset1 = 0, int baseOffset2 = 0)
{
	std::wstring comStr;
	int pos1, pos2;
	bool found = get_common_stringW(str1, str2, comStr, pos1, pos2);
	if (!found)
		return;

	COMMON_STRING_INFO strInfo;
	strInfo.comStr = comStr;
	strInfo.pos1 = pos1 + baseOffset1;
	strInfo.pos2 = pos2 + baseOffset2;
	outComStrs.push_back(strInfo);

	if (pos1 != 0 && pos2 != 0)
	{
		std::wstring strLeft1 = str1.substr(0, pos1);
		std::wstring strLeft2 = str2.substr(0, pos2);
		discoveryCommonText(outComStrs, strLeft1, strLeft2, baseOffset1, baseOffset2);
	}

	std::wstring strRight1 = str1.substr(pos1 + comStr.size());
	std::wstring strRight2 = str2.substr(pos2 + comStr.size());
	if (strRight1.size() > 0 && strRight2.size() > 0)
	{
		discoveryCommonText(outComStrs, strRight1, strRight2,
			baseOffset1 + pos1 + comStr.size(), baseOffset2 + pos2 + comStr.size());
	}
}

bool comparePageTextWithCommonString(PDF_DOCUMENT* doc1, PDF_PAGE* page1, PDF_DOCUMENT* doc2, PDF_PAGE* page2)
{
	auto textPage1 = page1->OpenTextPage();
	auto textPage2 = page2->OpenTextPage();

	std::wstring text1;
	int countChars1 = textPage1->CountChars();
	for (int i = 0; i < countChars1; i++)
	{
		text1 += textPage1->GetChar(i);
	}
	std::wstring text2;
	int countChars2 = textPage2->CountChars();
	for (int i = 0; i < countChars2; i++)
	{
		text2 += textPage2->GetChar(i);
	}

	bool found = false;
	std::vector<COMMON_STRING_INFO> outComStrs;
	discoveryCommonText(outComStrs, text1, text2);

	//做字符的相同标志
	std::vector<bool> commFlags1(text1.size(), false);
	std::vector<bool> commFlags2(text2.size(), false);
	for (size_t i = 0; i < outComStrs.size(); i++)
	{
		std::fill(commFlags1.begin() + outComStrs[i].pos1, 
			commFlags1.begin() + (outComStrs[i].pos1 + outComStrs[i].comStr.size()), true);
		std::fill(commFlags2.begin() + outComStrs[i].pos2,
			commFlags2.begin() + (outComStrs[i].pos2 + outComStrs[i].comStr.size()), true);
	}
	
	//根据标志来判断是否相同并渲染
	float left, right, bottom, top;
	for (int i = 0; i < countChars1 && i < text1.size(); i++)
	{
		if (!commFlags1[i])
		{
			if (textPage1->GetCharBox(i, &left, &right, &bottom, &top))
			{
				renderDiff(doc1, page1, left, top, right, bottom);
			}
			
		}
	}
	for (int i = 0; i < countChars2 && i < text2.size(); i++)
	{
		if (!commFlags2[i])
		{
			if (textPage2->GetCharBox(i, &left, &right, &bottom, &top))
			{
				renderDiff(doc2, page2, left, top, right, bottom);
			}

		}
	}

	page2->CloseTextPage(&textPage2);
	page1->CloseTextPage(&textPage1);

	return true;
}



bool IsPathObjectSame(PDF_PAGEOBJECT* pathObj1, PDF_PAGEOBJECT* pathObj2)
{
	if (pathObj1->Path_IsClosed() != pathObj2->Path_IsClosed())
		return false;

	PDF_PAGEOBJECT::PDF_FILLMODE fileMode1;
	PDF_PAGEOBJECT::PDF_FILLMODE fileMode2;
	bool drawLine1 = true;
	bool drawLine2 = true;
	if (pathObj1->Path_GetDrawMode(&fileMode1, &drawLine1) &&
		pathObj2->Path_GetDrawMode(&fileMode2, &drawLine2))
	{
		if (fileMode1 != fileMode2 || drawLine1 != drawLine2)
			return false;
	}

	int countSegs1 = pathObj1->Path_CountSegments();
	if (countSegs1 != pathObj2->Path_CountSegments())
		return false;

	bool bSegDiff = false;
	for (int i = 0; i < countSegs1; i++)
	{
		PDF_PATHSEGMENT* seg1 = pathObj1->Path_OpenSegment(i);
		PDF_PATHSEGMENT* seg2 = pathObj1->Path_OpenSegment(i);

		if ((bool)seg1 != (bool)seg2)
		{
			bSegDiff = true;
		}

		if (seg1 && seg2)
		{
			if (seg1->GetType() != seg2->GetType())
			{
				bSegDiff = true;
			}
		}

		if (seg1)
			pathObj1->Path_CloseSegment(&seg1);
		if (seg2)
			pathObj2->Path_CloseSegment(&seg2);

		if (bSegDiff)
			break;
	}
	if (bSegDiff)
		return false;

	return true;
}

bool IsImageObjectSame(PDF_PAGEOBJECT* imgObj1, PDF_PAGEOBJECT* imgObj2)
{
	unsigned long dataLen1 = imgObj1->Image_GetDataRaw(NULL, 0);
	unsigned long dataLen2 = imgObj2->Image_GetDataRaw(NULL, 0);
	if (dataLen1 != dataLen2)
		return false;

	byte* raw1 = new byte[dataLen1];
	if (imgObj1->Image_GetDataRaw(raw1, dataLen1) != dataLen1)
	{
		delete[] raw1;
		return false;
	}
	
	byte* raw2 = new byte[dataLen2];
	if (imgObj2->Image_GetDataRaw(raw2, dataLen2) != dataLen2)
	{
		delete[] raw1;
		delete[] raw2;
		return false;
	}

	bool isDiff = false;
	for (size_t i = 0; i < dataLen1; i++)
	{
		if (raw1[i] != raw2[i])
		{
			isDiff = true;
			break;
		}
	}

	delete[] raw1;
	delete[] raw2;

	return !isDiff;
}

bool compareOtherObject(PDF_DOCUMENT* docRender, PDF_PAGE* pageRender, PDF_PAGE* pageOther)
{
	bool pageHasDiff = false;
	float left, bottom, right, top;

#if 0
	std::vector<RECT> rects;
	int countObjs = pageRender->CountPageObjects();
	for (int i = 0; i < countObjs; i++)
	{
		auto obj = pageRender->OpenPageObject(i);
		obj->GetBounds(&left, &top, &right, &bottom);
		RECT r;
		r.left = left;
		r.top = top;
		r.right = right;
		r.bottom = bottom;
		rects.push_back(r);

		pageRender->ClosePageObject(&obj);
	}

	for (size_t i = 0; i < rects.size(); i++)
	{
		auto pathRect = docRender->NewRectPageObject(0, 0,
			rects[i].right - rects[i].left, rects[i].top - rects[i].bottom);
		pathRect->SetStrokeColor(255, 0, 0, 255);
		pathRect->Path_SetDrawMode(PDF_PAGEOBJECT::PDF_FILLMODE_NONE, true);
		pathRect->Transform(1, 0, 0, 1, rects[i].left, rects[i].bottom);
		pageRender->InsertPageObject(pathRect);
		docRender->ClosePageObject(&pathRect);
	}
#else

	std::vector<PDF_PAGEOBJECT*> pageObjectsBuf;
	auto rt = pageOther->NewRTree();
	int countObjsOther = pageOther->CountPageObjects();
	for (int i = 0; i < countObjsOther; i++)
	{
		auto pageObj = pageOther->OpenPageObject(i);
		PDF_PAGE_OBJECT_TYPE objType = pageObj->GetType();
		//只处理path/image
		if (objType == PDF_PAGEOBJ_PATH || objType == PDF_PAGEOBJ_IMAGE)
		{
			rt->AppendPageObject(pageObj);
			pageObjectsBuf.push_back(pageObj);
		}
		else
		{
			pageOther->ClosePageObject(&pageObj);
		}
	}

	//对比
	PDF_PAGEOBJECT* pageSearchedInOther[5];
	int countObjsRender = pageRender->CountPageObjects();
	for (int i = 0; i < countObjsRender; i++)
	{
		auto pageObj = pageRender->OpenPageObject(i);
		PDF_PAGE_OBJECT_TYPE objType = pageObj->GetType();
		//处理path
		if (objType == PDF_PAGEOBJ_PATH || objType == PDF_PAGEOBJ_IMAGE)
		{
			//auto countSegs = pageObj->Path_CountSegments();	
			if (pageObj->GetBounds(&left, &bottom, &right, &top))
			{
				float minPos[2] = { left , bottom };
				float maxPos[2] = { right , top };

				bool isPathDiff = true;
				int searchCount = rt->SearchPageObjectsByAABBBox(minPos, maxPos, pageSearchedInOther, 5);
				for (int se = 0; se < searchCount; se++)
				{
					if (objType == PDF_PAGEOBJ_PATH)
					{
						if (IsPathObjectSame(pageSearchedInOther[se], pageObj))
						{
							isPathDiff = false;
							break;
						}
					}
					else if (objType == PDF_PAGEOBJ_IMAGE)
					{
						if (IsImageObjectSame(pageSearchedInOther[se], pageObj))
						{
							isPathDiff = false;
							break;
						}
					}

				}

				if (isPathDiff)
				{
					renderDiff(docRender, pageRender, left, top, right, bottom);
					pageHasDiff = true;
				}
			}
		}
		pageRender->ClosePageObject(&pageObj);
	}

	for (size_t i = 0; i < pageObjectsBuf.size(); i++)
	{
		pageOther->ClosePageObject(&pageObjectsBuf[i]);
	}

#endif

	return pageHasDiff;
}



void CompareLeftRight(const std::string& pdf1, const std::string& pdf2, 
	const std::string& leftPdf, const std::string& rightPdf, int compareType)
{
    auto doc1 = PDFuck::Ins().LoadDocumentFromFile(pdf1.c_str(), NULL);
    auto doc2 = PDFuck::Ins().LoadDocumentFromFile(pdf2.c_str(), NULL);

    int pagesCount = min(doc1->CountPages(), doc2->CountPages());
    for (int i = 0; i < pagesCount; i++)
    {
        auto page1 = doc1->OpenPage(i);
        auto page2 = doc2->OpenPage(i);

		bool page1TextHasDiff = false;
		bool page2TextHasDiff = false;
		bool page1PathHasDiff = false;
		bool page2PathHasDiff = false;

		if (compareType == 0 || compareType == 1)
		{
			//page1TextHasDiff = comparePageText(doc1, page1, page2);
			//page2TextHasDiff = comparePageText(doc2, page2, page1);
			page1TextHasDiff = comparePageTextWithCommonString(doc1, page1, doc2, page2);
			page2TextHasDiff = page1TextHasDiff;
		}

		if (compareType == 0 || compareType == 2)
		{
			page1PathHasDiff = compareOtherObject(doc1, page1, page2);
			page2PathHasDiff = compareOtherObject(doc2, page2, page1);
		}

        if (page1TextHasDiff || page1PathHasDiff)
            page1->CommitChange();
        if (page2TextHasDiff || page2PathHasDiff)
            page2->CommitChange();

        doc1->ClosePage(&page1);
        doc2->ClosePage(&page2);
    }

    doc1->SaveTo(leftPdf.c_str(), PDF_DOCUMENT::PDF_NO_INCREMENTAL);
    doc2->SaveTo(rightPdf.c_str(), PDF_DOCUMENT::PDF_NO_INCREMENTAL);

    PDFuck::Ins().CloseDocument(&doc1);
    PDFuck::Ins().CloseDocument(&doc2);
}



bool compareTextOverride(PDF_DOCUMENT* docRender, PDF_PAGE* pageRender, PDF_PAGE* pageOther)
{
	bool isPageDiff = false;
	float left, right, bottom, top;

	auto textPageRender = pageRender->OpenTextPage();
	auto textPageOther = pageRender->OpenTextPage();
	auto rt = textPageRender->NewTextCharRTree();
	rt->AppendTextPageAllChars();

	int countCharsRender = textPageRender->CountChars();
	for (int i = 0; i < countCharsRender; i++)
	{
		wchar_t c1 = textPageRender->GetChar(i);
		if (!textPageRender->GetCharBox(i, &left, &right, &bottom, &top))
			continue;

		bool isDiff = true;
		float minPos[2] = { left, bottom };
		float maxPos[2] = { right, top };
		int arrIndex[3];
		int resultLen = rt->SearchCharIndexByAABBBox(minPos, maxPos, arrIndex, 3);
		for (int r = 0; r < resultLen; r++)
		{
			wchar_t c2 = textPageOther->GetChar(arrIndex[r]);
			if (c1 == c2)
			{
				isDiff = false;
				break;
			}
		}

		if (isDiff)
		{
			//textPage1->GetCharOrigin(i);
			//textPage1->GetCharFontSize(i);
			
			isPageDiff = true;
		}
	}

	textPageRender->CloseTextCharRTree(&rt);
	pageRender->CloseTextPage(&textPageRender);
	pageOther->CloseTextPage(&textPageOther);

	return isPageDiff;
}

void CompareOverride(const std::string& pdf1, const std::string& pdf2, const std::string& mergePdf, int compareType = 0)
{
	auto doc1 = PDFuck::Ins().LoadDocumentFromFile(pdf1.c_str(), NULL);
	auto doc2 = PDFuck::Ins().LoadDocumentFromFile(pdf2.c_str(), NULL);
	auto docMerge = PDFuck::Ins().CreateDocument();

	docMerge->SetDefaultFontFilePath("simfang.ttf");

	int countPages1 = doc1->CountPages();
	int countPages2 = doc2->CountPages();

	for (int i = 0; i < countPages1 && i < countPages2; i++)
	{
		auto page1 = doc1->OpenPage(i);
		auto page2 = doc2->OpenPage(i);

		int width = (int)page1->GetWidth();
		int height = (int)page1->GetHeight();

		auto bitmap1 = doc1->NewBitmap(width, height, true);
		page1->RenderToBitmap(bitmap1, 0, 0, width, height, PDF_PAGE::PAGE_RATEION_0, 0);
		auto bitmap2 = doc2->NewBitmap(width, height, true);
		page2->RenderToBitmap(bitmap2, 0, 0, width, height, PDF_PAGE::PAGE_RATEION_0, 0);

		//bitmap1->ClearWidthColor(255, 0, 0, 255);
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				bitmap1->SetPixel(x, y, 255, 0, 0, 100);
			}
		}
		bitmap1->WriteToFile((std::to_string(i) + ".png").c_str());

		doc1->CloseBitmap(&bitmap1);
		doc2->CloseBitmap(&bitmap2);

		doc1->ClosePage(&page1);
		doc2->ClosePage(&page2);
	}

	docMerge->SaveTo(mergePdf.c_str(), PDF_DOCUMENT::PDF_NO_INCREMENTAL);

	PDFuck::Ins().CloseDocument(&docMerge);
	PDFuck::Ins().CloseDocument(&doc2);
}


int main(int argc, char** argv)
{
	//std::wstring text1 = L"123abcde234";
	//std::wstring text2 = L"x123xabcde5236";
	//std::vector<COMMON_STRING_INFO> outComStrs;
	//discoveryCommonText(outComStrs, text1, text2);
	//std::vector<bool> commFlags1(text1.size(), false);
	//std::vector<bool> commFlags2(text2.size(), false);
	//for (size_t i = 0; i < outComStrs.size(); i++)
	//{
	//	std::fill(commFlags1.begin() + outComStrs[i].pos1,
	//		commFlags1.begin() + (outComStrs[i].pos1 + outComStrs[i].comStr.size()), true);
	//	std::fill(commFlags2.begin() + outComStrs[i].pos2,
	//		commFlags2.begin() + (outComStrs[i].pos2 + outComStrs[i].comStr.size()), true);
	//}
	//return 0;

	auto funcUsage = []()
	{
		std::cout << "Usage:" 
			<< " -pdf1=pdf1" 
			<< " -pdf2=pdf2" 
			<< " -mode=leftright/merge"
			<< " -left=leftPdf" 
			<< " -right=rightPdf" 
			<< " -merge=mergePdf" 
			<< " -onlytext"
			<< " -onlyshape"
			<< std::endl;
	};

	std::string pdf1;
	std::string pdf2;
	std::string mode;
	std::string leftPdf, rightPdf;
	std::string mergePdf;
	int compareType = 0;//0:全部；1:仅文本；2:仅图形

	auto funcSplitArg = [](const std::string& arg, std::string& key, std::string& val)->void
	{
		if (pystring::startswith(arg, "-"))
		{
			std::string argkv = pystring::lstrip(arg, "-");
			size_t idx = argkv.find('=');
			if (idx == std::string::npos)
			{//只有标志
				key = argkv;
			}
			else
			{//有KV
				key = argkv.substr(0, idx);
				val = argkv.substr(idx + 1);
			}
		}
		else
		{
			val = arg;
		}
	};

	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		std::string key, val;
		funcSplitArg(arg, key, val);
		if (key.empty())
			continue;

		if (pystring::equal(key,	  "pdf1", true))
			pdf1 = val;
		else if (pystring::equal(key, "pdf2", true))
			pdf2 = val;
		else if (pystring::equal(key, "mode", true))
			mode = val;
		else if (pystring::equal(key, "left", true))
			leftPdf = val;
		else if (pystring::equal(key, "right", true))
			rightPdf = val;
		else if (pystring::equal(key, "merge", true))
			mergePdf = val;
		else if (pystring::equal(key, "onlytext", true))
		{
			if (compareType == 0)
				compareType = 1;
			else
				std::cout << "已设置onlyshape，忽略onlytext" << std::endl;
		}
		else if (pystring::equal(key, "onlyshape", true))
		{
			if (compareType == 0)
				compareType = 2;
			else
				std::cout << "已设置onlytext，忽略onlyshape" << std::endl;
		}
	}
    
	if (pdf1.empty() || pdf2.empty() || mode.empty())
	{
		funcUsage();
		return 2;
	}

	if (leftPdf.empty())
	{
		std::string dir = os_path::dirname(pdf1);
		leftPdf = os_path::join(dir, os_path::basename_no_ext(pdf1) + "_diff.pdf");
	}
	if (rightPdf.empty())
	{
		std::string dir = os_path::dirname(pdf2);
		rightPdf = os_path::join(dir, os_path::basename_no_ext(pdf2) + "_diff.pdf");
	}
	if (mergePdf.empty())
	{
		std::string dir = os_path::dirname(pdf1);
		mergePdf = os_path::join(dir, 
			os_path::basename_no_ext(pdf1) + "_" + os_path::basename_no_ext(pdf2) + ".pdf");
	}

	if (pystring::equal(mode, "leftright", true))
	{
		CompareLeftRight(pdf1, pdf2, leftPdf, rightPdf, compareType);
		CompareOverride(pdf1, pdf2, "", 0);
	}
	else if (pystring::equal(mode, "merge", true))
	{

	}

    return 0;
}
