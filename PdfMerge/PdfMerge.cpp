// PdfMerge.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <string>
#include "../PDFUCK/PDFuck.h"
#include "../pystring/pystring.h"

int main(int argc, char** argv)
{
	auto funcUsage = []()
	{
		std::cout << "Usage:"
			<< " -out=pdf_file"
			<< " pdf1 pdf2 pdf3 ..."
			<< std::endl;
	};

	std::string outPdfFile;
	std::vector<std::string> inPdfFiles;

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
		
		if (pystring::equal(key, "out", true))
			outPdfFile = val;
		else if (pystring::iscempty(key))
			inPdfFiles.push_back(val);
	}

	if (pystring::iscempty(outPdfFile) || inPdfFiles.size() == 0)
	{
		funcUsage();
		return 1;
	}

	auto outPdf = PDFuck::Ins().CreateDocument();
	if (!outPdf)
	{
		std::cout << "创建目标pdf失败" << std::endl;
		return 1;
	}
	for (size_t i = 0; i < inPdfFiles.size(); i++)
	{
		auto inPdf = PDFuck::Ins().LoadDocumentFromFile(inPdfFiles[i].c_str(), NULL);
		if (!inPdf)
		{
			std::cout << inPdfFiles[i] << " 打开失败，将跳过" << std::endl;
			continue;
		}
		outPdf->ImportPagesFrom(inPdf, NULL, outPdf->CountPages());

		PDFuck::Ins().CloseDocument(&inPdf);
	}

	bool b = outPdf->SaveTo(outPdfFile.c_str(), PDF_DOCUMENT::PDF_INCREMENTAL);
	PDFuck::Ins().CloseDocument(&outPdf);

	if (b)
		std::cout << "合并成功" << std::endl;
	else
		std::cout << "合并失败" << std::endl;

	return (b ? 0 : 1);
}
