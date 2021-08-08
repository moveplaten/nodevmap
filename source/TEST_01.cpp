#include "base/base.h"

class ActInit : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        elemGen("v0", MsgNone, nullptr);
        baseRect rect;
        rect.left = rect.top = 10;
        rect.right = rect.bottom = 50;
        base->setRect(&rect);
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}ActInit;

class ActMouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(200, 200, 200));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}ActMouseMove;

class ActMouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(150, 150, 150));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
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
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(0, 255, 0));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2Init;

class Act2MouseMove : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(0, 200, 200));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2MouseMove;

class Act2MouseLeave : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        baseRect rect = *(base->getRect());
        HDC hdc = GetDC(g_hwnd);
        HBRUSH hColor = CreateSolidBrush(RGB(0, 150, 150));
        FillRect(hdc, &rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2MouseLeave;
/////////////////////////////////////////////////////////////////////////

static void mousePtToLocal(mousePt* pt, BaseElement* base)
{
    pt->x = pt->x - base->getRect()->left;
    pt->y = pt->y - base->getRect()->top;
}

class Act1MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        mousePtToLocal(&pt, base);
        static mousePt last_pt = pt;
        ptSize sub_x = pt.x - last_pt.x;
        ptSize sub_y = pt.y - last_pt.y;
        const baseRect* old_rect = base->getRect();

        HDC hdc = GetDC(g_hwnd);
        FillRect(hdc, old_rect, (HBRUSH)COLOR_WINDOW);

        baseRect new_rect;
        new_rect.left = old_rect->left + sub_x;
        new_rect.right = old_rect->right + sub_x;
        new_rect.top = old_rect->top + sub_y;
        new_rect.bottom = old_rect->bottom + sub_y;
        base->setRect(&new_rect);

        HBRUSH hColor = CreateSolidBrush(RGB(200, 200, 200));
        FillRect(hdc, &new_rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act1MouseDrag;

class Act2MouseDrag : public BaseAction
{
    virtual void realAction(BaseElement* base) override
    {
        mousePtToLocal(&pt, base);
        static mousePt last_pt = pt;
        ptSize sub_x = pt.x - last_pt.x;
        ptSize sub_y = pt.y - last_pt.y;
        const baseRect* old_rect = base->getRect();

        HDC hdc = GetDC(g_hwnd);
        FillRect(hdc, old_rect, (HBRUSH)COLOR_WINDOW);

        baseRect new_rect;
        new_rect.left = old_rect->left + sub_x;
        new_rect.right = old_rect->right + sub_x;
        new_rect.top = old_rect->top + sub_y;
        new_rect.bottom = old_rect->bottom + sub_y;
        base->setRect(&new_rect);

        HBRUSH hColor = CreateSolidBrush(RGB(0, 200, 200));
        FillRect(hdc, &new_rect, hColor);
        DeleteObject(hColor);
        ReleaseDC(g_hwnd, hdc);
    }
}Act2MouseDrag;


ELEM_GEN(v1, MsgInit, ActInit)
ELEM_GEN(v1, MouseMove, ActMouseMove)
ELEM_GEN(v1, MouseLeave, ActMouseLeave)
ELEM_GEN(v1, MouseLButtonDown, ActMouseLButtonDown)
ELEM_GEN(v1, MouseLButtonUp, ActMouseLButtonUp)
ELEM_GEN(v1, MouseMove_MouseLButtonDown, Act1MouseDrag)

ELEM_GEN(v2, MsgInit, Act2Init)
ELEM_GEN(v2, MouseMove, Act2MouseMove)
ELEM_GEN(v2, MouseLeave, Act2MouseLeave)
ELEM_GEN(v2, MouseMove_MouseLButtonDown, Act2MouseDrag)

ELEM_GEN(v3, MsgNone, ActInit)
ELEM_GEN(v4, MsgNone, ActInit)
