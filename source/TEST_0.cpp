#include "base/base.h"
#include "draw/draw_priv.h"

class TopNodeView : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        NvpColor col = { 40, 40, 45 };
        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);
    }

    void mouseLDown(BaseElement* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("node_view_select", base);
        elem->getSelfEvent()->mouseLDown(elem, param);
    }

    void mouseLUp(BaseElement* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("node_view_select", base);
        elem->getSelfEvent()->mouseLUp(elem, param);
        param.freeCaptureMouse();
    }

    void mouseLDrag(BaseElement* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("node_view_select", base);
        elem->getSelfEvent()->mouseLDrag(elem, param);
        param.wantCaptureMouse(base);
    }
};

////////////////////////////////////////////////////////////////////////////////

class NodeViewSelect : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        NvpColor col = { 200, 200, 200, 50 };
        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);

        NvpColor col2 = { 200, 200, 200, 220 };
        NvpDrawCache one_rect_border(Draw_Rect_Same_Elem);
        one_rect_border.setStyle(NvpStyle::Frame);
        one_rect_border.setColor(col2);
        base->getSelfDraw()->pushDraw(one_rect_border);
    }

    void mouseLDown(BaseElement* base, NvpEventParam& param) override
    {
        down_pt = param.getLocalPt();
    }

    void mouseLUp(BaseElement* base, NvpEventParam& param) override
    {
        NvpLayout::setBaseRect(base, { 0, 0, 0, 0 });
        down_pt = { 0 };
    }

    void mouseLDrag(BaseElement* base, NvpEventParam& param) override
    {
        mousePt now_pt = param.getLocalPt();
        BaseRect select;
        select.left = down_pt.x;
        select.top = down_pt.y;
        select.right = now_pt.x;
        select.bottom = now_pt.y;
        NvpLayout::setBaseRect(base, select);
    }

    mousePt down_pt;
};

////////////////////////////////////////////////////////////////////////////////

class TopMenuStat : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        auto draw = base->getSelfDraw();
        NvpColor col = { 100, 200, 255 };
        NvpColor font_col = { 230, 230, 230 };
        
        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        draw->pushDraw(one_rect);

        NvpDrawCache text(Draw_Text_One_Line);
        text.setColor(font_col);
        text.text_one_line->setText("  Menu  |  Stat");
        text.text_one_line->setStart({ 0, 13 });
        draw->pushDraw(text);
    }
};

////////////////////////////////////////////////////////////////////////////////

static void nodeInit(BaseElement* base, const BaseRect& rect, const NvpColor& colo, float font_size)
{
    auto draw = base->getSelfDraw();
    NvpLayout::setBaseRect(base, rect);
    
    NvpDrawCache rect_elem(Draw_Rect_Same_Elem);
    rect_elem.setColor(colo);
    draw->pushDraw(rect_elem);

    NvpDrawCache font(Draw_Text_One_Line);
    font.setColor({ 240, 240, 240 });
    font.text_one_line->setFontSize(font_size);
    draw->pushDraw(font);
}

static void nodeText(BaseElement* base, NvpEventParam& param)
{
    mousePt local_pt = param.getLocalPt();
    auto font = base->getSelfDraw()->getDraw(1);
    font->text_one_line->setStart({ local_pt.x, local_pt.y });
    
    char temp[50];
    sprintf(temp, "X=%f, Y=%f", local_pt.x, local_pt.y);
    std::string str(temp);
    font->text_one_line->setText(str);
}

class TempNode1 : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        BaseRect rect;
        rect.left = rect.top = 30;
        rect.right = rect.bottom = 150;
        
        NvpColor col = { 150, 150, 250 };
        nodeInit(base, rect, col, 15);
    }

    void mouseMove(BaseElement* base, NvpEventParam& param) override
    {
        nodeText(base, param);
    }
};

class TempNode2 : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        BaseRect rect;
        rect.left = rect.top = 30;
        rect.right = rect.bottom = 100;
        
        NvpColor col = { 250, 150, 150 };
        nodeInit(base, rect, col, 12);
    }

    void mouseMove(BaseElement* base, NvpEventParam& param) override
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
