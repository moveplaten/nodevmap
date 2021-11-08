#include "draw.h"
#include "base/base.h"

void NvpStyle::colorBright()
{
    color.Red = bright(color.Red);
    color.Green = bright(color.Green);
    color.Blue = bright(color.Blue);
}

void NvpStyle::colorDarker()
{
    color.Red = darker(color.Red);
    color.Green = darker(color.Green);
    color.Blue = darker(color.Blue);
}

uint8_t NvpStyle::bright(uint8_t RGB)
{
    int value = 50;
    
    if (RGB + value > 255)
    {
        return 255;
    }
    else
    {
        return RGB + value;
    }
}

uint8_t NvpStyle::darker(uint8_t RGB)
{
    int value = 50;
    
    if (RGB - value < 0)
    {
        return 0;
    }
    else
    {
        return RGB - value;
    }
}

void NvpDraw::drawAll(BaseElement* base)
{
    for (;;)
    {
        if (base)
        {
            auto draw = base->getSelfDraw();

            if (draw)
            {
                auto draw_size = draw->draw_cache.size();

                for (size_t i = 0; i < draw_size; ++i)
                {
                    draw->drawOneCache(draw->draw_cache[i], *base);
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

void NvpDraw::drawOneCache(const NvpDrawCache& cache, const BaseElement& base)
{
    auto command = cache.a_command;

    switch (command)
    {
    case Draw_Rect_Same_Elem:
    {
        NvpDrawReal::Draw_Rect_Same_Elem(base, cache.a_style);
    }
    break;

    case Draw_Text_One_Line:
    {
        auto xy = cache.text_one_line->getStart();
        auto text = cache.text_one_line->getText();
        NvpDrawReal::Draw_Text_One_Line(base, xy, text, cache.a_style);
    }
    break;

    case Draw_Four_Rect_Percent:
    {
        auto percent = cache.four_rect_percent->getPercent();
        NvpDrawReal::Draw_Four_Rect_Percent(base, cache.a_style, percent);
    }
    break;

    default:
        break;
    }
}

void NvpDrawCache::OptByPush(Opt opt)
{
    auto command = a_command;
    
    switch (command)
    {
    case Draw_Text_One_Line:
    {
        if (opt == DEL && !is_push)
        {
            delete text_one_line;
        }
        else if (opt == NEW)
        {
            text_one_line = new NvpDrawData::TextOneLine<>;
        }
    }
    break;

    case Draw_Four_Rect_Percent:
    {
        if (opt == DEL && !is_push)
        {
            delete four_rect_percent;
        }
        else if (opt == NEW)
        {
            four_rect_percent = new NvpDrawData::FourRectPercent<>;
        }
    }
    break;

    default:
        break;
    }
}

void NvpDrawCache::colorBright()
{
    a_style.colorBright();
}

void NvpDrawCache::colorDarker()
{
    a_style.colorDarker();
}

void NvpDrawCache::setColor(NvpColor color)
{
    a_style.setColor(color);
}

NvpColor NvpDrawCache::getColor()
{
    return a_style.getColor();
}

void NvpDrawCache::setStyle(NvpStyle::Style style)
{
    a_style.setStyle(style);
}

NvpStyle::Style NvpDrawCache::getStyle()
{
    return a_style.getStyle();
}

NvpDrawCache::NvpDrawCache(const NvpStyle& style, NvpDrawCommand command)
    :a_style(style), a_command(command)
{
    OptByPush(NEW);
}

NvpDrawCache::NvpDrawCache(NvpDrawCommand command)
    :a_command(command)
{
    OptByPush(NEW);
}

NvpDrawCache::~NvpDrawCache()
{
    OptByPush(DEL);
}

void NvpDraw::pushDraw(NvpDrawCache& cache)
{
    cache.is_push = true;
    draw_cache.push_back(cache);
}

NvpDrawCache* NvpDraw::getDraw(size_t order)
{
    return &draw_cache[order];
}

NvpDraw::~NvpDraw()
{
    auto size = draw_cache.size();

    for (size_t i = 0; i < size; ++i)
    {
        draw_cache[i].is_push = false;
    }
}
