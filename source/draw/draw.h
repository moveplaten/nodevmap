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

    static void Draw_Text_One_Line(const BaseElement& base, NvpXyCoord xy,
        const std::string& str, const NvpStyle& style);

    static void Draw_Rect_Same_Elem(const BaseElement& base, const NvpStyle& style);

    static void Draw_Four_Rect_Percent(const BaseElement& base, const NvpStyle& style, int percent);

};

namespace NvpDrawData
{
    ////////////////////////////////////////////////////////////////////////////

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
    NvpDrawCache(const NvpStyle& style, NvpDrawCommand command);
    NvpDrawCache(NvpDrawCommand command);
    ~NvpDrawCache();

    void colorBright();
    void colorDarker();

    void setColor(NvpColor color);
    NvpColor getColor();

    void setStyle(NvpStyle::Style style);
    NvpStyle::Style getStyle();

    union
    {
        NvpDrawData::NullData<>* null_data;
        NvpDrawData::TextOneLine<>* text_one_line;
        NvpDrawData::FourRectPercent<>* four_rect_percent;

    };

private:
    enum Opt { NEW, DEL };
    
    void OptByPush(Opt opt);

    NvpStyle a_style;

    NvpDrawCommand a_command;

    bool is_push = false;

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
