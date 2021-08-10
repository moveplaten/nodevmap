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
    }
}Act2MouseDrag;


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

ELEM_GEN(v3, MsgNone, ActInit)
ELEM_GEN(v4, MsgNone, ActInit)
