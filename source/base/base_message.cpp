#include "base.h"
#include <stdio.h>

HWND BaseMessage::g_hwnd = nullptr;
ElemStorage* BaseMessage::g_store_shapes = nullptr;

void BaseMessage::hitTest(MsgBaseType msg_type, mousePt* pt)
{
    if (msg_type == MsgInit)
    {
        initAll(pt);
    }
    else if (msg_type == MouseLeave)
    {
        BaseElement::g_before_leave_id->msgRoute(MouseLeave, pt);
    }
    else if (msg_type == MouseMove_MouseLButtonDown && BaseElement::g_hitTest_id)
    {
        if (BaseElement::g_mouse_snap_id)
        BaseElement::g_mouse_snap_id->msgRoute(MouseMove_MouseLButtonDown, pt);
    }
    else if (msg_type == MouseLButtonDown && BaseElement::g_hitTest_id)
    {
        BaseElement::g_mouse_snap_id = BaseElement::g_hitTest_id;
        BaseElement::g_hitTest_id->msgRoute(MouseLButtonDown, pt);
    }
    else
    {
        BaseElement* fetch = inRange(pt);

        if (fetch)
        {
            fetch->msgRoute(msg_type, pt);
        }
    }

    if (msg_type == MouseMove)
    {
        if (checkLeave() && BaseElement::g_before_leave_id != nullptr)
        {
            char temp[20];
            sprintf(temp, "%d", BaseElement::g_before_leave_id->getSelfID());
            OutputDebugStringA(temp);
            OutputDebugStringA("LEAVE\n");
            hitTest(MouseLeave, pt);
        }
    }
}

void BaseMessage::initAll(mousePt* pt)
{
    for (elemIDSize i = 0; i < g_store_shapes->getTotalUsed(); ++i)
    {
        BaseShape* content = (BaseShape*)g_store_shapes->readOneElem(i);
        content->elem->msgRoute(MsgInit, pt);
    }
}

bool BaseMessage::checkLeave()
{
    static BaseElement* prev_hit = nullptr;
    BaseElement* now_hit = BaseElement::g_hitTest_id;
    size_t check_sub = prev_hit - now_hit;

    if (check_sub == 0)
    {
        return false;
    }
    else
    {
        BaseElement::g_before_leave_id = prev_hit;
        prev_hit = now_hit;
        return true;
    }
}

BaseElement* BaseMessage::inRange(mousePt* pt)
{
    ptSize pt_x = pt->x;
    ptSize pt_y = pt->y;
    BaseShape* base_shapes = (BaseShape*)g_store_shapes->getContents();
    elemIDSize total = g_store_shapes->getTotalUsed();

    for (elemIDSize i = 0; i < total; ++i)
    {
        baseRect* rect = &base_shapes[i].rect;
        if (rect->left <= pt_x && rect->right > pt_x &&
            rect->top <= pt_y && rect->bottom > pt_y)
        {
            BaseElement::g_hitTest_id = base_shapes[i].elem;
            return base_shapes[i].elem;
        }
    }

    BaseElement::g_hitTest_id = nullptr;

    return nullptr;
}
