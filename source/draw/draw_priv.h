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
    void drawPrivate(const BaseElement& base, const NvpStyle& style)\
    {                                                               \
        V                                                           \
    }                                                               \

    typedef NvpDrawCommand Command;

    template<Command D> class DrawSafe
    {
    public:
        void valid(Command cmd)
        {
            assert(cmd == command);
        }
    private:
        Command command = D;
    };

    ////////////////////////////////////////////////////////////////////////////

    template<Command D = Draw_Null> class NullData : DrawSafe<Draw_Null>
    {

    };

    ////////////////////////////////////////////////////////////////////////////

    template<Command D = Draw_Rect_Same_Elem> class RectSameElem :
        DrawSafe<Draw_Rect_Same_Elem>
    {
    public:
        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Rect_Same_Elem(base, style);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    template<Command D = Draw_Text_One_Line> class TextOneLine :
        DrawSafe<Draw_Text_One_Line>
    {
    public:
        void setText(const std::string& str) { valid(D); text = str; }
        void setStart(NvpXyCoord xy) { valid(D); start = xy; }

        const std::string& getText() { valid(D); return text; }
        NvpXyCoord getStart() { valid(D); return start; }

        ptSize getFontSize() { valid(D); return font_size; }
        void setFontSize(ptSize size) { valid(D); assert(size > 0); font_size = size; }

    private:
        std::string text = "?";
        NvpXyCoord start = { 0, 0 };
        ptSize font_size = 12.0f;

    public:
        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Text_One_Line(base, style, start, text, font_size);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    template<Command D = Draw_Four_Rect_Percent> class FourRectPercent :
        DrawSafe<Draw_Four_Rect_Percent>
    {
    public:
        void setPercent(int per /*1~50*/) { valid(D); percent = per; }
        int getPercent() { valid(D); return percent; }

    private:
        int percent;

    public:
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

    void setColor(NvpColor colo) { color = colo; }
    NvpColor getColor() const { return color; }

    enum Style
    {
        Fill,
        Frame,
        Fill_Frame,
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

    union
    {
        NvpDrawData::NullData<>* const null_data;
        NvpDrawData::RectSameElem<>* const rect_same_elem;
        NvpDrawData::TextOneLine<>* const text_one_line;
        NvpDrawData::FourRectPercent<>* const four_rect_percent;

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
    void NvpOptPush(T* const* t, const Opt opt, const Param* const param)
    {
        switch (opt)
        {
        case NEW:
        {
            T** t_ = const_cast<T**>(t);

            (*t_) = new T;
        }
        break;

        case DELE:
        {
            if (!this->is_push)
            {
                delete (*t);
            }
        }
        break;

        case DRAW:
        {
            (*t)->drawPrivate(param->base_elem, this->a_style); //from NvpDrawData;
        }
        break;

        default:
            break;
        }
    }

    NvpStyle a_style;

    const NvpDrawCommand a_command;

    bool is_push;

    friend class NvpDraw;
};
