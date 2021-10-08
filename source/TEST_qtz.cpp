#include "base/base.h"
#include "draw/draw.h"

class Act1Init : public BaseAction
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
}Act1Init;

class Act1MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 200, 200, 200 };
        nvpDraw->Record(base, &col, Draw);
    }
}Act1MouseMove;

class Act1MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 150, 150, 150 };
        nvpDraw->Record(base, &col, Draw);
    }
}Act1MouseLeave;

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

            NvpColor col = { 200, 200, 200 };
            nvpDraw->Record(base, &col, Draw, &new_rect);
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

ELEM_GEN(init, MsgNone, Act1Init) //init first;

ELEM_GEN(v1, MsgInit, Act1Init)
ELEM_GEN(v1, MouseMove, Act1MouseMove)
ELEM_GEN(v1, MouseLeave, Act1MouseLeave)
ELEM_GEN(v1, MouseLButtonDown, Act1MouseLButtonDown)
ELEM_GEN(v1, MouseLButtonUp, Act1MouseLButtonUp)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseMove, Act2MouseMove)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseLButtonDown, Act2MouseLButtonDown)
ELEM_GEN(v2, MouseLButtonUp, Act2MouseLButtonUp);
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)
