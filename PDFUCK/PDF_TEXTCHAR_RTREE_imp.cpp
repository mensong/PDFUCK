#include "pch.h"
#include "PDF_TEXTCHAR_RTREE_imp.h"

bool PDF_TEXTCHAR_RTREE_imp::AppendTextPageAllChars()
{
    int countChars = m_textPage->CountChars();
    for (int i = 0; i < countChars; i++)
    {
        AppendCharIndex(i);
    }
    return true;
}


bool PDF_TEXTCHAR_RTREE_imp::AppendCharIndex(int charIndex)
{
    float lbrt[4];//left, bottom, right, top
    if (!m_textPage->GetCharBox(charIndex, &lbrt[0], &lbrt[2], &lbrt[1], &lbrt[3]))
		return false;
    return m_rtree.Insert(&lbrt[0], &lbrt[2], charIndex);
}

int PDF_TEXTCHAR_RTREE_imp::SearchCharIndexByAABBBox(float minPos[2], float maxPos[2], int* arrOutIndexBuffer, int bufferLen, float threshold)
{
    std::set<int> result;
    m_rtree.Search(minPos, maxPos, &result);

    float left, right, bottom, top;
    int i = 0;
    for (std::set<int>::iterator it = result.begin();
        it != result.end() && i < bufferLen; ++it)
    {
        int idx = *it;
        if (!m_textPage->GetCharBox(idx, &left, &right, &bottom, &top))
            continue;

        if (fabs(left - minPos[0]) <= threshold &&
            fabs(bottom - minPos[1]) <= threshold &&
            fabs(right - maxPos[0]) <= threshold &&
            fabs(top - maxPos[1]) <= threshold)
        {
            arrOutIndexBuffer[i] = *it;
            ++i;
        }
    }
    return i;
}

