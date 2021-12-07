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

void NvpStyle::setColor(NvpColor colo)
{
    if (colo.Red == 0 && colo.Green == 0 && colo.Blue == 0)
    {
        colo.Alpha = 0; color = colo;
    }
    else
    {
        if (colo.Alpha == 0)
        {
            colo.Alpha = 200; //default global alpha value;
        }
        color = colo;
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

void NvpDrawCache::commandString(const NvpDrawData::Command command, char* const str)
{
    NvpDrawCache temp(NvpDrawData::Null_Data);
    temp.null_data.cmd = command;

    temp.OptSwitch(NvpDrawCache::NUL, nullptr, str);
}

void NvpDrawCache::OptSwitch(const Opt opt, const Param* const param, char* const str)
{
    auto command = null_data.getCmd();

    switch (command)
    {
    case NvpDrawData::One_Line:
    {
        NvpOptPush(&one_line, opt, param);
        if (str) { strcat(str, "One_Line"); }
    }
    break;

    case NvpDrawData::Rect_Same_Elem:
    {
        NvpOptPush(&rect_same_elem, opt, param);
        if (str) { strcat(str, "Rect_Same_Elem"); }
    }
    break;

    case NvpDrawData::Text_Left_Right:
    {
        NvpOptPush(&text_left_right, opt, param);
        if (str) { strcat(str, "Text_Left_Right"); }
    }
    break;

    case NvpDrawData::Four_Rect_Percent:
    {
        NvpOptPush(&four_rect_percent, opt, param);
        if (str) { strcat(str, "Four_Rect_Percent"); }
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

NvpColor NvpDrawCache::getColor() const
{
    return a_style.getColor();
}

void NvpDrawCache::setStyle(NvpStyle::Style style)
{
    a_style.setStyle(style);
}

NvpStyle::Style NvpDrawCache::getStyle() const
{
    return a_style.getStyle();
}

NvpDrawCache::NvpDrawCache(const NvpStyle& style, const NvpDrawData::Command command)
    :a_style(style), is_push(false)
{
    null_data.cmd = command;
    null_data.ptr = nullptr;
    OptByPush(NEW);
}

NvpDrawCache::NvpDrawCache(const NvpDrawData::Command command)
    :is_push(false)
{
    null_data.cmd = command;
    null_data.ptr = nullptr;
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
