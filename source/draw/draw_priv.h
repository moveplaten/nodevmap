#pragma once

#include "draw.h"

enum NvpDrawCommand
{
    Draw_Null,
    
    Draw_Text_One_Line,
    Draw_Rect_Same_Elem,
    Draw_Four_Rect_Percent,

};

class NvpStyle;

class NvpDrawReal
{
public:
    static void Draw_Null() {}

    static void Draw_One_Rect(const BaseRect& rect, const NvpStyle& style);

    static void Draw_Text_One_Line(const BaseElement& base, const NvpStyle& style,
        NvpXyCoord xy, const std::string& str, ptSize font_size);

    static void Draw_Rect_Same_Elem(const BaseElement& base, const NvpStyle& style);

    static void Draw_Four_Rect_Percent(const BaseElement& base, const NvpStyle& style,
        int percent);

};

namespace NvpDrawData
{
    ////////////////////////////////////////////////////////////////////////////

    #define NVP_DRAW_PRIVATE(V)                                     \
    public:                                                         \
    void drawPrivate(const BaseElement& base, const NvpStyle& style)\
    {                                                               \
        V                                                           \
    }                                                               \

    ////////////////////////////////////////////////////////////////////////////

    class NullData
    {

    };

    ////////////////////////////////////////////////////////////////////////////

    class RectSameElem
    {
        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Rect_Same_Elem(base, style);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class TextOneLine
    {
    public:
        void setText(const std::string& str) { text = str; }
        void setStart(NvpXyCoord xy) { start = xy; }

        const std::string& getText() { return text; }
        NvpXyCoord getStart() { return start; }

        ptSize getFontSize() { return font_size; }
        void setFontSize(ptSize size) { assert(size > 0); font_size = size; }

    private:
        std::string text = "?";
        NvpXyCoord start = { 0, 0 };
        ptSize font_size = 12.0f;

        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Text_One_Line(base, style, start, text, font_size);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class FourRectPercent
    {
    public:
        void setPercent(int per /*1~50*/) { percent = per; }
        int getPercent() { return percent; }

    private:
        int percent;

        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Four_Rect_Percent(base, style, percent);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

};

class NvpStyle
{
public:
    NvpStyle() : color{ 0, 0, 0, 0 }, style(Fill) {}

    void setColor(NvpColor colo);
    NvpColor getColor() const { return color; }

    enum Style : uint8_t
    {
        Fill,
        Frame,
    };
    
    void setStyle(Style styl) { style = styl; }
    Style getStyle() const { return style; }

    void colorBright();
    void colorDarker();

private:
    uint8_t bright(uint8_t RGB);
    uint8_t darker(uint8_t RGB);
    
    NvpColor color;
    Style style;
};

class NvpDrawCache
{
public:
    NvpDrawCache(const NvpStyle& style, const NvpDrawCommand command);
    NvpDrawCache(const NvpDrawCommand command);
    ~NvpDrawCache();

    void colorBright();
    void colorDarker();

    void setColor(NvpColor color);
    NvpColor getColor();

    void setStyle(NvpStyle::Style style);
    NvpStyle::Style getStyle();

    template<typename T, NvpDrawCommand D>
    class draw_safe
    {
    public:
        draw_safe(NvpDrawCommand c) : cmd(c), ptr(nullptr)
        {
            ptr = new T;
        }

        T* operator->() const //override ->;
        {
            int draw_cmd = D;
            assert(draw_cmd == this->cmd);
            return ptr;
        }

        ~draw_safe()
        {
            assert(ptr != nullptr);
        }

        T* getPtr() const { return ptr; }
        NvpDrawCommand getCmd() const { return cmd; }

    private:
        T* ptr;
        NvpDrawCommand cmd;

        friend class NvpDrawCache;
    };

    union
    {
        draw_safe<NvpDrawData::NullData, Draw_Null> null_data;
        draw_safe<NvpDrawData::RectSameElem, Draw_Rect_Same_Elem> const rect_same_elem;
        draw_safe<NvpDrawData::TextOneLine, Draw_Text_One_Line> const text_one_line;
        draw_safe<NvpDrawData::FourRectPercent, Draw_Four_Rect_Percent> const four_rect_percent;

    };

private:
    enum Opt { NEW, DELE, DRAW };
    
    void OptByPush(const Opt opt);

    struct Param
    {
        Param(const BaseElement& base) : base_elem(base) {};
        
        const BaseElement& base_elem;
    };

    void OptSwitch(const Opt opt, const Param* const param = nullptr);

    template<typename T>
    void NvpOptPush(const T* t, const Opt opt, const Param* const param)
    {
        switch (opt)
        {
        case NEW:
        {
            *(const_cast<T*>(t)) = T(this->null_data.cmd);
        }
        break;

        case DELE:
        {
            if (!this->is_push)
            {
                delete t->getPtr(); //from draw_safe;
            }
        }
        break;

        case DRAW:
        {
            t->getPtr()->drawPrivate(param->base_elem, this->a_style); //from NvpDrawData;
        }
        break;

        default:
            break;
        }
    }

    NvpStyle a_style;

    bool is_push;

    friend class NvpDraw;
};
