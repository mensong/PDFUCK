
#include <iostream>
#include "../PDFUCK/PDFuck.h"
#include "../pystring/pystring.h"

int main(int argc, char** argv)
{
	auto funcUsage = []()
	{
		std::cout << "Usage:"
			<< " -pdf=pdf file"
			<< " -img=page image file"
			<< std::endl;
	};

	std::string pdfFile;
	std::string imgFile;

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
		std::string outFile = os_path::join(outDir, outFileName);
		if (i != 0)
			outFile += "-" + std::to_string(i);
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
