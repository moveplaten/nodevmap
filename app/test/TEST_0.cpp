/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/base.h"
#include "draw/draw_priv.h"

class TopNodeView : public NvpEvent
{
    void userInit(NvpBaseObj* base, NvpEventParam& param) override
    {
        NvpColor col = { 40, 40, 45 };
        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);
    }

    void mouseLDown(NvpBaseObj* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("node_view_select", base);
        elem->getSelfEvent()->mouseLDown(elem, param);
    }

    void mouseLUp(NvpBaseObj* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("node_view_select", base);
        elem->getSelfEvent()->mouseLUp(elem, param);
        param.freeCaptureMouse();
    }

    void mouseLDrag(NvpBaseObj* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("node_view_select", base);
        elem->getSelfEvent()->mouseLDrag(elem, param);
        param.wantCaptureMouse(base);
    }
};

////////////////////////////////////////////////////////////////////////////////

class NodeViewSelect : public NvpEvent
{
    void userInit(NvpBaseObj* base, NvpEventParam& param) override
    {
        NvpColor col = { 200, 200, 200, 50 };
        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);

        NvpColor col2 = { 200, 200, 200, 220 };
        NvpDrawCache one_rect_border(NvpDrawData::Rect_Same_Elem);
        one_rect_border.setStyle(NvpStyle::Frame);
        one_rect_border.setColor(col2);
        base->getSelfDraw()->pushDraw(one_rect_border);

        NvpColor col3 = { 0, 100, 200, 230 };
        NvpDrawCache one_line(NvpDrawData::One_Line);
        one_line.setColor(col3);
        base->getSelfDraw()->pushDraw(one_line);
    }

    void mouseLDown(NvpBaseObj* base, NvpEventParam& param) override
    {
        down_pt = param.getLocalPt();
    }

    void mouseLUp(NvpBaseObj* base, NvpEventParam& param) override
    {
        NvpLayout::setBaseRect(base, { 0, 0, 0, 0 });
        down_pt = { 0 };
        auto one_line = base->getSelfDraw()->getDraw(2);
        one_line->one_line->setPoint1({ 0 });
        one_line->one_line->setPoint2({ 0 });
    }

    void mouseLDrag(NvpBaseObj* base, NvpEventParam& param) override
    {
        mousePt now_pt = param.getLocalPt();
        BaseRect select;
        select.left = down_pt.x;
        select.top = down_pt.y;
        select.right = now_pt.x;
        select.bottom = now_pt.y;
        NvpLayout::setBaseRect(base, select);

        NvpXyCoord p1, p2;
        p1.x = 10;
        p1.y = 10;
        
        p2.x = select.right - select.left - 10;
        p2.y = select.bottom - select.top - 10;
        auto one_line = base->getSelfDraw()->getDraw(2);
        one_line->one_line->setPoint1(p1);
        one_line->one_line->setPoint2(p2);
    }

    mousePt down_pt;
};

////////////////////////////////////////////////////////////////////////////////

class TopMenuStat : public NvpEvent
{
    void userInit(NvpBaseObj* base, NvpEventParam& param) override
    {
        auto draw = base->getSelfDraw();
        NvpColor col = { 100, 200, 255 };
        NvpColor font_col = { 230, 230, 230 };
        
        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(col);
        draw->pushDraw(one_rect);

        NvpDrawCache text(NvpDrawData::Text_Left_Right);
        text.setColor(font_col);
        text.text_left_right->setText("  Menu  |  Stat");
        text.text_left_right->setStart({ 0, 13 });
        draw->pushDraw(text);
    }
};

////////////////////////////////////////////////////////////////////////////////

#define INIT_STRING(x) #x;

static const char* node_1_init_str = INIT_STRING
(
    @Rect_Same_Elem
    @150 150 250

    @Text_Left_Right
    @240 240 240
    @end
)

static const char* node_2_init_str = INIT_STRING
(
    @Rect_Same_Elem
    @250 150 150

    @Text_Left_Right
    @240 240 240
    @end
)

static NvpColor toColor(const char* col)
{
    std::string col_str(col);
    char red[10] = { 0 };
    char green[10] = { 0 };
    char blue[10] = { 0 };
    
    col_str.copy(red, 3, 0);
    col_str.copy(green, 3, 4);
    col_str.copy(blue, 3, 8);
    
    auto _red = atoi(red);
    auto _green = atoi(green);
    auto _blue = atoi(blue);
    
    NvpColor colo{ 0 };
    colo.Red = _red;
    colo.Green = _green;
    colo.Blue = _blue;
    
    return colo;
}

static void initCmdColStr(NvpDraw* draw, const char* cmd, const char* col, float font_size)
{
    char rect_same_elem[30] = { 0 };
    NvpDrawCache::commandString(NvpDrawData::Rect_Same_Elem, rect_same_elem);
    char text_left_right[30] = { 0 };
    NvpDrawCache::commandString(NvpDrawData::Text_Left_Right, text_left_right);

    NvpColor colo = toColor(col);

    if (strcmp(cmd, rect_same_elem) == 0)
    {
        NvpDrawCache rect_elem(NvpDrawData::Rect_Same_Elem);
        rect_elem.setColor(colo);
        draw->pushDraw(rect_elem);
    }
    
    if (strcmp(cmd, text_left_right) == 0)
    {
        NvpDrawCache font(NvpDrawData::Text_Left_Right);
        font.setColor(colo);
        font.text_left_right->setFontSize(font_size);
        draw->pushDraw(font);
    }
}

static void initFromStr(NvpDraw* draw, const char* str, float font_size)
{
    std::string init_str(str);

    auto col1 = init_str.find("@", 1);
    char cmd_str1[30] = { 0 };
    init_str.copy(cmd_str1, col1 - 2, 1);
    
    auto cmd2 = init_str.find("@", col1 + 1);
    char col_str1[30] = { 0 };
    init_str.copy(col_str1, cmd2 - col1 - 2, col1 + 1);
    
    auto col2 = init_str.find("@", cmd2 + 1);
    char cmd_str2[30] = { 0 };
    init_str.copy(cmd_str2, col2 - cmd2 - 2, cmd2 + 1);

    auto end = init_str.find("@", col2 + 1);
    char col_str2[30] = { 0 };
    init_str.copy(col_str2, end - col2 - 2, col2 + 1);

    initCmdColStr(draw, cmd_str1, col_str1, font_size);
    initCmdColStr(draw, cmd_str2, col_str2, font_size);
}

static void nodeInit(NvpBaseObj* base, const BaseRect& rect, const char* colo, float font_size)
{
    auto draw = base->getSelfDraw();
    NvpLayout::setBaseRect(base, rect);

    initFromStr(draw, colo, font_size);
}

static void nodeText(NvpBaseObj* base, NvpEventParam& param)
{
    mousePt local_pt = param.getLocalPt();
    auto font = base->getSelfDraw()->getDraw(1);
    font->text_left_right->setStart({ local_pt.x, local_pt.y });
    
    char temp[50];
    sprintf(temp, "X=%f, Y=%f", local_pt.x, local_pt.y);
    std::string str(temp);
    font->text_left_right->setText(str);
}

class TempNode1 : public NvpEvent
{
    void userInit(NvpBaseObj* base, NvpEventParam& param) override
    {
        BaseRect rect;
        rect.left = rect.top = 30;
        rect.right = rect.bottom = 150;
        
        nodeInit(base, rect, node_1_init_str, 15);
    }

    void mouseMove(NvpBaseObj* base, NvpEventParam& param) override
    {
        nodeText(base, param);
    }
};

class TempNode2 : public NvpEvent
{
    void userInit(NvpBaseObj* base, NvpEventParam& param) override
    {
        BaseRect rect;
        rect.left = rect.top = 30;
        rect.right = rect.bottom = 100;
        
        nodeInit(base, rect, node_2_init_str, 12);
    }

    void mouseMove(NvpBaseObj* base, NvpEventParam& param) override
    {
        nodeText(base, param);
    }
};

////////////////////////////////////////////////////////////////////////////////

void initFirst()
{
    NvpLayout::subElemGen("top_node_view", new TopNodeView, NvpLayout::getTopLayout());
    NvpLayout::subElemGen("top_menu_stat", new TopMenuStat, NvpLayout::getTopLayout());
    NvpLayout::subElemGen("node_view_select", new NodeViewSelect, NvpLayout::getTopLayout());

    auto sub = NvpLayout::subElemGen("v1", new TempNode1, NvpLayout::getTopNodeView());
    NvpLayout::subElemGen("_v1", new TempNode2, sub);
}
