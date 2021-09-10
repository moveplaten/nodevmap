#include "draw.h"
#include "base/base.h"

void NvpDraw::Record(const BaseRect& rec, NvpColor col, RecordOption opt)
{
    ++record_offset;
    auto rec_begin = recs.begin();
    auto col_begin = cols.begin();

    for (int i = 0; i < record_offset - 1; ++i)
    {
        ++rec_begin;
        ++col_begin;
    }
    if (rec_begin == recs.end())
    {
        recs.push_back(rec);
        cols.push_back(col);
    }
    else
    {
        *rec_begin = rec;
        *col_begin = col;
    }

    if (opt == BeginEnd || opt == End)
    {
        realDraw();
        record_offset = 0;
    }
}

void NvpDraw::realDraw()
{
    auto size = g_all_elem_map->size();
    auto content = *g_all_elem_map;
    auto it = content.begin();

    /////////////////////////////////////////////////////////
    doBegin();
    for (size_t i = 0; i < size; ++i)
    {
        auto elem = (*it).second;
        auto dra = elem->getSelfLayout()->draw;
        if (dra)
        {
            dra->doDraw();
        }
        ++it;
    }
    doEnd();
    /////////////////////////////////////////////////////////
}
