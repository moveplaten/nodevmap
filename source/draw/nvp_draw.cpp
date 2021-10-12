#include "draw.h"
#include "base/base.h"

NvpDraw* const nvpDraw = nullptr;
NvpDrawPort* NvpDraw::g_draw_port = nullptr;

void NvpDraw::Record(BaseElement* base, NvpColor* cols,
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
        base->self_draw = draw;
        draw->setColor(cols);
        set_color = false;
    }

    if (base->self_draw && set_color && cols)
    {
        base->self_draw->setColor(cols);
    }

    if (opt == Draw)
    {
        g_draw_port->beginDraw();
    }
}

void NvpDraw::drawAll(BaseElement* base)
{
    if (!base)
    {
        return;
    }

    auto sub_level = base->self_layout->sub;
    
    if (base->self_draw)
    {
        base->self_draw->realDraw(base);
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
        drawAll(next->body.elem);
    }
}

void NvpFrameOneRect::realDraw(BaseElement* base)
{
    auto main_rect = base->getRectRefTop();
    g_draw_port->frameRect(*main_rect, color);
}

void NvpFrameFiveRect::realDraw(BaseElement* base)
{
    auto main_rect = base->getRectRefTop();

    auto min_width = (main_rect->right - main_rect->left) * percent;
    auto min_height = (main_rect->bottom - main_rect->top) * percent;

    BaseRect TL, TR, BL, BR;
    
    TL.left = main_rect->left;
    TL.top = main_rect->top;
    TL.right = TL.left + min_width;
    TL.bottom = TL.top + min_height;

    TR.right = main_rect->right;
    TR.top = main_rect->top;
    TR.left = TR.right - min_width;
    TR.bottom = TR.top + min_height;

    BL.left = main_rect->left;
    BL.bottom = main_rect->bottom;
    BL.right = BL.left + min_width;
    BL.top = BL.bottom - min_height;

    BR.right = main_rect->right;
    BR.bottom = main_rect->bottom;
    BR.left = BR.right - min_width;
    BR.top = BR.bottom - min_height;

    g_draw_port->frameRect(*main_rect, color[0]);
    g_draw_port->frameRect(TL, color[1]);
    g_draw_port->frameRect(TR, color[1]);
    g_draw_port->frameRect(BL, color[1]);
    g_draw_port->frameRect(BR, color[1]);
}

void NvpFillOneRect::realDraw(BaseElement* base)
{
    auto main_rect = base->getRectRefTop();
    g_draw_port->fillRect(*main_rect, color);
}

void NvpFillFiveRect::realDraw(BaseElement* base)
{
    auto main_rect = base->getRectRefTop();

    auto min_width = (main_rect->right - main_rect->left) * percent;
    auto min_height = (main_rect->bottom - main_rect->top) * percent;

    BaseRect TL, TR, BL, BR;
    
    TL.left = main_rect->left;
    TL.top = main_rect->top;
    TL.right = TL.left + min_width;
    TL.bottom = TL.top + min_height;

    TR.right = main_rect->right;
    TR.top = main_rect->top;
    TR.left = TR.right - min_width;
    TR.bottom = TR.top + min_height;

    BL.left = main_rect->left;
    BL.bottom = main_rect->bottom;
    BL.right = BL.left + min_width;
    BL.top = BL.bottom - min_height;

    BR.right = main_rect->right;
    BR.bottom = main_rect->bottom;
    BR.left = BR.right - min_width;
    BR.top = BR.bottom - min_height;

    g_draw_port->fillRect(*main_rect, color[0]);
    g_draw_port->fillRect(TL, color[1]);
    g_draw_port->fillRect(TR, color[1]);
    g_draw_port->fillRect(BL, color[1]);
    g_draw_port->fillRect(BR, color[1]);
}
