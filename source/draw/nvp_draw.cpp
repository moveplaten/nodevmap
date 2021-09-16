#include "draw.h"
#include "base/base.h"

void NvpDraw::Record(const BaseRect& rec, NvpColor col, RecordOption opt)
{
    if (opt == Clear)
    {
        recs.clear();
        cols.clear();
        record_offset = 0;
    }
    
    ++record_offset;
    auto rec_begin = recs.begin();
    auto col_begin = cols.begin();

    auto width = rec.right - rec.left;
    auto height = rec.bottom - rec.top;

    auto header = getLayoutHead(elem->getSelfLayout());
    auto up_elem = header->up_elem;
    auto up_rect = up_elem->getRect();

    BaseRect new_rect;

    //left_top system;
    new_rect.left = up_rect->left + rec.left;
    new_rect.top = up_rect->top + rec.top;

    auto depth = header->cur_depth;

    for (uint32_t i = 0; i < depth; ++i)
    {
        header = getLayoutHead(up_elem->getSelfLayout());
        if (header->up_elem)
        {
            up_elem = header->up_elem;
            up_rect = up_elem->getRect();
            new_rect.left += up_rect->left;
            new_rect.top += up_rect->top;
        }
    }

    new_rect.right = new_rect.left + width;
    new_rect.bottom = new_rect.top + height;

    for (int i = 0; i < record_offset - 1; ++i)
    {
        ++rec_begin;
        ++col_begin;
    }
    if (rec_begin == recs.end())
    {
        recs.push_back(new_rect);
        cols.push_back(col);
    }
    else
    {
        *rec_begin = new_rect;
        *col_begin = col;
    }

    if (opt == BeginEnd || opt == End)
    {
        realDraw();
        record_offset = 0;
    }
    
    if (opt == Clear)
    {
        record_offset = 0;
    }
}

void NvpDraw::realDraw()
{
    auto iter = g_top_layout->begin();
    auto top = *(++iter);
    auto top_elem = top->body.elem;
    /////////////////////////////////////////////////////////
    doBegin();

    subLevelDraw(top_elem);
    
    doEnd();
    /////////////////////////////////////////////////////////
}

void subLevelDraw(BaseElement* elem)
{
    if (!elem)
    {
        return;
    }

    auto sub_level = elem->getSelfLayout()->sub;
    auto draw = elem->getSelfLayout()->draw;
    if (draw)
    {
        draw->doDraw();
    }
    if (!sub_level)
    {
        return;
    }

    size_t size = sub_level->size();
    auto iter = sub_level->begin();

    for (size_t i = 0; i < size - 1; ++i)
    {
        auto next = *(++iter);
        subLevelDraw(next->body.elem);
    }
}
