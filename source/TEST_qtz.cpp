#include "base/base.h"
#include "draw/draw.h"

class Act1Init : public BaseAction
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
}Act1Init;

class Act1MouseEnter : public BaseAction
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
}Act1MouseEnter;

class Act1MouseLeave : public BaseAction
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

        //NvpColor col = { 200, 200, 200 };
        nvpDraw->Record(base, 0, 0, Draw, &new_rect);
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


ELEM_GEN(v1, MsgInit, Act1Init)
ELEM_GEN(v1, MouseEnter, Act1MouseEnter)
ELEM_GEN(v1, MouseLeave, Act1MouseLeave)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseEnter, Act2MouseEnter)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)

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
