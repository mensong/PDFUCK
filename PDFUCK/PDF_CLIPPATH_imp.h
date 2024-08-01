#pragma once
#include "PDFuck.h"
#include "fpdfview.h"

class PDF_CLIPPATH_imp 
    : public PDF_CLIPPATH
{
public:
    FPDF_CLIPPATH m_clipPath;

    PDF_CLIPPATH_imp()
        : m_clipPath(NULL)
    {

    }
    PDF_CLIPPATH_imp(FPDF_CLIPPATH clipPath)
        : m_clipPath(clipPath)
    {

    }
    ~PDF_CLIPPATH_imp()
    {
        if (m_clipPath)
        {

            m_clipPath = NULL;
        }
    }
            
    virtual int CountPaths() override;
            
    virtual int CountPathSegments(int path_index) override;

    virtual PDF_PATHSEGMENT* OpenPathSegment(int path_index, int segment_index) override;


    // Í¨¹ý PDF_CLIPPATH ¼Ì³Ð
    virtual void ClosePathSegment(PDF_PATHSEGMENT** pathSegment) override;

};

