#include "base/base.h"
#include "base/layout.h"
#include "port-win/direct2d/d2d_common.h"

class ActInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        BaseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;

        auto draw = new NvpFrameOneRect;
        NvpColor col = { 255, 0, 0 };
        nvpDraw->Record(base, &col, Draw, &rect, draw);
    }
}ActInit;

class ActMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 200, 200, 200 };
        nvpDraw->Record(base, &col, Draw);
    }
}ActMouseMove;

class ActMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 150, 150, 150 };
        nvpDraw->Record(base, &col, Draw);
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

        auto draw = new NvpFrameOneRect;
        NvpColor col = { 0, 255, 0 };
        nvpDraw->Record(base, &col, Draw, &rect, draw);
    }
}Act2Init;

class Act2MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 0, 200, 200 };
        nvpDraw->Record(base, &col, Draw);
    }
}Act2MouseMove;

class Act2MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 0, 150, 150 };
        nvpDraw->Record(base, &col, Draw);
    }
}Act2MouseLeave;
/////////////////////////////////////////////////////////////////////////

class Act1MouseLButtonDown : public BaseAction
{
public:
    static mousePt last_pt;
    static bool can_drag;
    virtual void realAction(BaseElement* base) override
    {
        can_drag = true;
        last_pt = local_pt;
    }
}Act1MouseLButtonDown;
mousePt Act1MouseLButtonDown::last_pt;
bool Act1MouseLButtonDown::can_drag = false;

class Act1MouseLButtonUp : public Act1MouseLButtonDown
{
public:
    virtual void realAction(BaseElement* base) override
    {
        can_drag = false;
    }
}Act1MouseLButtonUp;

class Act2MouseLButtonDown : public BaseAction
{
public:
    static mousePt last_pt;
    static bool can_drag;
    virtual void realAction(BaseElement* base) override
    {
        can_drag = true;
        last_pt = local_pt;
    }
}Act2MouseLButtonDown;
mousePt Act2MouseLButtonDown::last_pt;
bool Act2MouseLButtonDown::can_drag = false;

class Act2MouseLButtonUp : public Act2MouseLButtonDown
{
public:
    virtual void realAction(BaseElement* base) override
    {
        can_drag = false;
    }
}Act2MouseLButtonUp;

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
        
        NvpColor col = { 150, 150, 150 };
        nvpDraw->Record(next->body.elem, &col, NoneDraw, rect);
        subLevelRecord(next->body.elem);
    }
}

class Act1MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        if (Act1MouseLButtonDown.can_drag)
        {
            mousePt last_pt = Act1MouseLButtonDown.last_pt;
            ptSize sub_x = local_pt.x - last_pt.x;
            ptSize sub_y = local_pt.y - last_pt.y;
            const BaseRect* old_rect = base->getRectRefUp();

            //draw->fillRect(*old_rect, RGB(0, 0, 0), Begin);

            BaseRect new_rect;
            new_rect.left = old_rect->left + sub_x;
            new_rect.right = old_rect->right + sub_x;
            new_rect.top = old_rect->top + sub_y;
            new_rect.bottom = old_rect->bottom + sub_y;
            
            nvpDraw->Record(base, 0, NoneDraw, &new_rect);

            subLevelRecord(base);
            
            NvpColor col = { 200, 200, 200 };
            nvpDraw->Record(base, &col, Draw);

        }
    }
}Act1MouseDrag;

class Act2MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        if (Act2MouseLButtonDown.can_drag)
        {
            mousePt last_pt = Act2MouseLButtonDown.last_pt;
            ptSize sub_x = local_pt.x - last_pt.x;
            ptSize sub_y = local_pt.y - last_pt.y;
            const BaseRect* old_rect = base->getRectRefUp();

            //draw->fillRect(*old_rect, RGB(0, 0, 0), Begin);

            BaseRect new_rect;
            new_rect.left = old_rect->left + sub_x;
            new_rect.right = old_rect->right + sub_x;
            new_rect.top = old_rect->top + sub_y;
            new_rect.bottom = old_rect->bottom + sub_y;

            NvpColor col = { 0, 200, 200 };
            nvpDraw->Record(base, &col, Draw, &new_rect);
        }
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

        auto draw = new NvpFrameFiveRect(40);
        nvpDraw->Record(base, colors, Draw, &rect, draw);
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

        auto draw = (NvpFrameFiveRect*)base->getSelfDraw();
        draw->setPersent(36);

        nvpDraw->Record(base, colors, Draw);
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

        auto draw = (NvpFrameFiveRect*)base->getSelfDraw();
        draw->setPersent(40);

        nvpDraw->Record(base, colors, Draw);
    }
}Act3MouseLeave;

//global opt speed control;
static RecordOption opt[] = { NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw,
    NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, NoneDraw, Draw };

class ActRandomInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        static long seed = 0; ++seed;
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 100;
        srand(seed);
        int temp = rand();
        int move_x = rand() / 100;
        int move_y = rand() / 100;
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        NvpColor col = { 100, 100, 100 };
        auto draw = new NvpFrameOneRect;
        nvpDraw->Record(base, &col, opt[offset], &rect, draw);
        if (offset >= ARRAYSIZE(opt) - 1)
        {
            offset = -1;
        }
    }
}ActRandomInit;

class ActSubInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        static long seed = 0; ++seed;
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 40;
        srand(seed);
        int temp = rand();
        int move_x = rand() / 500;
        int move_y = rand() / 500;
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        NvpColor col = { 100, 100, 100 };
        auto draw = new NvpFrameOneRect;
        nvpDraw->Record(base, &col, opt[offset], &rect, draw);
        if (offset >= ARRAYSIZE(opt) - 1)
        {
            offset = -1;
        }
    }
}ActSubInit;

class Act2SubInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        static long seed = 0; ++seed;
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 20;
        srand(seed);
        int temp = rand();
        int move_x = rand() / 2000;
        int move_y = rand() / 2000;
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;
        
        NvpColor col = { 100, 100, 100 };
        auto draw = new NvpFrameOneRect;
        nvpDraw->Record(base, &col, opt[offset], &rect, draw);
        if (offset >= ARRAYSIZE(opt) - 1)
        {
            offset = -1;
        }
    }
}Act2SubInit;

class Act3SubInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        static long seed = 0; ++seed;
        BaseRect rect;
        rect.left = rect.top = 0;
        rect.right = rect.bottom = 10;
        srand(seed);
        int temp = rand();
        int move_x = rand() / 5000;
        int move_y = rand() / 5000;
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;

        static int offset = -1; ++offset;

        NvpColor col = { 100, 100, 100 };
        auto draw = new NvpFrameOneRect;
        nvpDraw->Record(base, &col, opt[offset], &rect, draw);
        if (offset >= ARRAYSIZE(opt) - 1)
        {
            offset = -1;
        }
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
        nvpDraw->Record(next->body.elem, &col, Draw);
        subLevelRemove(next->body.elem);
        elemDel(next->body.elem->getSelfName(), next->body.elem->getSelfLevel());
    }
}

class ActMouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 0, 0, 0 };
        nvpDraw->Record(base, &col);

        subLevelRemove(base);
        
        elemDel(base->getSelfName(), base->getSelfLevel());
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

            auto iter = g_top_node_view->begin();
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
                nvpDraw->Record(shape, &col, opt[offset]);
                if (offset >= ARRAYSIZE(opt) - 1)
                {
                    offset = -1;
                }

                elemDel(number_str, g_top_node_view);
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

            BaseElement* elem = elemGen(number_str, MsgInit, &ActRandomInit);
            mousePt pt;
            elem->msgRoute(MsgInit, &pt);
            // same act as v1 except init position;
            elemGen(number_str, MouseMove, &ActMouseMove);
            elemGen(number_str, MouseLeave, &ActMouseLeave);
            elemGen(number_str, MouseLButtonDown, &Act1MouseLButtonDown);
            elemGen(number_str, MouseLButtonUp, &Act1MouseLButtonUp);
            elemGen(number_str, MouseRButtonDown, &ActMouseRButtonDown); //Delete;
            elemGen(number_str, MouseMove_MouseLButtonDown, &Act1MouseDrag);


            /////////////////////////////////////////////////////////////////
            auto sub_level = subLevelGen(elem);
            std::string sub_string;
            sub_string = "_1";
            
            elem = elemGen(sub_string, MsgInit, &ActSubInit, sub_level);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            elemGen(sub_string, MouseMove, &ActMouseMove, sub_level);
            elemGen(sub_string, MouseLeave, &ActMouseLeave, sub_level);
            elemGen(sub_string, MouseLButtonDown, &Act1MouseLButtonDown, sub_level);
            elemGen(sub_string, MouseLButtonUp, &Act1MouseLButtonUp, sub_level);
            elemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_level); //Delete;
            elemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_level);

            /////////////////////////////////////////////////////////////////
            sub_string = "_2";
            
            elem = elemGen(sub_string, MsgInit, &ActSubInit, sub_level);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            elemGen(sub_string, MouseMove, &ActMouseMove, sub_level);
            elemGen(sub_string, MouseLeave, &ActMouseLeave, sub_level);
            elemGen(sub_string, MouseLButtonDown, &Act1MouseLButtonDown, sub_level);
            elemGen(sub_string, MouseLButtonUp, &Act1MouseLButtonUp, sub_level);
            elemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_level); //Delete;
            elemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_level);

            /////////////////////////////////////////////////////////////////
            sub_string = "_3";
            
            elem = elemGen(sub_string, MsgInit, &ActSubInit, sub_level);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            elemGen(sub_string, MouseMove, &ActMouseMove, sub_level);
            elemGen(sub_string, MouseLeave, &ActMouseLeave, sub_level);
            elemGen(sub_string, MouseLButtonDown, &Act1MouseLButtonDown, sub_level);
            elemGen(sub_string, MouseLButtonUp, &Act1MouseLButtonUp, sub_level);
            elemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_level); //Delete;
            elemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_level);

            /////////////////////////////////////////////////////////////////
            sub_level = subLevelGen(elem);
            sub_string = "_1";

            elem = elemGen(sub_string, MsgInit, &Act2SubInit, sub_level);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            elemGen(sub_string, MouseMove, &ActMouseMove, sub_level);
            elemGen(sub_string, MouseLeave, &ActMouseLeave, sub_level);
            elemGen(sub_string, MouseLButtonDown, &Act1MouseLButtonDown, sub_level);
            elemGen(sub_string, MouseLButtonUp, &Act1MouseLButtonUp, sub_level);
            elemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_level); //Delete;
            elemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_level);

            /////////////////////////////////////////////////////////////////
            sub_level = subLevelGen(elem);
            sub_string = "_1";

            elem = elemGen(sub_string, MsgInit, &Act3SubInit, sub_level);
            elem->msgRoute(MsgInit);
            // same act as v1 except init position;
            elemGen(sub_string, MouseMove, &ActMouseMove, sub_level);
            elemGen(sub_string, MouseLeave, &ActMouseLeave, sub_level);
            elemGen(sub_string, MouseLButtonDown, &Act1MouseLButtonDown, sub_level);
            elemGen(sub_string, MouseLButtonUp, &Act1MouseLButtonUp, sub_level);
            elemGen(sub_string, MouseRButtonDown, &ActMouseRButtonDown, sub_level); //Delete;
            elemGen(sub_string, MouseMove_MouseLButtonDown, &Act1MouseDrag, sub_level);
        }
    }
}Act3MouseLButtonDown;

ELEM_GEN(init, MsgNone, ActInit) //init first;

ELEM_GEN(v1, MsgInit, ActInit)
ELEM_GEN(v1, MouseMove, ActMouseMove)
ELEM_GEN(v1, MouseLeave, ActMouseLeave)
ELEM_GEN(v1, MouseLButtonDown, Act1MouseLButtonDown)
ELEM_GEN(v1, MouseLButtonUp, Act1MouseLButtonUp)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseMove, Act2MouseMove)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseLButtonDown, Act2MouseLButtonDown)
ELEM_GEN(v2, MouseLButtonUp, Act2MouseLButtonUp);
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)

ELEM_GEN(v3, MsgInit, Act3Init) //v3 is Add Button;
ELEM_GEN(v3, MouseMove, Act3MouseMove)
ELEM_GEN(v3, MouseLeave, Act3MouseLeave)
ELEM_GEN(v3, MouseLButtonDown, Act3MouseLButtonDown) //generate
ELEM_GEN(v3, MouseRButtonDown, Act3MouseRButtonDown) //delete

/////////////////////////////////////////////////////////////////////
class MenuInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto iter = g_top_menu_bar->begin();
        auto elem = (*iter)->head->up_elem;
        auto rect = elem->getSelfLayout()->ref_up;

        auto draw = new NvpFillOneRect;
        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, &col, Draw, &rect, draw);
    }
}MenuInit;

class MenuMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto iter = g_top_status_bar->begin();
        auto elem = (*iter)->head->up_elem;
        auto rect = elem->getSelfLayout()->ref_up;
        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;
        rect.left = rect.top = 0;
        rect.right = width;
        rect.bottom = height;

        NvpColor col = { 200, 200, 255 };
        nvpDraw->Record(base, &col, Draw, &rect);
    }
}MenuMouseMove;

class MenuMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto iter = g_top_status_bar->begin();
        auto elem = (*iter)->head->up_elem;
        auto rect = elem->getSelfLayout()->ref_up;
        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;
        rect.left = rect.top = 0;
        rect.right = width;
        rect.bottom = height;

        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, &col, Draw, &rect);
    }
}MenuMouseLeave;

ELEM_GEN_FULL(menu_bar, MsgInit, MenuInit, g_top_menu_bar)
ELEM_GEN_FULL(menu_bar, MouseMove, MenuMouseMove, g_top_menu_bar)
ELEM_GEN_FULL(menu_bar, MouseLeave, MenuMouseLeave, g_top_menu_bar)
/////////////////////////////////////////////////////////////////////

class StatInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto iter = g_top_status_bar->begin();
        auto elem = (*iter)->head->up_elem;
        auto rect = elem->getSelfLayout()->ref_up;
        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;
        rect.left = rect.top = 0;
        rect.right = width;
        rect.bottom = height;

        auto draw = new NvpFillOneRect;
        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, &col, Draw, &rect, draw);
    }
}StatInit;

class StatMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto iter = g_top_status_bar->begin();
        auto elem = (*iter)->head->up_elem;
        auto rect = elem->getSelfLayout()->ref_up;
        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;
        rect.left = rect.top = 0;
        rect.right = width;
        rect.bottom = height;

        NvpColor col = { 200, 200, 255 };
        nvpDraw->Record(base, &col, Draw, &rect);
    }
}StatMouseMove;

class StatMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto iter = g_top_status_bar->begin();
        auto elem = (*iter)->head->up_elem;
        auto rect = elem->getSelfLayout()->ref_up;
        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;
        rect.left = rect.top = 0;
        rect.right = width;
        rect.bottom = height;

        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, &col, Draw, &rect);
    }
}StatMouseLeave;

ELEM_GEN_FULL(status_bar, MsgInit, StatInit, g_top_status_bar)
ELEM_GEN_FULL(status_bar, MouseMove, StatMouseMove, g_top_status_bar)
ELEM_GEN_FULL(status_bar, MouseLeave, StatMouseLeave, g_top_status_bar)
/////////////////////////////////////////////////////////////////////
