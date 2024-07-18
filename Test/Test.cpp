// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../PDFUCK/PDFuck.h"
#include <fstream>

void testCreate()
{
	auto doc = PDFuck::Ins().CreateDocument();

	auto page = doc->NewPage(doc->CountPages(), 800, 600);
	if (page)
	{
		//{
		//    auto font = doc->LoadStandardFont("TimesNewRoman-BoldItalic");
		//    auto textObj = doc->NewTextPageObject(font, 12);
		//    bool b = textObj->Text_SetText(L"我123爱456");
		//    page->InsertPageObject(textObj);
		//    doc->CloseFont(&font);
		//    doc->ClosePage(&page);
		//}

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

			int x = 10;
			{
				auto textObj = doc->NewTextPageObject(font, 12);
				textObj->SetFillColor(255, 0, 0, 100);
				//int nc = textObj->Text_CalcCharWidth(font, L'我');
				int nc = font->CalcCharWidth(L'我', 12);
				bool b = textObj->Text_SetText(L"我123爱456");
				textObj->Transform(1, 0, 0, 1, x, 20);
				x += nc;
				page->InsertPageObject(textObj);
				doc->ClosePageObject(&textObj);				
			}
			{
				auto textObj = doc->NewTextPageObject(font, 12);
				textObj->SetFillColor(255, 0, 0, 255);
				int nc = textObj->Text_CalcCharWidth(font, L'我');
				bool b = textObj->Text_SetText(L"我123爱456");
				textObj->Transform(1, 0, 0, 1, x, 40);
				page->InsertPageObject(textObj);
				doc->ClosePageObject(&textObj);
			}

			doc->CloseFont(&font);
			page->CommitChange();
			doc->ClosePage(&page);
		}
	}

	doc->SaveTo("1.pdf", doc->PDF_NO_INCREMENTAL);

	PDFuck::Ins().DestroyDocument(&doc);

}

void testReadWriteText()
{	
	auto doc = PDFuck::Ins().LoadDocumentFromFile("1.pdf", NULL);

	int pagesCount = doc->CountPages();
	for (int i = 0; i < pagesCount; i++)
	{
		auto page = doc->OpenPage(i);

		auto textPage = page->OpenTextPage();

#if 1
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

		int nRects = textPage->CountRects(0, 1000);
		for (int r = 0; r < nRects; r++)
		{
			double left, top, right, bottom;
			textPage->GetRect(r, &left, &top, &right, &bottom);
			std::wcout << left << top << right << bottom << std::endl;
		}

#endif

		page->CloseTextPage(&textPage);
		doc->ClosePage(&page);

		doc->SaveTo("1_1.pdf", doc->PDF_NO_INCREMENTAL);
	}

	PDFuck::Ins().DestroyDocument(&doc);
}

void testCompare()
{
	auto doc1 = PDFuck::Ins().LoadDocumentFromFile("1.pdf", NULL);
	auto doc2 = PDFuck::Ins().LoadDocumentFromFile("2.pdf", NULL);

	int pagesCount = doc1->CountPages();
	for (int i = 0; i < pagesCount; i++)
	{
		auto page = doc1->OpenPage(i);

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

		int nRects = textPage->CountRects(0, 1000);
		for (int r = 0; r < nRects; r++)
		{
			double left, top, right, bottom;
			textPage->GetRect(r, &left, &top, &right, &bottom);
			std::wcout << left << top << right << bottom << std::endl;
		}

#endif

		page->CloseTextPage(&textPage);
		doc1->ClosePage(&page);

		doc1->SaveTo("1_1.pdf", doc1->PDF_NO_INCREMENTAL);
	}

	PDFuck::Ins().DestroyDocument(&doc1);
}

int main()
{
	std::wcout.imbue(std::locale("chs"));

	PDFuck::Ins().GlobalInitializeLibrary(NULL);

	testCreate();
	//testReadWriteText();

	PDFuck::Ins().GlobalDestroyLibrary();
	return 0;
}
