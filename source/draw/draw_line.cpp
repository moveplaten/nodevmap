/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "draw_priv.h"
#include "base/base.h"

void NvpDrawReal::Draw_One_Line(const NvpBaseObj& base, const NvpStyle& style,
    NvpXyCoord p1, NvpXyCoord p2)
{
    auto main_rect = base.getRectRefTop();

    NvpXyCoord pt1, pt2;
    
    pt1.x = main_rect.left + p1.x;
    pt1.y = main_rect.top + p1.y;
    
    pt2.x = main_rect.left + p2.x;
    pt2.y = main_rect.top + p2.y;

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
        NvpDrawPort::drawOneLine(pt1, pt2, style.getColor());
    }
    break;
    }
}
