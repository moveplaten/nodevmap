#include "draw_priv.h"
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

        auto sub = NvpLayout::getSubFirst(base);

        if (sub)
        {
            drawAll(sub);
        }

        base = NvpLayout::getNext(base);

        if (!base)
        {
            break;
        }
    }
}

void NvpDraw::drawOneCache(const NvpDrawCache& cache, const BaseElement& base)
{
    NvpDrawCache::Param param(base);
    
    const_cast<NvpDrawCache&>(cache).OptSwitch(NvpDrawCache::DRAW, &param);
}

void NvpDrawCache::OptSwitch(const Opt opt, const Param* const param)
{
    auto command = this->a_command;

    switch (command)
    {
    case Draw_Rect_Same_Elem:
    {
        NvpOptPush<NvpDrawData::RectSameElem<>>(&rect_same_elem, opt, param);
    }
    break;

    case Draw_Text_One_Line:
    {
        NvpOptPush<NvpDrawData::TextOneLine<>>(&text_one_line, opt, param);
    }
    break;

    case Draw_Four_Rect_Percent:
    {
        NvpOptPush<NvpDrawData::FourRectPercent<>>(&four_rect_percent, opt, param);
    }
    break;

    default:
        break;
    }
}

void NvpDrawCache::OptByPush(const Opt opt)
{
    OptSwitch(opt);
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
    :a_style(style), a_command(command), is_push(false)
{
    OptByPush(NEW);
}

NvpDrawCache::NvpDrawCache(NvpDrawCommand command)
    :a_command(command), is_push(false)
{
    OptByPush(NEW);
}

NvpDrawCache::~NvpDrawCache()
{
    OptByPush(DELE);
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

NvpDraw::NvpDraw(BaseElement* base) : base_elem(base)
{
    
}

NvpDraw::~NvpDraw()
{
    auto size = draw_cache.size();

    for (size_t i = 0; i < size; ++i)
    {
        draw_cache[i].is_push = false;
    }
}
