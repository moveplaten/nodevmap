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

        NvpColor col = { 200, 200, 200 };
        nvpDraw->Record(base, &col, Draw, &new_rect);
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

        NvpColor col = { 0, 200, 200 };
        nvpDraw->Record(base, &col, Draw, &new_rect);
    }
}Act2MouseDrag;

ELEM_GEN_FULL(init, MsgNone, Act1Init, (BaseElement*)nullptr) //init first;

ELEM_GEN(v1, MsgInit, Act1Init)
ELEM_GEN(v1, MouseMove, Act1MouseMove)
ELEM_GEN(v1, MouseLeave, Act1MouseLeave)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseMove, Act2MouseMove)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)

/////////////////////////////////////////////////////////////////////

class StatInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        auto draw = new NvpFillOneRect;
        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, &col, Draw, nullptr, draw);
    }
}StatInit;

class StatMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 200, 200, 255 };
        nvpDraw->Record(base, &col, Draw);
    }
}StatMouseMove;

class StatMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        NvpColor col = { 100, 200, 255 };
        nvpDraw->Record(base, &col, Draw);
    }
}StatMouseLeave;

ELEM_GEN_FULL(top_menu_stat, MsgInit, StatInit, nvpBuild->g_top_layout)
ELEM_GEN_FULL(top_menu_stat, MouseMove, StatMouseMove, nvpBuild->g_top_layout)
ELEM_GEN_FULL(top_menu_stat, MouseLeave, StatMouseLeave, nvpBuild->g_top_layout)
/////////////////////////////////////////////////////////////////////
