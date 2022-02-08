/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "draw_priv.h"
#include "base/base.h"

void NvpDrawReal::Draw_Text_Left_Right(const BaseElement& base, const NvpStyle& style,
    NvpXyCoord xy, const std::string& str, ptSize font_size)
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

    default:
    {
        NvpDrawPort::drawTextLeftRight(start, str, font_size, style.getColor());
    }
    break;
    }
}
