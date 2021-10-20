#include "base/base.h"
#include "draw/draw.h"

class ActInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;

        auto draw = new NvpFillOneRect;
        NvpColor col = { 150, 150, 150 };
        nvpDraw->Record(base, 0, &col, Draw, &rect, draw);

        auto font = new NvpDrawTextOneLineOneColor;
        font->str_to_draw = "Node-V1-!@#$%^&*()_+";
        font->start_pt.y = 25;
        NvpColor font_col = { 230, 230, 230 };
        nvpDraw->Record(base, 1, &font_col, Draw, 0, font);
    }
}ActInit;

class ActMouseEnter : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto draw = (NvpFillOneRect*)nvpDraw->getOneDraw(base, 0);
        auto col = draw->color;
        col.Red += 50;
        col.Green += 50;
        col.Blue += 50;
        nvpDraw->Record(base, 0, &col, Draw);
    }
}ActMouseEnter;

class ActMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto draw = (NvpFillOneRect*)nvpDraw->getOneDraw(base, 0);
        auto col = draw->color;
        col.Red -= 50;
        col.Green -= 50;
        col.Blue -= 50;
        nvpDraw->Record(base, 0, &col, Draw);
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

        auto draw = new NvpFillOneRect;
        NvpColor col = { 0, 150, 150 };
        nvpDraw->Record(base, 0, &col, Draw, &rect, draw);

        auto font = new NvpDrawTextOneLineOneColor;
        font->str_to_draw = "fNode-V2-font-Test";
        font->start_pt.y = 25;
        NvpColor font_col = { 230, 230, 230 };
        nvpDraw->Record(base, 1, &font_col, Draw, 0, font);
    }
}Act2Init;

class Act2MouseEnter : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto draw = (NvpFillOneRect*)nvpDraw->getOneDraw(base, 0);
        auto col = draw->color;
        col.Red += 50;
        col.Green += 50;
        col.Blue += 50;
        nvpDraw->Record(base, 0, &col, Draw);
    }
}Act2MouseEnter;

class Act2MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto draw = (NvpFillOneRect*)nvpDraw->getOneDraw(base, 0);
        auto col = draw->color;
        col.Red -= 50;
        col.Green -= 50;
        col.Blue -= 50;
        nvpDraw->Record(base, 0, &col, Draw);
    }
}Act2MouseLeave;
/////////////////////////////////////////////////////////////////////////

static void subLevelRecord(BaseElement* elem)
{
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
}

class Act1MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        mousePt last_pt = baseMsg->getLastMouseLDown();
        ptSize sub_x = local_pt.x - last_pt.x;
        ptSize sub_y = local_pt.y - last_pt.y;
        const BaseRect* old_rect = base->getRectRefUp();

        //draw->fillRect(*old_rect, RGB(0, 0, 0), Begin);

        BaseRect new_rect;
        new_rect.left = old_rect->left + sub_x;
        new_rect.right = old_rect->right + sub_x;
        new_rect.top = old_rect->top + sub_y;
        new_rect.bottom = old_rect->bottom + sub_y;

        nvpDraw->Record(base, 0, 0, NoneDraw, &new_rect);

        subLevelRecord(base);

        //NvpColor col = { 200, 200, 200 };
        nvpDraw->Record(base, 0, 0, Draw);
    }
}Act1MouseDrag;

class Act2MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        mousePt last_pt = baseMsg->getLastMouseLDown();
        ptSize sub_x = local_pt.x - last_pt.x;
        ptSize sub_y = local_pt.y - last_pt.y;
        const BaseRect* old_rect = base->getRectRefUp();

        //draw->fillRect(*old_rect, RGB(0, 0, 0), Begin);

        BaseRect new_rect;
        new_rect.left = old_rect->left + sub_x;
        new_rect.right = old_rect->right + sub_x;
        new_rect.top = old_rect->top + sub_y;
        new_rect.bottom = old_rect->bottom + sub_y;

        //NvpColor col = { 0, 200, 200 };
        nvpDraw->Record(base, 0, 0, Draw, &new_rect);
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
    
        NvpColor hColor = { 0, 200, 200 };
        NvpColor hColor2 = { 200, 200, 200 };
        NvpColor colors[2];
        colors[0] = hColor;
        colors[1] = hColor2;

        auto draw = new NvpFillFiveRect(40);
        nvpDraw->Record(base, 0, colors, Draw, &rect, draw);
    }
}Act3Init;

class Act3MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor hColor = { 0, 200, 200 };
        NvpColor hColor2 = { 200, 200, 200 };
        NvpColor colors[2];
        colors[0] = hColor;
        colors[1] = hColor2;

        auto draw = (NvpFillFiveRect*)nvpDraw->getOneDraw(base, 0);
        draw->setPersent(36);

        nvpDraw->Record(base, 0, colors, Draw);
    }
}Act3MouseMove;

class Act3MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor hColor = { 0, 200, 200 };
        NvpColor hColor2 = { 200, 200, 200 };
        NvpColor colors[2];
        colors[0] = hColor;
        colors[1] = hColor2;

        auto draw = (NvpFillFiveRect*)nvpDraw->getOneDraw(base, 0);
        draw->setPersent(40);

        nvpDraw->Record(base, 0, colors, Draw);
    }
}Act3MouseLeave;

//global opt speed control;
static RecordOption opt[] = { NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw,
    NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, Draw };

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
        auto draw = new NvpFillOneRect;
        nvpDraw->Record(base, 0, &col, NoneDraw, &rect, draw);
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
        auto draw = new NvpFillOneRect;
        nvpDraw->Record(base, 0, &col, NoneDraw, &rect, draw);
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
        auto draw = new NvpFillOneRect;
        nvpDraw->Record(base, 0, &col, NoneDraw, &rect, draw);
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
        auto draw = new NvpFillOneRect;
        nvpDraw->Record(base, 0, &col, NoneDraw, &rect, draw);
    }
}Act3SubInit;

static void subLevelRemove(BaseElement* elem)
{
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
        nvpBuild->elemDel(next->body.elem->getSelfName(), next->body.elem->getSelfLevel());
    }
}

class ActMouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 0, 0, 0 };
        nvpDraw->Record(base, 0, &col);

        subLevelRemove(base);
        
        nvpBuild->elemDel(base->getSelfName(), base->getSelfLevel());
    }
}ActMouseRButtonDown;

#define MAX_TEST_ELEM 5

class Act3MouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        for (elemIDSize add_number = 0; add_number < MAX_TEST_ELEM; ++add_number)
        {
            std::string number_str("add_");
            std::string add_str = std::to_string(add_number);
            number_str = number_str + add_str;

            auto iter = nvpBuild->g_top_node_view->getSelfLayout()->sub->begin();
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

                nvpBuild->elemDel(number_str, nvpBuild->g_top_node_view->getSelfLayout()->sub);
            }
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

            BaseElement* elem = nvpBuild->subElemGen(number_str, MsgInit, &ActRandomInit, nvpBuild->g_top_node_view);
            mousePt pt;
            elem->msgRoute(MsgInit, &pt);
            // same act as v1 except init position;
            nvpBuild->subElemGen(number_str, MouseEnter, &ActMouseEnter, nvpBuild->g_top_node_view);
            nvpBuild->subElemGen(number_str, MouseLeave, &ActMouseLeave, nvpBuild->g_top_node_view);
            nvpBuild->subElemGen(number_str, MouseRButtonDown, &ActMouseRButtonDown, nvpBuild->g_top_node_view); //Delete;
            nvpBuild->subElemGen(number_str, MouseMove_MouseLButtonDown, &Act1MouseDrag, nvpBuild->g_top_node_view);


            /////////////////////////////////////////////////////////////////
            std::string sub_string;
            sub_string = "_1";
            
            auto sub_elem_1 = nvpBuild->subElemGen(sub_string, MsgInit, &ActSubInit, elem);
            sub_elem_1->msgRoute(MsgInit);
            // same act as v1 except init position;
            nvpBuild->subElemGen(sub_string, MouseEnter, &ActMouseEnter, elem);
            nvpBuild->subElemGen(sub_string, MouseLeave, &ActMouseLeave, elem);
            nvpBuild->subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, elem); //Delete;
            nvpBuild->subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem);

            /////////////////////////////////////////////////////////////////
            sub_string = "_2";
            
            auto sub_elem_2 = nvpBuild->subElemGen(sub_string, MsgInit, &ActSubInit, elem);
            sub_elem_2->msgRoute(MsgInit);
            // same act as v1 except init position;
            nvpBuild->subElemGen(sub_string, MouseEnter, &ActMouseEnter, elem);
            nvpBuild->subElemGen(sub_string, MouseLeave, &ActMouseLeave, elem);
            nvpBuild->subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, elem); //Delete;
            nvpBuild->subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem);

            /////////////////////////////////////////////////////////////////
            sub_string = "_3";
            
            auto sub_elem_3 = nvpBuild->subElemGen(sub_string, MsgInit, &ActSubInit, elem);
            sub_elem_3->msgRoute(MsgInit);
            // same act as v1 except init position;
            nvpBuild->subElemGen(sub_string, MouseEnter, &ActMouseEnter, elem);
            nvpBuild->subElemGen(sub_string, MouseLeave, &ActMouseLeave, elem);
            nvpBuild->subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, elem); //Delete;
            nvpBuild->subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, elem);

            /////////////////////////////////////////////////////////////////
            sub_string = "_1";

            auto sub_elem_1_1 = nvpBuild->subElemGen(sub_string, MsgInit, &Act2SubInit, sub_elem_1);
            sub_elem_1_1->msgRoute(MsgInit);
            // same act as v1 except init position;
            nvpBuild->subElemGen(sub_string, MouseEnter, &ActMouseEnter, sub_elem_1);
            nvpBuild->subElemGen(sub_string, MouseLeave, &ActMouseLeave, sub_elem_1);
            nvpBuild->subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_elem_1); //Delete;
            nvpBuild->subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_elem_1);

            /////////////////////////////////////////////////////////////////
            sub_string = "_1";

            elem = nvpBuild->subElemGen(sub_string, MsgInit, &Act3SubInit, sub_elem_1_1);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            nvpBuild->subElemGen(sub_string, MouseEnter, &ActMouseEnter, sub_elem_1_1);
            nvpBuild->subElemGen(sub_string, MouseLeave, &ActMouseLeave, sub_elem_1_1);
            nvpBuild->subElemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_elem_1_1); //Delete;
            nvpBuild->subElemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_elem_1_1);
        }
    }
}Act3MouseLButtonDown;


ELEM_GEN(v1, MsgInit, ActInit)
ELEM_GEN(v1, MouseEnter, ActMouseEnter)
ELEM_GEN(v1, MouseLeave, ActMouseLeave)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseEnter, Act2MouseEnter)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)

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
        auto draw = new NvpFillOneRect;
        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, 0, &col, Draw, nullptr, draw);

        auto font = new NvpDrawTextOneLineOneColor;
        font->str_to_draw = "  Menu  |  Stat";
        font->start_pt.y = 13;
        NvpColor font_col = { 230, 230, 230 };
        nvpDraw->Record(base, 1, &font_col, Draw, 0, font);
    }
}StatInit;

class StatMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 200, 200, 255 };
        nvpDraw->Record(base, 0, &col, Draw);
    }
}StatMouseMove;

class StatMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, 0, &col, Draw);
    }
}StatMouseLeave;

ELEM_GEN_FULL(top_menu_stat, MsgInit, StatInit, nvpBuild->g_top_layout)
ELEM_GEN_FULL(top_menu_stat, MouseMove, StatMouseMove, nvpBuild->g_top_layout)
ELEM_GEN_FULL(top_menu_stat, MouseLeave, StatMouseLeave, nvpBuild->g_top_layout)
/////////////////////////////////////////////////////////////////////

class NodeViewInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto draw = new NvpFillOneRect;
        NvpColor col = { 40, 40, 45 };

        nvpDraw->Record(base, 0, &col, Draw, 0, draw);
    }
}NodeViewInit;

ELEM_GEN_FULL(top_node_view, MsgInit, NodeViewInit, nvpBuild->g_top_layout)
