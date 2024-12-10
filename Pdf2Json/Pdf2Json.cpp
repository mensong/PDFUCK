#include <iostream>
#include <fstream>
#include "../PDFUCK/PDFuck.h"
#include "../pystring/pystring.h"
#include "../jsoncppx/include/json/json.h"

std::string UnicodeToAnsi(const std::wstring& Unicode)
{
	std::string Result("");

	if (Unicode.empty())
	{
		return Result;
	}

	int nChar = WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, 0, 0, 0, 0);
	int nSize = nChar + 1;

	char* pchBuffer = new char[nSize];

	nChar = WideCharToMultiByte(CP_ACP, 0, Unicode.c_str(), -1, pchBuffer, nSize, 0, 0);
	if (nChar == 0)
	{
		delete[] pchBuffer;
		return Result;
	}

	pchBuffer[nChar] = 0;
	Result = pchBuffer;

	delete[] pchBuffer;

	return Result;
}

std::wstring AnsiToUnicode(const std::string& Ansi)
{
	std::wstring Result(L"");

	if (Ansi.empty())
	{
		return Result;
	}

	int nChar = MultiByteToWideChar(CP_ACP, 0, Ansi.c_str(), -1, NULL, 0);

	int nSize = nChar * sizeof(wchar_t) + 1;
	wchar_t* pwchBuffer = new wchar_t[nSize];

	nChar = MultiByteToWideChar(CP_ACP, 0, Ansi.c_str(), -1, pwchBuffer, nSize);
	if (nChar == 0)
	{
		delete[] pwchBuffer;
		return Result;
	}
	pwchBuffer[nChar] = 0;
	Result = pwchBuffer;

	delete[] pwchBuffer;

	return Result;
}

int main(int argc, char** argv)
{
	auto funcUsage = []()
	{
		std::cout << "Usage:"
			<< " -pdf=pdf file"
			<< " -out=json file[no set will print]"
			<< std::endl;
	};

	std::string pdfFile;
	std::string outFile;

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
		else if (pystring::equal(key, "out", true))
			outFile = val;
	}

	if (pystring::iscempty(pdfFile))
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

	Json::Value jArrText;

	int countPages = pdf->CountPages();
#define BUFF_SIZE 10240
	wchar_t text[BUFF_SIZE + 1];
	for (int i = 0; i < countPages; i++)
	{
		PDF_PAGE* page = pdf->OpenPage(i);
		if (!page)
			continue;
		auto textPage = page->OpenTextPage();
		if (textPage)
		{
			int writed = 0;
			int totalWrited = 0;
			std::string resText;
			do
			{
				writed = textPage->GetText(totalWrited, BUFF_SIZE, text);
				if (writed > 0)
				{
					resText += UnicodeToAnsi(text);
					totalWrited += writed;
				}
			} while (writed > 0);
			page->CloseTextPage(&textPage);

			Json::Value jTextObj;
			jTextObj["page_idx"] = i;
			jTextObj["text"] = resText;
			jArrText.append(jTextObj);
		}
		pdf->ClosePage(&page);
	}
	PDFuck::Ins().CloseDocument(&pdf);

	if (pystring::iscempty(outFile))
	{
		std::cout << Json::FastWriter().write(jArrText);
	}
	else
	{
		Json::FastWriter().writeFile(jArrText, outFile);
	}

	return 0;
}
