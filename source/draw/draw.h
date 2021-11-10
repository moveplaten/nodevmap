#pragma once

#include <vector>
#include <string>
#include <cassert>

#include "base/layout.h"

struct BaseRect;

class BaseElement;

struct NvpColor
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
};

typedef mousePt NvpXyCoord;

class NvpDrawPort
{
public:
    //where define these functions
    //port-win/direct2d/d2d_main.cpp
    //port-osx/quartz/qtz_main.mm

    
    static void beginDraw();

    //draw all str from left to right just one line;
    //start from base rect left-top coord;
    static void drawTextFromLToR(NvpXyCoord start, const std::string& str, NvpColor colo);

    static void fillRect(const BaseRect& rect, NvpColor colo);
    static void frameRect(const BaseRect& rect, NvpColor colo); //only border;
    
};

class NvpStyle;
class NvpDrawCache;

enum NvpDrawCommand
{
    Draw_Null,
    
    Draw_Text_One_Line,
    Draw_Rect_Same_Elem,
    Draw_Four_Rect_Percent,

};

class NvpDrawReal
{
public:
    static void Draw_Null() {}

    static void Draw_One_Rect(const BaseRect& rect, const NvpStyle& style);

    static void Draw_Text_One_Line(const BaseElement& base, const NvpStyle& style,
        NvpXyCoord xy, const std::string& str);

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

    private:
        std::string text;
        NvpXyCoord start;

    public:
        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Text_One_Line(base, style, start, text);
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
    void NvpOptPush(T* const* t, const Opt opt, const bool is_push,
        const Param* const param, const NvpStyle& style)
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
            if (!is_push)
            {
                delete (*t);
            }
        }
        break;

        case DRAW:
        {
            (*t)->drawPrivate(param->base_elem, style); //from NvpDrawData;
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

class NvpDraw
{
public:
    static void drawAll(BaseElement* base);

    void pushDraw(NvpDrawCache& cache);

    NvpDrawCache* getDraw(size_t order); //order from 0;

    NvpDraw(BaseElement* base) : base_elem(base) {}

    ~NvpDraw();

private:
    void drawOneCache(const NvpDrawCache& cache, const BaseElement& base);
    
    BaseElement* base_elem;

    std::vector<NvpDrawCache> draw_cache;
};
