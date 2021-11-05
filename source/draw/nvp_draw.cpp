#include "draw.h"
#include "base/base.h"

NvpDraw* const nvpDraw = nullptr;
NvpDrawPort* NvpDraw::g_draw_port = nullptr;

void NvpDraw::Record(BaseElement* base, size_t offset, NvpColor* cols,
    RecordOption opt,
    const BaseRect* rect, NvpDraw* draw)
{
    if (!base)
    {
        return;
    }

    if (rect)
    {
        base->self_layout->ref_up = *rect; //ref_up is input from user;

        auto iter = base->self_level->begin();
        auto up_elem = (*iter)->head->up_elem;
        if (up_elem)
        {
            auto up_view_rect = up_elem->self_layout->ref_top;
            auto view_rect = &(base->self_layout->ref_top);

            auto width = rect->right - rect->left;
            auto height = rect->bottom - rect->top;

            view_rect->left = rect->left + up_view_rect.left;
            view_rect->top = rect->top + up_view_rect.top;
            view_rect->right = view_rect->left + width;
            view_rect->bottom = view_rect->top + height;
        }
        else if (base->getSelfName() == "top_layout")
        {
            base->self_layout->ref_top = *rect;
        }
    }

    bool set_color = true;

    if (draw && cols)
    {
        if (!base->self_draw)
        {
            base->self_draw = new NvpDrawVec;
        }
        
        base->self_draw->push_back(draw); //problem with same draw;
        draw->setColor(cols);
        set_color = false;
    }

    if (base->self_draw && set_color && cols)
    {
        (*(base->self_draw))[offset]->setColor(cols);
    }

    if (opt == Draw)
    {
        g_draw_port->beginDraw();
    }
}

NvpDraw* NvpDraw::getOneDraw(BaseElement* base, size_t offset)
{
    if (!base)
    {
        return nullptr;
    }

    return (*(base->self_draw))[offset];
}

void NvpDraw::drawAll(BaseElement* base)
{
    for (;;)
    {
        if (base)
        {
            if (base->self_draw)
            {
                auto draw_size = (*(base->self_draw)).size();

                for (size_t i = 0; i < draw_size; ++i)
                {
                    (*(base->self_draw))[i]->realDraw(base);
                }
            }
        }

        auto sub = nvpBuild->getSubFirst(base);

        if (sub)
        {
            drawAll(sub);
        }

        base = nvpBuild->getNext(base);

        if (!base)
        {
            break;
        }
    }
}
