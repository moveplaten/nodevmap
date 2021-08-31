#include "base/base.h"
#include "port-win/direct2d/d2d_common.h"

#define draw D2dUtil::g_d2dutil

class ActInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        elemGen("v0", MsgNone, nullptr);
        baseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;
        base->setRect(&rect);

        draw->fillRect(rect, RGB(255, 0, 0));
    }
}ActInit;

class ActMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());

        draw->fillRect(rect, RGB(200, 200, 200));
    }
}ActMouseMove;

class ActMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());

        draw->fillRect(rect, RGB(150, 150, 150));
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
        baseRect rect;
        rect.left = 50;
        rect.right = 90;
        rect.top = 10;
        rect.bottom = 50;
        base->setRect(&rect);

        draw->fillRect(rect, RGB(0, 255, 0));
    }
}Act2Init;

class Act2MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());

        draw->fillRect(rect, RGB(0, 200, 200));
    }
}Act2MouseMove;

class Act2MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());

        draw->fillRect(rect, RGB(0, 150, 150));
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

class Act1MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        if (Act1MouseLButtonDown.can_drag)
        {
            mousePt last_pt = Act1MouseLButtonDown.last_pt;
            ptSize sub_x = local_pt.x - last_pt.x;
            ptSize sub_y = local_pt.y - last_pt.y;
            const baseRect* old_rect = base->getRect();

            draw->fillRect(*old_rect, RGB(0, 0, 0), Begin);

            baseRect new_rect;
            new_rect.left = old_rect->left + sub_x;
            new_rect.right = old_rect->right + sub_x;
            new_rect.top = old_rect->top + sub_y;
            new_rect.bottom = old_rect->bottom + sub_y;
            base->setRect(&new_rect);

            draw->fillRect(new_rect, RGB(200, 200, 200), End);
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
            const baseRect* old_rect = base->getRect();

            draw->fillRect(*old_rect, RGB(0, 0, 0), Begin);

            baseRect new_rect;
            new_rect.left = old_rect->left + sub_x;
            new_rect.right = old_rect->right + sub_x;
            new_rect.top = old_rect->top + sub_y;
            new_rect.bottom = old_rect->bottom + sub_y;
            base->setRect(&new_rect);

            draw->fillRect(new_rect, RGB(0, 200, 200), End);
        }
    }
}Act2MouseDrag;

////////////////////////////////////////////////////////////////////////
class Act3Init : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect;
        rect.left = 110;
        rect.right = 150;
        rect.top = 10;
        rect.bottom = 50;
        base->setRect(&rect);
    
        COLORREF hColor = RGB(0, 200, 200);
        COLORREF hColor2 = RGB(200, 200, 200);
 
        draw->fillRect(rect, hColor, Begin);

        int width = 8;
        rect.left = 110; rect.right = 130 - width / 2;
        rect.top = 10; rect.bottom = 30 - width / 2;
        draw->fillRect(rect, hColor2, None);

        rect.left = 130 + width / 2; rect.right = 150;
        rect.top = 10; rect.bottom = 30 - width / 2;
        draw->fillRect(rect, hColor2, None);

        rect.left = 110; rect.right = 130 - width / 2;
        rect.top = 30 + width / 2; rect.bottom = 50;
        draw->fillRect(rect, hColor2, None);

        rect.left = 130 + width / 2; rect.right = 150;
        rect.top = 30 + width / 2; rect.bottom = 50;
        draw->fillRect(rect, hColor2, End);
    }
}Act3Init;

class Act3MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect;
        rect.left = 110;
        rect.right = 150;
        rect.top = 10;
        rect.bottom = 50;
        base->setRect(&rect);

        COLORREF hColor = RGB(0, 200, 200);
        COLORREF hColor2 = RGB(200, 200, 200);

        draw->fillRect(rect, hColor, Begin);

        int width = 12;
        rect.left = 110; rect.right = 130 - width / 2;
        rect.top = 10; rect.bottom = 30 - width / 2;
        draw->fillRect(rect, hColor2, None);

        rect.left = 130 + width / 2; rect.right = 150;
        rect.top = 10; rect.bottom = 30 - width / 2;
        draw->fillRect(rect, hColor2, None);

        rect.left = 110; rect.right = 130 - width / 2;
        rect.top = 30 + width / 2; rect.bottom = 50;
        draw->fillRect(rect, hColor2, None);

        rect.left = 130 + width / 2; rect.right = 150;
        rect.top = 30 + width / 2; rect.bottom = 50;
        draw->fillRect(rect, hColor2, End);
    }
}Act3MouseMove;

class Act3MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect;
        rect.left = 110;
        rect.right = 150;
        rect.top = 10;
        rect.bottom = 50;
        base->setRect(&rect);

        COLORREF hColor = RGB(0, 200, 200);
        COLORREF hColor2 = RGB(200, 200, 200);

        draw->fillRect(rect, hColor, Begin);

        int width = 8;
        rect.left = 110; rect.right = 130 - width / 2;
        rect.top = 10; rect.bottom = 30 - width / 2;
        draw->fillRect(rect, hColor2, None);

        rect.left = 130 + width / 2; rect.right = 150;
        rect.top = 10; rect.bottom = 30 - width / 2;
        draw->fillRect(rect, hColor2, None);

        rect.left = 110; rect.right = 130 - width / 2;
        rect.top = 30 + width / 2; rect.bottom = 50;
        draw->fillRect(rect, hColor2, None);

        rect.left = 130 + width / 2; rect.right = 150;
        rect.top = 30 + width / 2; rect.bottom = 50;
        draw->fillRect(rect, hColor2, End);
    }
}Act3MouseLeave;

class ActRandomInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        static long seed = 0; ++seed;
        elemGen("v0", MsgNone, nullptr);
        baseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;
        srand(seed);
        int temp = rand();
        int move_x = rand() / 100;
        int move_y = rand() / 100;
        rect.left += move_x;
        rect.right += move_x;
        rect.top += move_y;
        rect.bottom += move_y;
        base->setRect(&rect);

        DrawOption opt[] = { Begin, None, None, None, None, None, 
            None, None, None, None, None, None, None, None, End };
        static int offset = -1; ++offset;

        draw->fillRect(rect, RGB(255, 0, 0), opt[offset]);
        if (offset >= ARRAYSIZE(opt) - 1)
        {
            offset = -1;
        }
    }
}ActRandomInit;

class ActMouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());

        draw->fillRect(rect, RGB(0, 0, 0));

        elemDel(base->getSelfName());
    }
}ActMouseRButtonDown;

#define MAX_TEST_ELEM 1000

class Act3MouseRButtonDown : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        for (elemIDSize add_number = 0; add_number < MAX_TEST_ELEM; ++add_number)
        {
            std::string number_str("add_");
            std::string add_str = std::to_string(add_number);
            number_str = number_str + add_str;

            auto ret = ElementGenerator::g_elements_map->find(number_str);
            if (ret == ElementGenerator::g_elements_map->end())
            {
                continue;
            }
            else
            {
                auto temp = *ret;
                auto shape = temp.second;

                DrawOption opt[] = { Begin, None, None, None, None, None,
                    None, None, None, None, None, None, None, None, End };
                static int offset = -1; ++offset;

                draw->fillRect(*(shape->getRect()), RGB(0, 0, 0), opt[offset]);
                if (offset >= ARRAYSIZE(opt) - 1)
                {
                    offset = -1;
                }

                elemDel(number_str);
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
        }
    }
}Act3MouseLButtonDown;


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

ELEM_GEN(v4, MsgNone, ActInit)