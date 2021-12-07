#pragma once

#include "draw.h"

class NvpStyle;

class NvpDrawReal
{
public:
    static void Draw_Null() {}

    static void Draw_One_Rect(const BaseRect& rect, const NvpStyle& style);

    static void Draw_Text_Left_Right(const BaseElement& base, const NvpStyle& style,
        NvpXyCoord xy, const std::string& str, ptSize font_size);

    static void Draw_One_Line(const BaseElement& base, const NvpStyle& style,
        NvpXyCoord p1, NvpXyCoord p2);

    static void Draw_Rect_Same_Elem(const BaseElement& base, const NvpStyle& style);

    static void Draw_Four_Rect_Percent(const BaseElement& base, const NvpStyle& style,
        int percent);

};

namespace NvpDrawData
{
    ////////////////////////////////////////////////////////////////////////////
    
    enum Command : uint8_t
    {
        Null_Data,

        One_Line,

        Rect_Same_Elem,

        Text_Left_Right,

        Four_Rect_Percent,
    };
    
    ////////////////////////////////////////////////////////////////////////////

    #define NVP_DRAW_PRIVATE(V)                                     \
    public:                                                         \
    void drawPrivate(const BaseElement& base, const NvpStyle& style)\
    {                                                               \
        V                                                           \
    }                                                               \

    ////////////////////////////////////////////////////////////////////////////

    class Null_Data_
    {

    };

    ////////////////////////////////////////////////////////////////////////////

    class Rect_Same_Elem_
    {
        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Rect_Same_Elem(base, style);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class Text_Left_Right_
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
            NvpDrawReal::Draw_Text_Left_Right(base, style, start, text, font_size);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class One_Line_
    {
    public:
        void setPoint1(NvpXyCoord pt1) { p1 = pt1; }
        void setPoint2(NvpXyCoord pt2) { p2 = pt2; }

        NvpXyCoord getPoint1() { return p1; }
        NvpXyCoord getPoint2() { return p2; }

    private:
        NvpXyCoord p1 = { 0 };
        NvpXyCoord p2 = { 0 };

        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_One_Line(base, style, p1, p2);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class Four_Rect_Percent_
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
    NvpDrawCache(const NvpStyle& style, const NvpDrawData::Command command);
    NvpDrawCache(const NvpDrawData::Command command);
    ~NvpDrawCache();

    static void commandString(const NvpDrawData::Command command, char* const str);

    void colorBright();
    void colorDarker();

    void setColor(NvpColor color);
    NvpColor getColor() const;

    void setStyle(NvpStyle::Style style);
    NvpStyle::Style getStyle() const;

    template<typename T, NvpDrawData::Command D>
    class draw_safe
    {
    public:
        draw_safe(NvpDrawData::Command c) : cmd(c), ptr(nullptr)
        {
            ptr = new T;
        }

        T* operator->() const //override ->;
        {
            uint8_t draw_cmd = D;
            assert(draw_cmd == this->cmd);
            return ptr;
        }

        ~draw_safe()
        {
            assert(ptr != nullptr);
        }

        T* getPtr() const { return ptr; }
        NvpDrawData::Command getCmd() const { return cmd; }

    private:
        T* ptr;
        NvpDrawData::Command cmd;

        friend class NvpDrawCache;
    };

    union
    {
        draw_safe<NvpDrawData::Null_Data_, NvpDrawData::Null_Data> null_data;

        draw_safe<NvpDrawData::One_Line_, NvpDrawData::One_Line> const one_line;
        
        draw_safe<NvpDrawData::Rect_Same_Elem_, NvpDrawData::Rect_Same_Elem> const rect_same_elem;
        
        draw_safe<NvpDrawData::Text_Left_Right_, NvpDrawData::Text_Left_Right> const text_left_right;

        draw_safe<NvpDrawData::Four_Rect_Percent_, NvpDrawData::Four_Rect_Percent> const four_rect_percent;

    };

private:
    enum Opt { NUL, NEW, DELE, DRAW };
    
    void OptByPush(const Opt opt);

    struct Param
    {
        Param(const BaseElement& base) : base_elem(base) {};
        
        const BaseElement& base_elem;
    };

    void OptSwitch(const Opt opt, const Param* const param = nullptr, char* const str = nullptr);

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
            if (!this->is_push && t->getPtr() != nullptr)
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
