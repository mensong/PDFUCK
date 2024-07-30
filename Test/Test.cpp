// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../PDFUCK/PDFuck.h"
#include <fstream>
#include <vector>

void renderDiff(PDF_DOCUMENT* doc, PDF_PAGE* page, float left, float top, float right, float bottom)
{
	auto rectObj = doc->NewRectPageObject(0, 0, right - left, top - bottom);
	rectObj->SetFillColor(0, 255, 0, 50);
	rectObj->SetStrokeColor(255, 0, 0, 255);
	rectObj->Transform(1, 0, 0, 1, left, bottom);
	page->InsertPageObject(rectObj);
	doc->ClosePageObject(&rectObj);
}

void testCreate()
{
	auto doc = PDFuck::Ins().CreateDocument();
	doc->SetDefaultFontFilePath("simfang.ttf");

	float pageWidth = 800;
	float pageHeight = 600;
	auto page = doc->NewPage(doc->CountPages(), pageWidth, pageHeight);
	if (page)
	{
#if 0

		auto font = doc->LoadStandardFont("Microsoft YaHei");
		auto textObj = doc->NewTextPageObject(12, font);
		bool b = textObj->Text_SetText(L"我爱你abc");
		textObj->Transform(1, 0, 0, 1, 100, 100);
		page->InsertPageObject(textObj);
		doc->CloseFont(&font);
		doc->ClosePage(&page);

#elif 0
		std::ifstream fontFile("simfang.ttf", std::ios::out | std::ios::binary);
		if (fontFile.is_open())
		{
			fontFile.seekg(0, std::ios::end);
			std::streampos fileSize = fontFile.tellg();
			fontFile.seekg(0, std::ios::beg);
			char* fontBuff = new char[fileSize];
			fontFile.read(fontBuff, fileSize);
			fontFile.close();
			auto font = doc->LoadFontFromMemory((uint8_t*)fontBuff, fileSize, PDF_FONT::PDF_FONT_TYPE1, false);
			delete[] fontBuff;

			float fontSize = 12;

			const wchar_t* text = L"我123爱456";
			size_t textWidth = font->CalcStringWidth(text, fontSize);

			{
				auto textObj = doc->NewTextPageObject(font, fontSize);
				textObj->SetFillColor(255, 0, 0, 100);
				bool b = textObj->Text_SetText(text);
				textObj->Transform(1, 0, 0, 1, pageWidth - textWidth, pageHeight - fontSize);
				page->InsertPageObject(textObj);

				char fontName[256] = { 0 };
				textObj->Text_GetFontName(fontName, 256);

				doc->ClosePageObject(&textObj);				
			}
			{
				auto textObj = doc->NewTextPageObject(font, fontSize);
				textObj->SetFillColor(255, 0, 0, 255);
				float nc = textObj->Text_CalcCharWidth(font, L'我');
				bool b = textObj->Text_SetText(text);
				float x = pageWidth - textWidth - nc;
				float y = pageHeight - 2 * fontSize;
				textObj->Transform(1, 0, 0, 1, x, y);

				float left, bottom, right, top;
				textObj->GetBounds(&left, &bottom, &right, &top);
				float textWidth = right - left;
				float textHeight = top - bottom;

				page->InsertPageObject(textObj);
				doc->ClosePageObject(&textObj);

				//背景颜色
				auto rectObj = doc->NewRectPageObject(0, 0, textWidth, textHeight);
				rectObj->SetFillColor(0, 255, 0, 50);
				rectObj->SetStrokeColor(255, 0, 0, 255);
				rectObj->Transform(1, 0, 0, 1, x, y);
				page->InsertPageObject(rectObj);
				doc->ClosePageObject(&rectObj);
			}

			{
				auto pathObj = doc->NewPathPageObject();
				//pathObj->Path_SetDrawMode(PDF_PAGEOBJECT::PDF_FILLMODE_ALTERNATE, true);
				pathObj->Path_MoveTo(0, 0);
				pathObj->Path_LineTo(100, 0);
				pathObj->Path_LineTo(100, 100);
				pathObj->Path_LineTo(0, 100);
				pathObj->Path_SetClosed();
				pathObj->SetStrokeColor(0, 0, 255, 255);
				pathObj->SetStrokeWidth(3);
				pathObj->Transform(1, 0, 0, 1, 10, 20);
				page->InsertPageObject(pathObj);

				auto countSegs = pathObj->Path_CountSegments();
				for (int i = 0; i < countSegs; i++)
				{
					auto seg = pathObj->Path_OpenSegment(i);
					float x, y;
					seg->GetPoint(&x, &y);
					pathObj->Path_CloseSegment(&seg);
				}

				doc->ClosePageObject(&pathObj);
			}

			doc->CloseFont(&font);
			page->CommitChange();
			doc->ClosePage(&page);
		}
#else
		
		auto font = const_cast<PDF_FONT*>(doc->GetDefaultFont());
		char fontName[255];
		font->GetFontName(fontName, 255);

		float fontSize = 12;

		const wchar_t* text = L"我123爱456";
		size_t textWidth = font->CalcStringWidth(text, fontSize);

		{
			auto textObj = doc->NewTextPageObject(fontSize);
			textObj->SetFillColor(255, 0, 0, 100);
			bool b = textObj->Text_SetText(text);
			textObj->Transform(1, 0, 0, 1, pageWidth - textWidth, pageHeight - fontSize);
			page->InsertPageObject(textObj);

			char fontName[256] = { 0 };
			textObj->Text_GetFontName(fontName, 256);

			//复制
			auto textObjCopy = textObj->Clone(doc, page);
			textObjCopy->Transform(1, 0, 0, 1, (pageWidth - textWidth) / 2, (pageHeight - fontSize) / 2);
			page->InsertPageObject(textObjCopy);
			doc->ClosePageObject(&textObjCopy);

			doc->ClosePageObject(&textObj);
		}
		{
			auto textObj = doc->NewTextPageObject(fontSize);
			textObj->SetFillColor(255, 0, 0, 255);
			float nc = textObj->Text_CalcCharWidth(font, L'我');
			bool b = textObj->Text_SetText(text);
			float x = pageWidth - textWidth - nc;
			float y = pageHeight - 2 * fontSize;
			textObj->Transform(1, 0, 0, 1, x, y);

			float left, bottom, right, top;
			textObj->GetBounds(&left, &bottom, &right, &top);
			float textWidth = right - left;
			float textHeight = top - bottom;

			page->InsertPageObject(textObj);
			doc->ClosePageObject(&textObj);

			//背景颜色
			auto rectObj = doc->NewRectPageObject(0, 0, textWidth, textHeight);
			rectObj->SetFillColor(0, 255, 0, 50);
			rectObj->SetStrokeColor(255, 0, 0, 255);
			rectObj->Transform(1, 0, 0, 1, x, y);
			page->InsertPageObject(rectObj);
			doc->ClosePageObject(&rectObj);
		}

		{
			auto pathObj = doc->NewPathPageObject();
			//pathObj->Path_SetDrawMode(PDF_PAGEOBJECT::PDF_FILLMODE_ALTERNATE, true);
			pathObj->Path_MoveTo(0, 0);
			pathObj->Path_LineTo(100, 0);
			pathObj->Path_LineTo(100, 100);
			pathObj->Path_LineTo(0, 100);
			pathObj->Path_SetClosed();
			pathObj->SetStrokeColor(0, 0, 255, 255);
			pathObj->SetStrokeWidth(3);
			pathObj->Transform(1, 0, 0, 1, 10, 20);
			page->InsertPageObject(pathObj);

			auto countSegs = pathObj->Path_CountSegments();
			for (int i = 0; i < countSegs; i++)
			{
				auto seg = pathObj->Path_OpenSegment(i);
				float x, y;
				seg->GetPoint(&x, &y);
				pathObj->Path_CloseSegment(&seg);
			}

			doc->ClosePageObject(&pathObj);
		}

		page->CommitChange();
		doc->ClosePage(&page);
#endif
	}

	{
		int countPages = doc->CountPages();
		for (int i = 0; i < countPages; i++)
		{
			auto page = doc->OpenPage(i);
			int countPageObjs = page->CountPageObjects();
			for (int j = 0; j < countPageObjs; j++)
			{
				auto pageObj = page->OpenPageObject(j);

				if (pageObj->GetType() == PDF_PAGE_OBJECT_TYPE::PDF_PAGEOBJ_TEXT)
				{
					auto textPage = page->OpenTextPage();
					wchar_t text[255];
					pageObj->Text_GetText(textPage, text, 255);
					page->CloseTextPage(&textPage);
				}

				page->ClosePageObject(&pageObj);
			}

			doc->ClosePage(&page);
		}
	}

	{
		int countPages = doc->CountPages();
		for (int i = 0; i < countPages; i++)
		{
			auto page = doc->OpenPage(i);
			
			auto textPage = page->OpenTextPage();
			int countChars = textPage->CountChars();
			int countRects = textPage->CountRects();

			doc->ClosePage(&page);
		}
	}

	doc->SaveTo("1.pdf", doc->PDF_NO_INCREMENTAL);

	PDFuck::Ins().CloseDocument(&doc);

}

void testReadWriteText()
{	
	auto doc = PDFuck::Ins().LoadDocumentFromFile("shape1.pdf", NULL);

	int pagesCount = doc->CountPages();
	for (int i = 0; i < pagesCount; i++)
	{
		auto page = doc->OpenPage(i);

		auto textPage = page->OpenTextPage();

#if 0
		int objsCount = page->CountPageObjects();
		for (int p = 0; p < objsCount; p++)
		{
			auto obj = page->OpenPageObject(p);
			auto t = obj->GetType();
			switch (t)
			{
			case PDF_PAGEOBJ_TEXT:
			{
				wchar_t text[1024 + 1];
				//textPage->GetText(0, 100, text);
				auto readed = obj->Text_GetText(textPage, text, 1024);
				std::wcout << text << std::endl;
				float a, b, c, d, e, f;
				obj->Text_GetMatrix(&a, &b, &c, &d, &e, &f);
				//obj->SetStrokeColor(255, 0, 0, 255);
				obj->SetFillColor(255, 0, 0, 255);
				//unsigned int R, G, B, A;
				//obj->GetStrokeColor(&R, &G, &B, &A);
				
				page->CommitChange();

				//{
				//    auto pageObj = doc->NewRectPageObject(e, f, 10, 10);
				//    doc->DestroyNotYetManagedPageObject(pageObj);
				//    doc->ClosePageObject(&pageObj);
				//}

				int m = 0;
			}
			break;
			case PDF_PAGEOBJ_PATH:
				break;
			case PDF_PAGEOBJ_IMAGE:
				break;
			case PDF_PAGEOBJ_SHADING:
				break;
			case PDF_PAGEOBJ_FORM:
				break;
			default:
				break;
			}

			page->ClosePageObject(&obj);
		}
#else

		float left, right, bottom, top;
		int countChars = textPage->CountChars();
		for (int i = 0; i < countChars; i++)
		{
			wchar_t c = textPage->GetChar(i);

			if (textPage->GetCharBox(i, &left, &right, &bottom, &top))
			{
				int idx = textPage->GetCharIndexAtPos(left, bottom, 0, 0);
				if (idx != i)
					renderDiff(doc, page, left, top, right, bottom);
			}

			std::cout << c;
		}

		page->CommitChange();
#endif

		page->CloseTextPage(&textPage);
		doc->ClosePage(&page);

		doc->SaveTo("1_1.pdf", doc->PDF_NO_INCREMENTAL);
	}

	PDFuck::Ins().CloseDocument(&doc);
}

void generateComparePdf()
{
	auto doc = PDFuck::Ins().CreateDocument();

	float pageWidth = 800;
	float pageHeight = 600;
	auto page = doc->NewPage(doc->CountPages(), pageWidth, pageHeight);
	if (page)
	{
		std::ifstream fontFile("msyh.ttc", std::ios::out | std::ios::binary);
		if (fontFile.is_open())
		{
			fontFile.seekg(0, std::ios::end);
			std::streampos fileSize = fontFile.tellg();
			fontFile.seekg(0, std::ios::beg);
			char* fontBuff = new char[fileSize];
			fontFile.read(fontBuff, fileSize);
			auto font = doc->LoadFontFromMemory((uint8_t*)fontBuff, fileSize, PDF_FONT::PDF_FONT_TYPE1, true);
			delete[] fontBuff;

			float fontSize = 12;

			const wchar_t* text = L"我123爱456";
			float textWidth = font->CalcStringWidth(text, fontSize);

			{
				auto textObj = doc->NewTextPageObject(font, fontSize);
				textObj->SetFillColor(255, 0, 0, 100);
				bool b = textObj->Text_SetText(text);
				float x = (pageWidth - textWidth) / 2;
				float y = (pageHeight - fontSize) / 2;
				textObj->Transform(1, 0, 0, 1, x, y);
				page->InsertPageObject(textObj);
				doc->ClosePageObject(&textObj);
			}

			doc->CloseFont(&font);
			page->CommitChange();
			doc->ClosePage(&page);
		}
	}

	doc->SaveTo("a.pdf", doc->PDF_NO_INCREMENTAL);

	PDFuck::Ins().CloseDocument(&doc);
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

bool comparePathPageObject(PDF_PAGEOBJECT* pathObj1, PDF_PAGEOBJECT* pathObj2)
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

bool comparePagePaths(PDF_DOCUMENT* docRender, PDF_PAGE* pageRender, PDF_PAGE* pageOther)
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
		//只处理path
		if (pageObj->GetType() == PDF_PAGEOBJ_PATH)
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
		//只处理path
		if (pageObj->GetType() == PDF_PAGEOBJ_PATH)
		{
			if (pageObj->GetBounds(&left, &bottom, &right, &top))
			{
				float minPos[2] = { left , bottom };
				float maxPos[2] = { right , top };

				bool isPathDiff = true;
				int searchCount = rt->SearchPageObjectsByAABBBox(minPos, maxPos, pageSearchedInOther, 5);
				for (int se = 0; se < searchCount; se++)
				{
					if (comparePathPageObject(pageSearchedInOther[se], pageObj))
					{
						isPathDiff = false;
						break;
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

void testCompareLeftRight()
{
	//generateComparePdf();

	auto doc1 = PDFuck::Ins().LoadDocumentFromFile("shape1.pdf", NULL);
	auto doc2 = PDFuck::Ins().LoadDocumentFromFile("shape2.pdf", NULL);

	int pagesCount = min(doc1->CountPages(), doc2->CountPages());
	for (int i = 0; i < pagesCount; i++)
	{
		auto page1 = doc1->OpenPage(i);		
		auto page2 = doc2->OpenPage(i);
		
		bool page1TextHasDiff = comparePageText(doc1, page1, page2);
		bool page2TextHasDiff = comparePageText(doc2, page2, page1);

		bool page1PathHasDiff = comparePagePaths(doc1, page1, page2);
		bool page2PathHasDiff = comparePagePaths(doc2, page2, page1);

		if (page1TextHasDiff 
			|| page1PathHasDiff
			) 
			page1->CommitChange();
		if (page2TextHasDiff 
			|| page2PathHasDiff
			) 
			page2->CommitChange();
		doc1->ClosePage(&page1);		
		doc2->ClosePage(&page2);
	}

	doc1->SaveTo("left.pdf", PDF_DOCUMENT::PDF_NO_INCREMENTAL);
	doc2->SaveTo("right.pdf", PDF_DOCUMENT::PDF_NO_INCREMENTAL);

	PDFuck::Ins().CloseDocument(&doc1);
	PDFuck::Ins().CloseDocument(&doc2);
}

void testCompareOverride()
{
#if 1
	auto docMerge = PDFuck::Ins().LoadDocumentFromFile("shape1.pdf", NULL);
	auto doc2 = PDFuck::Ins().LoadDocumentFromFile("shape2.pdf", NULL);
#else
	auto doc2 = PDFuck::Ins().LoadDocumentFromFile("shape2.pdf", NULL);

	auto docMerge = PDFuck::Ins().CreateDocument();
	auto page2 = doc2->OpenPage(0);
	float width = page2->GetWidth();
	float height = page2->GetHeight();
	PDF_PAGE* defPage = docMerge->NewPage(0, width, height);
	defPage->SetRotation(page2->GetRotation());
	defPage->CommitChange();
	doc2->ClosePage(&page2);
#endif


	docMerge->SetDefaultFontFilePath("simfang.ttf");
	doc2->SetDefaultFontFilePath("simfang.ttf");

	int countPages1 = docMerge->CountPages();
	int countPages2 = doc2->CountPages();

	std::string pagerange = "1";	
	if (countPages2 > 1)
		pagerange += "-" + std::to_string(countPages2);
	if (!docMerge->ImportPagesFrom(doc2, pagerange.c_str(), countPages1))
		return;
	
	//auto expDoc = docMerge->ExportNPagesToOne(800, 600, 1, 2);	
	//expDoc->SaveTo("EXP.pdf", PDF_DOCUMENT::SAVE_FLAG::PDF_NO_INCREMENTAL);
	//PDFuck::Ins().CloseDocument(&expDoc);

	for (int i = 0; i < countPages1 && i < countPages2; i++)
	{
		auto page1 = docMerge->OpenPage(i);
		auto page2 = docMerge->OpenPage(i + countPages1);

		int countPageObj1 = page1->CountPageObjects();

		//把page2中的pageObject加到page1中
		int countPageObj2 = page2->CountPageObjects();
		for (int j = 0; j < countPageObj2; j++)
		{
			auto pageObj2 = page2->OpenPageObject(j);

			auto pageObjCopy = pageObj2->Clone(docMerge, page1);
			if (pageObjCopy)
			{
				float a, b, c, d, e, f;
				auto t = pageObj2->GetType();
				switch (t)
				{
				case PDF_PAGEOBJ_TEXT:
					pageObj2->Text_GetMatrix(&a, &b, &c, &d, &e, &f);
					pageObjCopy->Transform(a, b, c, d, e, f);
					break;
				case PDF_PAGEOBJ_PATH:
					pageObj2->Path_GetMatrix(&a, &b, &c, &d, &e, &f);
					pageObjCopy->Transform(a, b, c, d, e, f);
					break;
				case PDF_PAGEOBJ_IMAGE:
					pageObjCopy->Image_GetMatrix(&a, &b, &c, &d, &e, &f);
					pageObjCopy->Image_SetMatrix(a, b, c, d, e, f);
					break;
				case PDF_PAGEOBJ_SHADING:
					break;
				case PDF_PAGEOBJ_FORM:
					break;
				default:
					break;
				}
				

				page1->InsertPageObject(pageObjCopy);
				page1->ClosePageObject(&pageObjCopy);
			}
			page2->ClosePageObject(&pageObj2);
		}

		page1->CommitChange();
	}
	
	//删除导入的信息
	for (int i = 0; i < countPages2; i++)
	{
		docMerge->DeletePage(docMerge->CountPages() - 1);
	}

	docMerge->SaveTo("merge.pdf", PDF_DOCUMENT::PDF_NO_INCREMENTAL);

	PDFuck::Ins().CloseDocument(&docMerge);
	PDFuck::Ins().CloseDocument(&doc2);
}

void testRender()
{
	auto doc = PDFuck::Ins().LoadDocumentFromFile("123.pdf", NULL);

	int countPages = doc->CountPages();
	for (int i = 0; i < countPages; i++)
	{
		auto page = doc->OpenPage(i);

		{
			PDF_BITMAP* bitmap = doc->NewBitmap(800, 600);
			page->RenderToBitmap(bitmap, 0, 0, 800, 600, PDF_PAGE::PAGE_RATEION_0, 0);
			bitmap->WriteToFile("a.png");
			doc->CloseBitmap(&bitmap);
		}

		{
			PDF_BITMAP* bitmap = doc->NewBitmap(800, 600);
			page->RenderToBitmapEx(bitmap, 
				1, 0, 0, 1, 1, 1, 
				100, 100, 400, 400, 
				0);
			bitmap->WriteToFile("b.png");
			doc->CloseBitmap(&bitmap);
		}

		doc->ClosePage(&page);
		break;
	}

	PDFuck::Ins().CloseDocument(&doc);
}

void testBitmap()
{
	auto doc = PDFuck::Ins().CreateDocument();

	auto img = doc->NewBitmap(800, 600);
	for (int i = 0; i < 800/2; i++)
	{
		for (int j = 0; j < 600/2; j++)
		{
			img->SetPixel(i, j, 255, 0, 0, 100);
		}
	}
	img->WriteToFile("testBitmap.png");

	auto img1 = doc->NewBitmap(800, 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			uint8_t R, G, B, A;
			img->GetPixel(i, j, &R, &G, &B, &A);
			img1->SetPixel(i, j, R, G, B, A);
		}
	}
	img1->WriteToFile("testBitmap1.png");

	auto page = doc->NewPage(0, 800, 600);
	auto imgObj1 = doc->NewImagePageObject();
	imgObj1->Image_SetBitmap(img);
	imgObj1->Transform(200, 0, 0, 200, 120, 120);
	page->InsertPageObject(imgObj1);
	doc->ClosePageObject(&imgObj1);
	page->CommitChange();
	doc->ClosePage(&page);

	doc->CloseBitmap(&img);
	doc->CloseBitmap(&img1);

	doc->SaveTo("testBitmap.pdf", PDF_DOCUMENT::PDF_NO_INCREMENTAL);

	PDFuck::Ins().CloseDocument(&doc);
}

void testLoadImage()
{
	auto doc = PDFuck::Ins().CreateDocument();

	auto page = doc->NewPage(doc->CountPages(), 800, 600);

	auto img = doc->ReadImage("u.jpg");
	auto imgObj = doc->NewImagePageObject();
	imgObj->Image_SetBitmap(img);
	imgObj->Transform(200, 0, 0, 200, 120, 120);
	page->InsertPageObject(imgObj);

	doc->CloseBitmap(&img);

	page->CommitChange();
	doc->ClosePage(&page);

	doc->SaveTo("u.pdf", PDF_DOCUMENT::PDF_NO_INCREMENTAL);
	PDFuck::Ins().CloseDocument(&doc);
}

int main()
{
	std::wcout.imbue(std::locale("chs"));

	//testCreate();
	//testReadWriteText();
	//testCompareLeftRight();
	//testCompareOverride();
	//testRender();
	testBitmap();
	//testLoadImage();

	return 0;
}
