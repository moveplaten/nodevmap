#include "draw.h"
#include "base/base.h"

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
