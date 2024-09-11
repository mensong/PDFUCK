
#include <iostream>
#include <set>
#include "../PDFUCK/PDFuck.h"
#include "../pystring/pystring.h"

int main(int argc, char** argv)
{
	auto funcUsage = []()
	{
		std::cout << "Usage:"
			<< " -pdf=pdf file"
			<< " -img=page image file"
			<< " -page=1,3-10"
			<< std::endl;
	};

	std::string pdfFile;
	std::string imgFile;
	std::set<int> pages;

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

		if (pystring::equal(key, "pdf", true))
			pdfFile = val;
		else if (pystring::equal(key, "img", true))
			imgFile = val;
		else if (pystring::equal(key, "page", true))
		{
			std::vector<std::string> sp;
			pystring::split(val, sp, ",");
			for (size_t i = 0; i < sp.size(); i++)
			{
				std::string s = pystring::strip(sp[i]);
				if (pystring::iscempty(s))
					continue;
				size_t idxG = s.find('-');
				if (idxG != std::string::npos)
				{
					std::string startPage = s.substr(0, idxG);
					std::string endPage = s.substr(idxG + 1);
					int start = atoi(startPage.c_str());
					int end = atoi(endPage.c_str());
					if (start == 0 || end == 0)
						continue;
					for (int p = start; p <= end; p++)
					{
						pages.insert(p);
					}
				}
				else
				{
					int pageIdx = atoi(s.c_str());
					if (pageIdx == 0)
						continue;
					pages.insert(pageIdx);
				}

			}
		}
	}

	if (pystring::iscempty(pdfFile) || pystring::iscempty(imgFile))
	{
		funcUsage();
		return 1;
	}

	auto pdf = PDFuck::Ins().LoadDocumentFromFile(pdfFile.c_str(), NULL);
	if (!pdf)
	{
		std::cout << "打开文件错误:" << pdfFile << std::endl;
		return 2;
	}

	std::string outDir = os_path::dirname(imgFile);
	std::string outFileName = os_path::basename_no_ext(imgFile);
	std::string outFileExt = os_path::extension(imgFile);

	int countPages = pdf->CountPages();
	for (int i = 0; i < countPages; i++)
	{
		if (pages.size() != 0 && pages.find(i + 1) == pages.end())
			continue;

		std::string outFile = os_path::join(outDir, outFileName);
		if (i != 0)
			outFile += "-" + std::to_string(i + 1);
		outFile += "." + outFileExt;

		PDF_PAGE* page = pdf->OpenPage(i);
		if (!page)
			continue;
		PDF_BITMAP* pBmp = pdf->NewBitmap(page->GetWidth(), page->GetHeight());
		if (!pystring::equal(outFileExt, "png", true))
			pBmp->ClearWidthColor(255, 255, 255, 255);
		page->RenderToBitmap(pBmp, 0, 0, page->GetWidth(), page->GetHeight(), PDF_PAGE::PAGE_RATEION_0);
		pBmp->WriteToFile(outFile.c_str());
		pdf->CloseBitmap(&pBmp);
		pdf->ClosePage(&page);
	}
	PDFuck::Ins().CloseDocument(&pdf);

	return 0;
}
