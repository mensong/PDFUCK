#include "pch.h"
#include "PDF_PAGEOBJECT_RTREE_imp.h"
#include "PDF_PAGE_imp.h"
#include "PDF_PAGEOBJECT_imp.h"

bool PDF_PAGEOBJECT_RTREE_imp::AppendPageObject(PDF_PAGEOBJECT* pageObject)
{
	float lbrt[4];//left, bottom, right, top
    if (!pageObject->GetBounds(&lbrt[0], &lbrt[1], &lbrt[2], &lbrt[3]))
        return false;
	
	return m_rtree.Insert(&lbrt[0], &lbrt[2], pageObject);
}

int PDF_PAGEOBJECT_RTREE_imp::SearchPageObjectsByAABBBox(float minPos[2], float maxPos[2],
    PDF_PAGEOBJECT** arrOutPageObjectBuffer, int bufferLen, float threshold/* = 0.01*/)
{
    std::set<PDF_PAGEOBJECT*> result;
    m_rtree.Search(minPos, maxPos, &result);

    float left, bottom, right, top;
    int i = 0;
	for (std::set<PDF_PAGEOBJECT*>::iterator it = result.begin();
        it != result.end() && i < bufferLen; ++it)
    {
        PDF_PAGEOBJECT* pageObj = *it;
        if (!pageObj->GetBounds(&left, &bottom, &right, &top))
            continue;

        if (fabs(left - minPos[0]) <= threshold &&
            fabs(bottom - minPos[1]) <= threshold &&
            fabs(right - maxPos[0]) <= threshold &&
            fabs(top - maxPos[1]) <= threshold)
        {
            arrOutPageObjectBuffer[i] = *it;
            ++i;
        }
    }
    return i;
}

