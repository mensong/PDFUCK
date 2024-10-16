// ExtractImagesFromPdf.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <set>
#include "../PDFUCK/PDFuck.h"
#include "../pystring/pystring.h"

int main(int argc, char** argv)
{
	std::string pdfFile;
	std::string outDir;
	std::string out_image_ext = "png";

	auto funcUsage = []()
	{
		std::cout << "Usage:"
			<< " -pdf=pdf file"
			<< " -out_dir=output dir"
			<< " -out_ext=png/jpg/bmp"
			<< std::endl;
	};

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
		else if (pystring::equal(key, "out_dir", true))
			outDir = val;
		else if (pystring::equal(key, "out_ext", true))
			out_image_ext = val;
	}

	if (pystring::iscempty(pdfFile) || pystring::iscempty(outDir))
	{
		funcUsage();
		return 1;
	}

	auto doc = PDFuck::Ins().LoadDocumentFromFile(pdfFile.c_str(), NULL);
	if (!doc)
	{
		std::cout << "打开文件错误:" << pdfFile << std::endl;
		return 2;
	}

	int countPages = doc->CountPages();
	for (int i = 0; i < countPages; i++)
	{
		auto page = doc->OpenPage(i);
		if (!page)
			continue;

		int imageIdx = 0;
		int countObjs = page->CountPageObjects();
		for (int j = 0; j < countObjs; j++)
		{
			auto obj = page->OpenPageObject(j);
			if (!obj)
				continue;
			if (obj->GetType() == PDF_PAGEOBJ_IMAGE)
			{
				++imageIdx;
				auto bmp = obj->Image_GetBitmap();
				if (bmp)
				{
					std::string outFileName = std::to_string(i + 1) + "-" + std::to_string(imageIdx) + "." + out_image_ext;
					outFileName = os_path::join(outDir, outFileName);
					bmp->WriteToFile(outFileName.c_str());
					doc->CloseBitmap(&bmp);
				}
			}
			page->ClosePageObject(&obj);
		}

		doc->ClosePage(&page);
	}

	return 0;
}

