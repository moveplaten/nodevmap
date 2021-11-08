#include "draw.h"
#include "base/base.h"

void NvpDrawReal::Draw_One_Rect(const BaseRect& rect, const NvpStyle& style)
{
    auto one_style = style.getStyle();

    switch (one_style)
    {
    case NvpStyle::Fill:
    {
        NvpDrawPort::fillRect(rect, style.getColor());
    }
    break;

    case NvpStyle::Frame:
    {
        NvpDrawPort::frameRect(rect, style.getColor());
    }
    break;

    case NvpStyle::Fill_Frame:
    {
        NvpDrawPort::fillRect(rect, style.getColor());

        NvpDrawPort::frameRect(rect, style.getColor());
    }
    break;

    default:
        break;
    }
}

void NvpDrawReal::Draw_Rect_Same_Elem(const BaseElement& base, const NvpStyle& style)
{
    auto& rect = base.getRectRefTop();

    NvpDrawReal::Draw_One_Rect(rect, style);
}

void NvpDrawReal::Draw_Four_Rect_Percent(const BaseElement& base, const NvpStyle& style, int percent)
{
    auto& main_rect = base.getRectRefTop();

    float percent_f = percent / 100.0f;

    auto min_width = (main_rect.right - main_rect.left) * percent_f;
    auto min_height = (main_rect.bottom - main_rect.top) * percent_f;

    BaseRect TL, TR, BL, BR;

    TL.left = main_rect.left;
    TL.top = main_rect.top;
    TL.right = TL.left + min_width;
    TL.bottom = TL.top + min_height;

    TR.right = main_rect.right;
    TR.top = main_rect.top;
    TR.left = TR.right - min_width;
    TR.bottom = TR.top + min_height;

    BL.left = main_rect.left;
    BL.bottom = main_rect.bottom;
    BL.right = BL.left + min_width;
    BL.top = BL.bottom - min_height;

    BR.right = main_rect.right;
    BR.bottom = main_rect.bottom;
    BR.left = BR.right - min_width;
    BR.top = BR.bottom - min_height;

    NvpDrawReal::Draw_One_Rect(TL, style);
    NvpDrawReal::Draw_One_Rect(TR, style);
    NvpDrawReal::Draw_One_Rect(BL, style);
    NvpDrawReal::Draw_One_Rect(BR, style);
}
