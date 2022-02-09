/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include "nvp_draw.h"
#include "util/nvp_util.h"

class NvpStyle;

class NvpDrawReal
{
public:
    static void Draw_Null() {}

    static void Draw_One_Rect(const NvpRect& rect, const NvpStyle& style);

    static void Draw_Text_Left_Right(const NvpBaseObj& base, const NvpStyle& style,
        NvpXyCoord xy, const std::string& str, ptSize font_size);

    static void Draw_One_Line(const NvpBaseObj& base, const NvpStyle& style,
        NvpXyCoord p1, NvpXyCoord p2);

    static void Draw_Rect_Same_Elem(const NvpBaseObj& base, const NvpStyle& style);

    static void Draw_Four_Rect_Percent(const NvpBaseObj& base, const NvpStyle& style,
        int percent);

};

namespace NvpDrawData
{
    ////////////////////////////////////////////////////////////////////////////
    
    enum Command : uint8_t
    {
        Null_Data = 0U,

        One_Line = 1U,

        Rect_Same_Elem = 2U,

        Text_Left_Right = 3U,

        Four_Rect_Percent = 4U,
    };
    
    ////////////////////////////////////////////////////////////////////////////

    #define NVP_DRAW_PRIVATE(V)                                     \
    public:                                                         \
    void drawPrivate(const NvpBaseObj& base, const NvpStyle& style) \
    {                                                               \
        V                                                           \
    }                                                               \

    #define NVP_DRAW_CODING(V)                                      \
    public:                                                         \
    void drawCoding(std::string* dst, const char* src)              \
    {                                                               \
        V                                                           \
    }                                                               \

    ////////////////////////////////////////////////////////////////////////////

    class Null_Data_
    {

    };
    static Null_Data_ null_data;
    ////////////////////////////////////////////////////////////////////////////

    class Rect_Same_Elem_
    {
        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Rect_Same_Elem(base, style);
        )
        NVP_DRAW_CODING
        (
            NvpCoding::codingSeq(dst, src, &null_data);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class Text_Left_Right_
    {
    public:
        Text_Left_Right_() : text("?"), start{ 0 }, font_size(12) {}
        void setText(const std::string& str) { text = str; }
        void setStart(NvpXyCoord xy) { start = xy; }

        const std::string& getText() const { return text; }
        NvpXyCoord getStart() const { return start; }

        ptSize getFontSize() const { return font_size; }
        void setFontSize(ptSize size) { assert(size > 0); font_size = size; }

    private:
        std::string text;
        NvpXyCoord start;
        ptSize font_size;

        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Text_Left_Right(base, style, start, text, font_size);
        )
        NVP_DRAW_CODING
        (
            NvpCoding::codingSeq(dst, src, &text, &start.x, &start.y, &font_size);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class One_Line_
    {
    public:
        One_Line_() : p1{ 0 }, p2{ 0 } {}
        void setPoint1(NvpXyCoord pt1) { p1 = pt1; }
        void setPoint2(NvpXyCoord pt2) { p2 = pt2; }

        NvpXyCoord getPoint1() const { return p1; }
        NvpXyCoord getPoint2() const { return p2; }

    private:
        NvpXyCoord p1;
        NvpXyCoord p2;

        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_One_Line(base, style, p1, p2);
        )
        NVP_DRAW_CODING
        (
            NvpCoding::codingSeq(dst, src, &p1.x, &p1.y, &p2.x, &p2.y);
        )
    };

    ////////////////////////////////////////////////////////////////////////////

    class Four_Rect_Percent_
    {
    public:
        Four_Rect_Percent_() : percent(0) {}
        void setPercent(uint32_t per /*1~50*/) { percent = per; }
        uint32_t getPercent() const { return percent; }

    private:
        uint32_t percent;

        NVP_DRAW_PRIVATE
        (
            NvpDrawReal::Draw_Four_Rect_Percent(base, style, percent);
        )
        NVP_DRAW_CODING
        (
            NvpCoding::codingSeq(dst, src, &percent);
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
    
    NVP_DRAW_CODING
    (
        NvpCoding::codingSeq(dst, src, &color.Red, &color.Green, &color.Blue,
            &color.Alpha, reinterpret_cast<uint8_t*>(&style));
    )
};

class NvpDrawCache
{
public:
    NvpDrawCache(const NvpStyle& style, const NvpDrawData::Command command);
    NvpDrawCache(const NvpStyle& style, const NvpDrawData::Command command, const char* src);
    NvpDrawCache(const NvpDrawData::Command command);
    ~NvpDrawCache();

    NvpDrawCache(const NvpDrawCache& cache) = delete;
    NvpDrawCache& operator=(const NvpDrawCache& cache) = delete;
    NvpDrawCache(NvpDrawCache&& cache) = default;

    //static const char* commandString(const NvpDrawData::Command command);

    void colorBright();
    void colorDarker();

    void setColor(NvpColor color);
    NvpColor getColor() const;

    void setStyle(NvpStyle::Style style);
    NvpStyle::Style getStyle() const;

    NvpDrawData::Command getCommand() const;
    void encodeSeq(std::string* dst);
    static NvpDrawCache decodeSeq(const char* src);

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

        draw_safe<NvpDrawData::One_Line_, NvpDrawData::One_Line> one_line;

        draw_safe<NvpDrawData::Rect_Same_Elem_, NvpDrawData::Rect_Same_Elem> rect_same_elem;

        draw_safe<NvpDrawData::Text_Left_Right_, NvpDrawData::Text_Left_Right> text_left_right;

        draw_safe<NvpDrawData::Four_Rect_Percent_, NvpDrawData::Four_Rect_Percent> four_rect_percent;

    };

private:
    enum Opt { NUL, NEW, DELE, DRAW, CODING };
    
    void OptByPush(const Opt opt);

    struct Param
    {
        Param() : base(nullptr), opt(NUL), dst(nullptr), src(nullptr) {}
        const NvpBaseObj& getBase() const { return *base; }

        const NvpBaseObj* base;
        Opt opt;
        std::string* dst;
        const char* src;
    };

    void OptSwitch(const Param& param);

    template<typename T>
    void NvpOptPush(T& t, const Param& param)
    {
        switch (param.opt)
        {
        case NEW:
        {
            t = T(this->null_data.cmd);
        }
        break;

        case DELE:
        {
            if (!this->is_push && t.getPtr() != nullptr)
            {
                delete t.getPtr(); //from draw_safe;
            }
        }
        break;

        case DRAW:
        {
            t.getPtr()->drawPrivate(param.getBase(), this->a_style); //from NvpDrawData;
        }
        break;

        case CODING:
        {
            t.getPtr()->drawCoding(param.dst, param.src);
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
