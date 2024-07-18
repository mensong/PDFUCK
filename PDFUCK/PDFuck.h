#pragma once
#ifndef _AFX
#include <windows.h>
#endif
#include <string>
#include <stdint.h>

#ifdef PDFUCK_EXPORTS
#define PDF_API extern "C" __declspec(dllexport)
#else
#define PDF_API extern "C" __declspec(dllimport)
#endif

class PDF_BITMAP;
class PDF_FONT;
class PDF_PAGEOBJECTMARK;
class PDF_PATHSEGMENT;
class PDF_FINDER;
class PDF_PAGELINK;
class PDF_TEXTPAGE;
class PDF_PAGEOBJECT;
class PDF_PAGE;
class PDF_DOCUMENT;

enum PDF_OBJECT_TYPE
{
	PDF_OBJECT_UNKNOWN = 0,
	PDF_OBJECT_BOOLEAN = 1,
	PDF_OBJECT_NUMBER = 2,
	PDF_OBJECT_STRING = 3,
	PDF_OBJECT_NAME = 4,
	PDF_OBJECT_ARRAY = 5,
	PDF_OBJECT_DICTIONARY = 6,
	PDF_OBJECT_STREAM = 7,
	PDF_OBJECT_NULLOBJ = 8,
	PDF_OBJECT_REFERENCE = 9,
};

enum PDF_PAGE_OBJECT_TYPE
{
	PDF_PAGEOBJ_UNKNOWN = 0,
	PDF_PAGEOBJ_TEXT = 1,
	PDF_PAGEOBJ_PATH = 2,
	PDF_PAGEOBJ_IMAGE = 3,
	PDF_PAGEOBJ_SHADING = 4,
	PDF_PAGEOBJ_FORM = 5,
};

enum PDF_COLORSPACE
{
	PDF_COLORSPACE_UNKNOWN = 0,
	PDF_COLORSPACE_DEVICEGRAY = 1,
	PDF_COLORSPACE_DEVICERGB = 2,
	PDF_COLORSPACE_DEVICECMYK = 3,
	PDF_COLORSPACE_CALGRAY = 4,
	PDF_COLORSPACE_CALRGB = 5,
	PDF_COLORSPACE_LAB = 6,
	PDF_COLORSPACE_ICCBASED = 7,
	PDF_COLORSPACE_SEPARATION = 8,
	PDF_COLORSPACE_DEVICEN = 9,
	PDF_COLORSPACE_INDEXED = 10,
	PDF_COLORSPACE_PATTERN = 11,
};

enum PDF_TEXT_RENDERMODE
{
	PDF_TEXTRENDERMODE_UNKNOWN = -1,
	PDF_TEXTRENDERMODE_FILL = 0,
	PDF_TEXTRENDERMODE_STROKE = 1,
	PDF_TEXTRENDERMODE_FILL_STROKE = 2,
	PDF_TEXTRENDERMODE_INVISIBLE = 3,
	PDF_TEXTRENDERMODE_FILL_CLIP = 4,
	PDF_TEXTRENDERMODE_STROKE_CLIP = 5,
	PDF_TEXTRENDERMODE_FILL_STROKE_CLIP = 6,
	PDF_TEXTRENDERMODE_CLIP = 7,
	PDF_TEXTRENDERMODE_LAST = PDF_TEXTRENDERMODE_CLIP,
};

class PDF_BITMAP
{
public:
	virtual int GetFormat() = 0;
	virtual void FillRect(
		int left,
		int top,
		int width,
		int height,
		unsigned long color) = 0;

	virtual void* GetBuffer() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetStride() = 0;
};

class PDF_PAGEOBJECTMARK
{
public:
	virtual bool GetName(
		wchar_t* buffer,
		unsigned long buflen,
		unsigned long* out_buflen) = 0;

	virtual int CountParams() = 0;
	virtual bool GetParamKey(
		unsigned long index,
		wchar_t* buffer,
		unsigned long buflen,
		unsigned long* out_buflen) = 0;		
	virtual PDF_OBJECT_TYPE GetParamValueType(const char* key) = 0;
	virtual bool GetParamIntValue(const char* key, int* out_value) = 0;
	virtual bool GetParamStringValue(const char* key, 
		void* buffer,
		unsigned long buflen,
		unsigned long* out_buflen) = 0;
	virtual bool GetParamBlobValue(const char* key, 
		void* buffer,
		unsigned long buflen,
		unsigned long* out_buflen) = 0;
	virtual bool SetIntParam(const char* key, int value) = 0;
	virtual bool SetStringParam(const char* key, const char* out_value) = 0;
	virtual bool SetBlobParam(const char* key, void* value, unsigned long value_len) = 0;
	virtual bool RemoveParam(const char* key) = 0;
};

class PDF_PATHSEGMENT
{
public:
	virtual bool GetPoint(float* x, float* y) = 0;

	enum PDF_SEGMENT_TYPE
	{
		PDF_SEGMENT_UNKNOWN = -1,
		PDF_SEGMENT_LINETO = 0,
		PDF_SEGMENT_BEZIERTO = 1,
		PDF_SEGMENT_MOVETO = 2,
	};
	virtual PDF_SEGMENT_TYPE GetType() = 0;
	virtual bool GetIsClose() = 0;
	

};

class PDF_FONT
{
public:
	enum FONT_TYPE
	{
		PDF_FONT_TYPE1 = 1,
		PDF_FONT_TRUETYPE = 2,
	};

public:
	virtual ~PDF_FONT()
	{
	}

	virtual float CalcCharWidth(wchar_t c, float fontSize) = 0;
};

class PDF_FINDER
{
public:
	virtual int GetCurrentResultCharIndex() = 0;
	virtual int GetCurrentResultCharCount() = 0;
	virtual bool FindNext() = 0;
	virtual bool FindPrev() = 0;
	
};

class PDF_PAGELINK
{
public:
	virtual int CountWebLinks() = 0;
	virtual int GetURL(int link_index, unsigned short* buffer, int buflen) = 0;
	virtual int CountRects(int link_index) = 0;
	virtual bool GetRect(int link_index, int rect_index, double* left, double* top, double* right, double* bottom) = 0;
	virtual bool GetTextRange(int link_index, int* start_char_index, int* char_count) = 0;

};

class PDF_TEXTPAGE
{
public:
	virtual int CountChars() = 0;
	virtual int GetCharIndexAtPos(double x, double y, double xTolerance, double yTolerance) = 0;
	virtual wchar_t GetChar(int index) = 0;
	virtual double GetCharFontSize(int index) = 0;
	virtual unsigned long GetCharFontInfo(int index, char* bufferFontNameUtf8, unsigned long buflen, int* flags) = 0;
	virtual int GetCharFontWeight(int index) = 0;
	virtual PDF_TEXT_RENDERMODE GetCharTextRenderMode(int index) = 0;
	virtual bool GetCharFillColor(int index, unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A) = 0;
	virtual bool GetCharStrokeColor(int index, unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A) = 0;
	virtual float GetCharAngle(int index) = 0;
	virtual bool GetCharBox(int index, double* left, double* right, double* bottom, double* top) = 0;
	virtual bool GetCharLooseCharBox(int index, float* left, float* right, float* bottom, float* top) = 0;
	virtual bool GetCharMatrix(int index, 
		float* a, float* b,
		float* c, float* d,
		float* e, float* f) = 0;
	virtual bool GetCharOrigin(int index, double* x, double* y) = 0;

	virtual int GetText(int start_index, int count, wchar_t* resultBuff) = 0;
	virtual int CountRects(int start_index, int count) = 0;
	virtual bool GetRect(int rect_index, double* left, double* top, double* right, double* bottom) = 0;
	virtual int GetTextByBoundary(
		double left, double top, double right, double bottom, 
		wchar_t* buffer, int buflen) = 0;

	enum FIND_FLAGS
	{
		// If not set, it will not match case by default.
		PDF_MATCHCASE = 0x00000001,
		// If not set, it will not match the whole word by default.
		PDF_MATCHWHOLEWORD = 0x00000002,
		// If not set, it will skip past the current match to look for the next match.
		PDF_CONSECUTIVE = 0x00000004,
	};
	virtual PDF_FINDER* FindStart(wchar_t* findwhat, unsigned long/*FIND_FLAGS*/ flags, int start_index) = 0;
	virtual void FindClose(PDF_FINDER** finder) = 0;

	virtual PDF_PAGELINK* OpenWebLinks() = 0;
	virtual void CloseWebLinks(PDF_PAGELINK** link_page) = 0;
};

class PDF_PAGEOBJECT
{
public:
	virtual PDF_PAGE_OBJECT_TYPE GetType() = 0;
	virtual bool HasTransparency() = 0;
	virtual void Transform(
		double a, double b,
		double c, double d,
		double e, double f) = 0;

	virtual bool GetBounds(float* left, float* bottom, float* right, float* top) = 0;

	enum BLEND_MODE
	{
		Color = 0,
		ColorBurn, 
		ColorDodge, 
		Darken, 
		Difference, 
		Exclusion, 
		HardLight, 
		Hue, 
		Lighten,
		Luminosity, 
		Multiply, 
		Normal, 
		Overlay, 
		Saturation, 
		Screen, 
		SoftLight,
	};
	virtual void SetBlendMode(BLEND_MODE bm) = 0;
	
	virtual bool SetStrokeColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A) = 0;
	virtual bool GetStrokeColor(unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A) = 0;
	virtual bool SetStrokeWidth(float width) = 0;
	virtual bool GetStrokeWidth(float* width) = 0;
	virtual bool SetFillColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A) = 0;
	virtual bool GetFillColor(unsigned int* R, unsigned int* G, unsigned int* B, unsigned int* A) = 0;

	enum PDF_LINEJOIN
	{
		PDF_LINEJOIN_UNKNOW = -1,
		PDF_LINEJOIN_MITER = 0,
		PDF_LINEJOIN_ROUND = 1,
		PDF_LINEJOIN_BEVEL = 2,
	};
	virtual PDF_LINEJOIN GetLineJoin() = 0;
	virtual bool SetLineJoin(PDF_LINEJOIN linejoin) = 0;

	enum PDF_LINECAP
	{
		PDF_LINECAP_BUTT = 0,
		PDF_LINECAP_ROUND = 1,
		PDF_LINECAP_PROJECTING_SQUARE = 2,
	};
	virtual PDF_LINECAP GetLineCap() = 0;
	virtual bool SetLineCap(PDF_LINECAP line_cap) = 0;

	virtual bool Image_SetBitmap(PDF_BITMAP* bitmap) = 0;
	virtual PDF_BITMAP* Image_GetBitmap() = 0;
	virtual PDF_BITMAP* Image_GetRenderedBitmap() = 0;
	virtual unsigned long Image_GetImageDataDecoded(void* buffer,	unsigned long buflen) = 0;
	virtual unsigned long Image_GetImageDataRaw(void* buffer, unsigned long buflen) = 0;
	virtual int Image_GetImageFilterCount() = 0;
	virtual unsigned long Image_GetImageFilter(int index, void* buffer, unsigned long buflen) = 0;
	virtual bool Image_GetImageMetadata(PDF_PAGE* page,
		unsigned int* width,
		unsigned int* height,
		float* horizontal_dpi,
		float* vertical_dpi,
		unsigned int* bits_per_pixel,
		PDF_COLORSPACE* colorspace,
		int* marked_content_id) = 0;
	virtual bool Image_GetMatrix(
		double* a, double* b,
		double* c, double* d,
		double* e, double* f) = 0;
	virtual bool Image_SetMatrix(
		double a, double b,
		double c, double d,
		double e, double f) = 0;

	virtual int Path_CountSegments() = 0;
	virtual PDF_PATHSEGMENT* Path_GetPathSegment(int index) = 0;

	virtual bool Path_MoveTo(float x, float y) = 0;
	virtual bool Path_LineTo(float x, float y) = 0;
	virtual bool Path_BezierTo(
		float x1, float y1,
		float x2, float y2,
		float x3, float y3) = 0;
	virtual bool Path_SetClose() = 0;
	enum PDF_FILLMODE
	{
		PDF_FILLMODE_NONE = 0,
		PDF_FILLMODE_ALTERNATE = 1,
		PDF_FILLMODE_WINDING = 2,
	};
	virtual bool Path_SetDrawMode(PDF_FILLMODE fillmode, bool stroke) = 0;
	virtual bool Path_GetDrawMode(PDF_FILLMODE* fillmode, bool* stroke) = 0;
	virtual bool Path_GetMatrix(
		float* a, float* b,
		float* c, float* d,
		float* e, float* f) = 0;
	virtual bool Path_SetMatrix(
		float a, float b,
		float c, float d,
		float e, float f) = 0;

	virtual bool Text_SetText(const wchar_t* text) = 0;
	virtual bool Text_GetMatrix(
		float* a, float* b,
		float* c, float* d,
		float* e, float* f) = 0;
	virtual float Text_GetFontSize() = 0;
	virtual PDF_TEXT_RENDERMODE Text_GetTextRenderMode() = 0;
	virtual bool Text_SetTextRenderMode(PDF_TEXT_RENDERMODE render_mode) = 0;
	virtual unsigned long Text_GetFontName(void* bufferUtf8, unsigned long length) = 0;
	virtual unsigned long Text_GetText(PDF_TEXTPAGE* text_page, wchar_t* buffer, unsigned long length) = 0;
	virtual float Text_CalcCharWidth(PDF_FONT* font, wchar_t c) = 0;

	virtual int Form_CountObjects() = 0;
	virtual PDF_PAGEOBJECT* Form_GetObject(unsigned long index) = 0;
	virtual bool Form_GetMatrix(
		float* a, float* b,
		float* c, float* d,
		float* e, float* f) = 0;

	virtual int CountMarks() = 0;
	virtual PDF_PAGEOBJECTMARK* GetMark(int index) = 0;
	virtual PDF_PAGEOBJECTMARK* AddMark(const char* tag) = 0;
	virtual bool RemoveMark(PDF_PAGEOBJECTMARK* mark) = 0;
};

class PDF_PAGE
{
public:
	virtual double GetWidth() = 0;
	virtual double GetHeight() = 0;

	virtual int GetRotation() = 0;
	virtual void SetRotation(int rotate) = 0;
	virtual bool HasTransparency() = 0;
	virtual bool GetBoundingBox(float* left, float* top, float* right, float* bottom) = 0;

	virtual void TransformAllAnnots(
		double a, double b,
		double c, double d,
		double e, double f) = 0;

	virtual void InsertPageObject(PDF_PAGEOBJECT* pageObj) = 0;
	virtual bool RemovePageObject(PDF_PAGEOBJECT* pageObj) = 0;
	virtual int CountPageObjects() = 0;
	virtual PDF_PAGEOBJECT* OpenPageObject(int idx) = 0;
	virtual void ClosePageObject(PDF_PAGEOBJECT** pageObj) = 0;

	virtual PDF_TEXTPAGE* OpenTextPage() = 0;
	virtual void CloseTextPage(PDF_TEXTPAGE** text_page) = 0;

	virtual bool CommitChange() = 0;

	enum RENDER_FLAGS
	{
		// Set if annotations are to be rendered.
		FPDF_ANNOT = 0x01,
		// Set if using text rendering optimized for LCD display. This flag will only
		// take effect if anti-aliasing is enabled for text.
		FPDF_LCD_TEXT = 0x02,
		// Don't use the native text output available on some platforms
		FPDF_NO_NATIVETEXT = 0x04,
		// Grayscale output.
		FPDF_GRAYSCALE = 0x08,
		// Obsolete, has no effect, retained for compatibility.
		FPDF_DEBUG_INFO = 0x80,
		// Obsolete, has no effect, retained for compatibility.
		FPDF_NO_CATCH = 0x100,
		// Limit image cache size.
		FPDF_RENDER_LIMITEDIMAGECACHE = 0x200,
		// Always use halftone for image stretching.
		FPDF_RENDER_FORCEHALFTONE = 0x400,
		// Render for printing.
		FPDF_PRINTING = 0x800,
		// Set to disable anti-aliasing on text. This flag will also disable LCD
		// optimization for text rendering.
		FPDF_RENDER_NO_SMOOTHTEXT = 0x1000,
		// Set to disable anti-aliasing on images.
		FPDF_RENDER_NO_SMOOTHIMAGE = 0x2000,
		// Set to disable anti-aliasing on paths.
		FPDF_RENDER_NO_SMOOTHPATH = 0x4000,
		// Set whether to render in a reverse Byte order, this flag is only used when
		// rendering to a bitmap.
		FPDF_REVERSE_BYTE_ORDER = 0x10,
		// Set whether fill paths need to be stroked. This flag is only used when
		// FPDF_COLORSCHEME is passed in, since with a single fill color for paths the
		// boundaries of adjacent fill paths are less visible.
		FPDF_CONVERT_FILL_TO_STROKE = 0x20,
	};

	virtual void RenderToDC(
		HDC dc,
		int start_x,
		int start_y,
		int size_x,
		int size_y,
		int rotate,
		int/*RENDER_FLAGS*/ flags) = 0;
	virtual void RenderToBitmap(
		PDF_BITMAP* bitmap,
		int start_x,
		int start_y,
		int size_x,
		int size_y,
		int rotate,
		int/*RENDER_FLAGS*/ flags) = 0;


};

class PDF_DOCUMENT
{
public:
	virtual bool GetFileVersion(int* outVer) = 0;
	virtual bool HasValidCrossReferenceTable() = 0;
	virtual unsigned long GetPermissions() = 0;
	virtual int GetSecurityHandlerRevision() = 0;

	virtual int CountPages() = 0;
	virtual PDF_PAGE* OpenPage(int pageIdx) = 0;
	virtual PDF_PAGE* NewPage(int page_index, double width, double height) = 0;
	virtual void ClosePage(PDF_PAGE** page) = 0;
	virtual void DeletePage(int pageIdx) = 0;

	virtual bool GetPageSizeByIndex(int page_index, double* width, double* height) = 0;

	virtual PDF_PAGEOBJECT* NewImagePageObject() = 0;
	virtual PDF_PAGEOBJECT* NewPathPageObject(float x, float y) = 0;
	virtual PDF_PAGEOBJECT* NewRectPageObject(float x, float y, float w, float h) = 0;
	virtual PDF_PAGEOBJECT* NewTextPageObject(const char* font_withoutspaces, float font_size) = 0;
	virtual PDF_PAGEOBJECT* NewTextPageObject(PDF_FONT* font, float font_size) = 0;
	virtual void DestroyNotYetManagedPageObject(PDF_PAGEOBJECT* pageObj) = 0;
	virtual void ClosePageObject(PDF_PAGEOBJECT** pageObj) = 0;

	virtual PDF_FONT* LoadFontFromMemory(const uint8_t* data, uint32_t size, 
		PDF_FONT::FONT_TYPE font_type, bool cid) = 0;
	virtual PDF_FONT* LoadStandardFont(const char* fontWithoutSpaces) = 0;
	virtual void CloseFont(PDF_FONT** font) = 0;

	enum SAVE_FLAG
	{
		PDF_INCREMENTAL = 1,
		PDF_NO_INCREMENTAL = 2,
		PDF_REMOVE_SECURITY = 3,
	};
	virtual bool SaveTo(const char* filePath, SAVE_FLAG flag) = 0;
};

//ÿ��dll�ڴ�ʵ��ֻ����һ��
PDF_API void GlobalInitializeLibrary(const char* fonts[]);
PDF_API void GlobalDestroyLibrary();

PDF_API PDF_DOCUMENT* CreateDocument();
PDF_API PDF_DOCUMENT* LoadDocumentFromFile(const char* file_path, const char* password);
PDF_API PDF_DOCUMENT* LoadDocumentFromMemory(const void* data_buf, size_t size, const char* password);
PDF_API void DestroyDocument(PDF_DOCUMENT** doc);


class PDFuck
{
#define DEF_PROC(name) \
	decltype(::name)* name

#define SET_PROC(hDll, name) \
	this->name = (decltype(::name)*)::GetProcAddress(hDll, #name)

public:
	DEF_PROC(GlobalInitializeLibrary);
	DEF_PROC(GlobalDestroyLibrary);
	DEF_PROC(CreateDocument);
	DEF_PROC(LoadDocumentFromFile);
	DEF_PROC(LoadDocumentFromMemory);
	DEF_PROC(DestroyDocument);

	PDFuck()
	{
		hDll = LoadLibraryFromCurrentDir("PDFUCK.dll");
		if (!hDll)
			return;

		SET_PROC(hDll, GlobalInitializeLibrary);
		SET_PROC(hDll, GlobalDestroyLibrary);
		SET_PROC(hDll, CreateDocument);
		SET_PROC(hDll, LoadDocumentFromFile);
		SET_PROC(hDll, LoadDocumentFromMemory);
		SET_PROC(hDll, DestroyDocument);
	}


public:
	inline static PDFuck& Ins()
	{
		static PDFuck s_ins;
		return s_ins;
	}

	static HMODULE LoadLibraryFromCurrentDir(const char* dllName)
	{
		char selfPath[MAX_PATH];
		MEMORY_BASIC_INFORMATION mbi;
		HMODULE hModule = ((::VirtualQuery(
			LoadLibraryFromCurrentDir, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
		::GetModuleFileNameA(hModule, selfPath, MAX_PATH);
		std::string moduleDir(selfPath);
		size_t idx = moduleDir.find_last_of('\\');
		moduleDir = moduleDir.substr(0, idx);
		std::string modulePath = moduleDir + "\\" + dllName;
		char curDir[MAX_PATH];
		::GetCurrentDirectoryA(MAX_PATH, curDir);
		::SetCurrentDirectoryA(moduleDir.c_str());
		HMODULE hDll = LoadLibraryA(modulePath.c_str());
		::SetCurrentDirectoryA(curDir);
		if (!hDll)
		{
			DWORD err = ::GetLastError();
			char buf[10];
			sprintf_s(buf, "%u", err);
			::MessageBoxA(NULL, ("�Ҳ���" + modulePath + "ģ��:" + buf).c_str(), 
				"�Ҳ���ģ��", MB_OK | MB_ICONERROR);
		}
		return hDll;
	}
	~PDFuck()
	{
		if (hDll)
		{
			FreeLibrary(hDll);
			hDll = NULL;
		}
	}

private:
	HMODULE hDll;
};
