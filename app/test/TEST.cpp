#include "base/base.h"
#include "draw/draw_priv.h"

class ActInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;
        NvpLayout::setBaseRect(base, rect);

        NvpColor col = { 150, 150, 150 };
        NvpColor font_col = { 230, 230, 230 };

        NvpDrawCache rect_elem(Draw_Rect_Same_Elem);
        rect_elem.setColor(col);
        nvp_draw->pushDraw(rect_elem);

        NvpDrawCache text(Draw_Text_One_Line);
        text.setColor(font_col);
        text.text_one_line->setText("Node-V1-!@#$%^&*()_+");
        text.text_one_line->setStart({ 0, 25 });
        nvp_draw->pushDraw(text);
    }
}ActInit;

class ActMouseEnter : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto rect = nvp_draw->getDraw(0);
        rect->colorBright();
    }
}ActMouseEnter;

class ActMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto rect = nvp_draw->getDraw(0);
        rect->colorDarker();
    }
}ActMouseLeave;

class ActMouseLButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        
    }
}ActMouseLButtonDown;

class ActMouseLButtonUp : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {

    }
}ActMouseLButtonUp;

////////////////////////////////////////////////////////////////////////
class Act2Init : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = 50;
        rect.right = 90;
        rect.top = 10;
        rect.bottom = 50;
        NvpLayout::setBaseRect(base, rect);

        NvpColor col = { 0, 150, 150 };
        NvpColor font_col = { 230, 230, 230 };

        NvpDrawCache rect_elem(Draw_Rect_Same_Elem);
        rect_elem.setColor(col);
        nvp_draw->pushDraw(rect_elem);

        NvpDrawCache text(Draw_Text_One_Line);
        text.setColor(font_col);
        text.text_one_line->setText("fNode-V2-font-Test");
        text.text_one_line->setStart({ 0, 25 });
        nvp_draw->pushDraw(text);
    }
}Act2Init;

class Act2MouseEnter : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto rect = nvp_draw->getDraw(0);
        rect->colorBright();
    }
}Act2MouseEnter;

class Act2MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto rect = nvp_draw->getDraw(0);
        rect->colorDarker();
    }
}Act2MouseLeave;
/////////////////////////////////////////////////////////////////////////

static void subLevelRecord(BaseElement* elem)
{
    #if 0
    if (!elem)
    {
        return;
    }

    auto sub_level = elem->getSelfLayout()->sub;
    if (!sub_level)
    {
        return;
    }

    size_t size = sub_level->size();
    auto iter = sub_level->begin();
    
    for (size_t i = 0; i < size - 1; ++i)
    {
        auto next = *(++iter);
        auto rect = next->body.elem->getRectRefUp();
        
        //NvpColor col = { 150, 150, 150 };
        nvpDraw->Record(next->body.elem, 0, 0, NoneDraw, rect);
        subLevelRecord(next->body.elem);
    }
    #endif
}

class Act1MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        mousePt last_pt = baseMsg->getLastMouseLDown();
        ptSize sub_x = local_pt.x - last_pt.x;
        ptSize sub_y = local_pt.y - last_pt.y;
        const BaseRect& old_rect = base->getRectRefUp();

        BaseRect new_rect;
        new_rect.left = old_rect.left + sub_x;
        new_rect.right = old_rect.right + sub_x;
        new_rect.top = old_rect.top + sub_y;
        new_rect.bottom = old_rect.bottom + sub_y;

        NvpLayout::setBaseRect(base, new_rect);

        subLevelRecord(base);
    }
}Act1MouseDrag;

class Act2MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        mousePt last_pt = baseMsg->getLastMouseLDown();
        ptSize sub_x = local_pt.x - last_pt.x;
        ptSize sub_y = local_pt.y - last_pt.y;
        const BaseRect& old_rect = base->getRectRefUp();

        BaseRect new_rect;
        new_rect.left = old_rect.left + sub_x;
        new_rect.right = old_rect.right + sub_x;
        new_rect.top = old_rect.top + sub_y;
        new_rect.bottom = old_rect.bottom + sub_y;

        NvpLayout::setBaseRect(base, new_rect);
    }
}Act2MouseDrag;

////////////////////////////////////////////////////////////////////////
class Act3Init : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = 110;
        rect.right = 150;
        rect.top = 10;
        rect.bottom = 50;
        NvpLayout::setBaseRect(base, rect);

        NvpColor hColor = { 0, 200, 200 };
        NvpColor hColor2 = { 200, 200, 200 };

        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(hColor);
        nvp_draw->pushDraw(one_rect);

        NvpDrawCache four_rect(Draw_Four_Rect_Percent);
        four_rect.setColor(hColor2);
        four_rect.four_rect_percent->setPercent(40);
        nvp_draw->pushDraw(four_rect);

        NvpDrawCache font(Draw_Text_One_Line);
        font.setColor({ 200, 100, 100 });
        font.text_one_line->setText(" -Click Change Node Font Size-");
        font.text_one_line->setStart({ 40, 25 });
        nvp_draw->pushDraw(font);
    }
}Act3Init;

class Act3MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto four_rect = nvp_draw->getDraw(1);
        four_rect->four_rect_percent->setPercent(36);
    }
}Act3MouseMove;

class Act3MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto four_rect = nvp_draw->getDraw(1);
        four_rect->four_rect_percent->setPercent(40);
    }
}Act3MouseLeave;

//global opt speed control;
//static RecordOption opt[] = { NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw,
//    NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, Draw };

class ActRandomInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 100;
        static int move_x = 0;
        static int move_y = 0;
        move_x += 50; move_y += 50;
        if (move_x >= 300)
        {
            move_x = 50;
            move_y = 50;
        }
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        static double tr, tg, tb;
        tr += 0.1; tg += 0.2, tb += 0.3;
        int r = 255 * sin(tr + rand() / 1000);
        int g = 255 * cos(tg + rand() / 1000);
        int b = 255 * sin(tb + rand() / 1000);
        NvpColor col = { static_cast<uint8_t>(abs(r)), static_cast<uint8_t>(abs(g)), static_cast<uint8_t>(abs(b)) };

        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        NvpLayout::setBaseRect(base, rect);
        nvp_draw->pushDraw(one_rect);
    }
}ActRandomInit;

class ActSubInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 40;
        static int move_x = 0;
        static int move_y = 0;
        move_x += 15; move_y += 15;
        if (move_x >= 60)
        {
            move_x = 15;
            move_y = 15;
        }
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        static double tr, tg, tb;
        tr += 0.2; tg += 0.3, tb += 0.4;
        int r = 255 * sin(tr + rand() / 1000);
        int g = 255 * cos(tg + rand() / 1000);
        int b = 255 * sin(tb + rand() / 1000);
        NvpColor col = { static_cast<uint8_t>(abs(r)), static_cast<uint8_t>(abs(g)), static_cast<uint8_t>(abs(b)) };

        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        NvpLayout::setBaseRect(base, rect);
        nvp_draw->pushDraw(one_rect);
    }
}ActSubInit;

class Act2SubInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 20;
        static int move_x = 0;
        static int move_y = 0;
        move_x += 10; move_y += 10;
        if (move_x >= 20)
        {
            move_x = 10;
            move_y = 10;
        }
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        static double tr, tg, tb;
        tr += 0.3; tg += 0.2, tb += 0.1;
        int r = 255 * sin(tr + rand() / 1000);
        int g = 255 * cos(tg + rand() / 1000);
        int b = 255 * sin(tb + rand() / 1000);
        NvpColor col = { static_cast<uint8_t>(abs(r)), static_cast<uint8_t>(abs(g)), static_cast<uint8_t>(abs(b)) };

        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        NvpLayout::setBaseRect(base, rect);
        nvp_draw->pushDraw(one_rect);
    }
}Act2SubInit;

class Act3SubInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 10;
        static int move_x = 0;
        static int move_y = 0;
        move_x += 5; move_y += 5;
        if (move_x >= 10)
        {
            move_x = 5;
            move_y = 5;
        }
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        static double tr, tg, tb;
        tr += 0.5; tg += 0.4, tb += 0.3;
        int r = 255 * sin(tr + rand() / 1000);
        int g = 255 * cos(tg + rand() / 1000);
        int b = 255 * sin(tb + rand() / 1000);
        NvpColor col = { static_cast<uint8_t>(abs(r)), static_cast<uint8_t>(abs(g)), static_cast<uint8_t>(abs(b)) };

        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        NvpLayout::setBaseRect(base, rect);
        nvp_draw->pushDraw(one_rect);
    }
}Act3SubInit;

static void subLevelRemove(BaseElement* elem)
{
    #if 0
    if (!elem)
    {
        return;
    }

    auto sub_level = elem->getSelfLayout()->sub;
    if (!sub_level)
    {
        return;
    }

    size_t size = sub_level->size();
    auto iter = sub_level->begin();
    
    for (size_t i = 0; i < size - 1; ++i)
    {
        auto iter = sub_level->begin(); //something was delete;
        auto next = *(++iter);
        NvpColor col = { 0, 0, 0 };
        nvpDraw->Record(next->body.elem, 0, &col, Draw);
        subLevelRemove(next->body.elem);
        NvpLayout::elemDel(next->body.elem->getSelfName(), next->body.elem->getSelfLevel());
    }
    #endif
}

class ActMouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 0, 0, 0 };
        //nvpDraw->Record(base, 0, &col);

        subLevelRemove(base);
        
        //NvpLayout::elemDel(base->getSelfName(), base->getSelfLevel());
    }
}ActMouseRButtonDown;

#define MAX_TEST_ELEM 5

class Act3MouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        #if 0
        for (elemIDSize add_number = 0; add_number < MAX_TEST_ELEM; ++add_number)
        {
            std::string number_str("add_");
            std::string add_str = std::to_string(add_number);
            number_str = number_str + add_str;

            auto iter = NvpLayout::g_top_node_view->getSelfLayout()->sub->begin();
            auto elem_map = (*iter)->head->cur_map;
            auto ret = elem_map->find(number_str);
            if (ret == elem_map->end())
            {
                continue;
            }
            else
            {
                auto temp = *ret;
                auto shape = temp.second;

                static int offset = -1; ++offset;

                NvpColor col = { 0, 0, 0 };
                nvpDraw->Record(shape, 0, &col);

                NvpLayout::elemDel(number_str, NvpLayout::g_top_node_view->getSelfLayout()->sub);
            }
        }
        #endif

        auto v2 = NvpLayout::findSameLevel("v2", base);

        if (v2)
        {
            auto draw = v2->getSelfDraw();
            auto font = draw->getDraw(1);
            auto font_size = font->text_one_line->getFontSize();
            font_size -= 0.5f;
            font->text_one_line->setFontSize(font_size);
        }
    }
}Act3MouseRButtonDown;

class Act3MouseLButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        for (elemIDSize add_number = 0; add_number < MAX_TEST_ELEM; ++add_number)
        {
            std::string number_str("add_");
            std::string add_str = std::to_string(add_number);
            number_str = number_str + add_str;

            auto elem_top = NvpLayout::getTopNodeView();

            auto elem = NvpLayout::subElemGen(number_str, MsgInit, &ActRandomInit, elem_top);
            mousePt pt;
            elem->msgRoute(MsgInit, &pt);
            // same act as v1 except init position;
            NvpLayout::subElemGen(number_str, MouseEnter, &ActMouseEnter, elem_top);
            NvpLayout::subElemGen(number_str, MouseLeave, &ActMouseLeave, elem_top);
            NvpLayout::subElemGen(number_str, MouseRButtonDown, &ActMouseRButtonDown, elem_top); //Delete;
            NvpLayout::subElemGen(number_str, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem_top);
            NvpLayout::subElemGen(number_str, MouseLButtonDown, &ActMouseLButtonDown, elem_top);


            /////////////////////////////////////////////////////////////////
            std::string sub_string;
            sub_string = "_1";
            
            auto sub_elem_1 = NvpLayout::subElemGen(sub_string, MsgInit, &ActSubInit, elem);
            sub_elem_1->msgRoute(MsgInit);
            // same act as v1 except init position;
            NvpLayout::subElemGen(sub_string, MouseEnter, &ActMouseEnter, elem);
            NvpLayout::subElemGen(sub_string, MouseLeave, &ActMouseLeave, elem);
            NvpLayout::subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, elem); //Delete;
            NvpLayout::subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem);
            NvpLayout::subElemGen(sub_string, MouseLButtonDown, &ActMouseLButtonDown, elem);

            /////////////////////////////////////////////////////////////////
            sub_string = "_2";
            
            auto sub_elem_2 = NvpLayout::subElemGen(sub_string, MsgInit, &ActSubInit, elem);
            sub_elem_2->msgRoute(MsgInit);
            // same act as v1 except init position;
            NvpLayout::subElemGen(sub_string, MouseEnter, &ActMouseEnter, elem);
            NvpLayout::subElemGen(sub_string, MouseLeave, &ActMouseLeave, elem);
            NvpLayout::subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, elem); //Delete;
            NvpLayout::subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem);
            NvpLayout::subElemGen(sub_string, MouseLButtonDown, &ActMouseLButtonDown, elem);

            /////////////////////////////////////////////////////////////////
            sub_string = "_3";
            
            auto sub_elem_3 = NvpLayout::subElemGen(sub_string, MsgInit, &ActSubInit, elem);
            sub_elem_3->msgRoute(MsgInit);
            // same act as v1 except init position;
            NvpLayout::subElemGen(sub_string, MouseEnter, &ActMouseEnter, elem);
            NvpLayout::subElemGen(sub_string, MouseLeave, &ActMouseLeave, elem);
            NvpLayout::subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, elem); //Delete;
            NvpLayout::subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem);
            NvpLayout::subElemGen(sub_string, MouseLButtonDown, &ActMouseLButtonDown, elem);

            /////////////////////////////////////////////////////////////////
            sub_string = "_1";

            auto sub_elem_1_1 = NvpLayout::subElemGen(sub_string, MsgInit, &Act2SubInit, sub_elem_1);
            sub_elem_1_1->msgRoute(MsgInit);
            // same act as v1 except init position;
            NvpLayout::subElemGen(sub_string, MouseEnter, &ActMouseEnter, sub_elem_1);
            NvpLayout::subElemGen(sub_string, MouseLeave, &ActMouseLeave, sub_elem_1);
            NvpLayout::subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_elem_1); //Delete;
            NvpLayout::subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_elem_1);
            NvpLayout::subElemGen(sub_string, MouseLButtonDown, &ActMouseLButtonDown, sub_elem_1);

            /////////////////////////////////////////////////////////////////
            sub_string = "_1";

            elem = NvpLayout::subElemGen(sub_string, MsgInit, &Act3SubInit, sub_elem_1_1);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            NvpLayout::subElemGen(sub_string, MouseEnter, &ActMouseEnter, sub_elem_1_1);
            NvpLayout::subElemGen(sub_string, MouseLeave, &ActMouseLeave, sub_elem_1_1);
            NvpLayout::subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_elem_1_1); //Delete;
            NvpLayout::subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_elem_1_1);
            NvpLayout::subElemGen(sub_string, MouseLButtonDown, &ActMouseLButtonDown, sub_elem_1_1);
        }
        auto v2 = NvpLayout::findSameLevel("v2", base);

        if (v2)
        {
            auto draw = v2->getSelfDraw();
            auto font = draw->getDraw(1);
            auto font_size = font->text_one_line->getFontSize();
            font_size += 0.5f;
            font->text_one_line->setFontSize(font_size);
        }
    }
}Act3MouseLButtonDown;


ELEM_GEN(v1, MsgInit, ActInit)
ELEM_GEN(v1, MouseEnter, ActMouseEnter)
ELEM_GEN(v1, MouseLeave, ActMouseLeave)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)
ELEM_GEN(v1, MouseLButtonDown, ActMouseLButtonDown)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseEnter, Act2MouseEnter)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)
ELEM_GEN(v2, MouseLButtonDown, ActMouseLButtonDown)

ELEM_GEN(v3, MsgInit, Act3Init) //v3 is Add Button;
ELEM_GEN(v3, MouseMove, Act3MouseMove)
ELEM_GEN(v3, MouseLeave, Act3MouseLeave)
ELEM_GEN(v3, MouseLButtonDown, Act3MouseLButtonDown) //generate
ELEM_GEN(v3, MouseRButtonDown, Act3MouseRButtonDown) //delete

/////////////////////////////////////////////////////////////////////

class StatInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 100, 200, 255 };
        NvpColor font_col = { 230, 230, 230 };
        
        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        nvp_draw->pushDraw(one_rect);

        NvpDrawCache text(Draw_Text_One_Line);
        text.setColor(font_col);
        text.text_one_line->setText("  Menu  |  Stat");
        text.text_one_line->setStart({ 0, 13 });
        nvp_draw->pushDraw(text);
    }
}StatInit;

class StatMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 200, 200, 255 };
        auto one_rect = nvp_draw->getDraw(0);
        one_rect->setColor(col);
    }
}StatMouseMove;

class StatMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 100, 200, 255 };
        auto one_rect = nvp_draw->getDraw(0);
        one_rect->setColor(col);
    }
}StatMouseLeave;

ELEM_GEN_FULL(top_menu_stat, MsgInit, StatInit, NvpLayout::getTopLayout())
ELEM_GEN_FULL(top_menu_stat, MouseMove, StatMouseMove, NvpLayout::getTopLayout())
ELEM_GEN_FULL(top_menu_stat, MouseLeave, StatMouseLeave, NvpLayout::getTopLayout())
/////////////////////////////////////////////////////////////////////

class NodeViewInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 40, 40, 45 };
        NvpDrawCache one_rect(Draw_Rect_Same_Elem);
        one_rect.setColor(col);
        nvp_draw->pushDraw(one_rect);
    }
}NodeViewInit;

ELEM_GEN_FULL(top_node_view, MsgInit, NodeViewInit, NvpLayout::getTopLayout())
