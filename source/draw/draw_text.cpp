#include "draw.h"
#include "base/base.h"

void NvpDrawTextOneLineOneColor::realDraw(BaseElement* base)
{
    auto main_rect = base->getRectRefTop();
    NvpXyCoord draw_pt;
    draw_pt.x = main_rect->left + start_pt.x;
    draw_pt.y = main_rect->top + start_pt.y;
    g_draw_port->drawTextFromLToR(draw_pt, str_to_draw, color);
}
