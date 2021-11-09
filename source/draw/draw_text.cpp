#include "draw.h"
#include "base/base.h"

void NvpDrawReal::Draw_Text_One_Line(const BaseElement& base, const NvpStyle& style,
    NvpXyCoord xy, const std::string& str)
{
    auto& main_rect = base.getRectRefTop();

    NvpXyCoord start;
    start.x = main_rect.left + xy.x;
    start.y = main_rect.top + xy.y;

    auto one_style = style.getStyle();

    switch (one_style)
    {
    case NvpStyle::Fill:
    {

    }
    //break;

    case NvpStyle::Frame:
    {

    }
    //break;

    case NvpStyle::Fill_Frame:
    {

    }
    //break;

    default:
    {
        NvpDrawPort::drawTextFromLToR(start, str, style.getColor());
    }
    break;
    }
}
