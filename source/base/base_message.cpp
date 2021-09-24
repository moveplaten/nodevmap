#include "base.h"
#include <stdio.h>

HWND BaseMessage::g_hwnd = nullptr;
BaseMessage* const baseMsg = nullptr;

void BaseMessage::hitTest(MsgBaseType msg_type, mousePt* pt)
{
    if (msg_type == MsgInit)
    {
        initAll(g_top_layout);
    }
    else if (msg_type == MouseEnter)
    {
        BaseElement::g_hitTest_id->msgRoute(MouseEnter, pt);
        char temp[100];
        sprintf(temp, "%lld", BaseElement::g_hitTest_id->getSelfID());
        OutputDebugStringA(temp);
        OutputDebugStringA("Enter\n");
    }
    else if (msg_type == MouseLeave)
    {
        BaseElement::g_before_leave_id->msgRoute(MouseLeave, pt);
        hitTest(MouseEnter, pt);
    }
    else if (msg_type == MouseMove_MouseLButtonDown)
    {
        BaseElement::g_mouse_drag_id->msgRoute(MouseMove_MouseLButtonDown, pt);
    }
    else if (msg_type == MouseLButtonDown)
    {
        BaseElement::g_mouse_drag_id = BaseElement::g_hitTest_id;
        BaseElement::g_hitTest_id->msgRoute(MouseLButtonDown, pt);
    }
    else
    {
        BaseElement* fetch = inRange(pt, g_top_layout);

        if (fetch)
        {
            fetch->msgRoute(msg_type, pt);
        }
    }

    if (msg_type == MouseMove)
    {
        if (checkLeave() && BaseElement::g_before_leave_id != nullptr)
        {
            char temp[100];
            //sprintf(temp, "%lld", BaseElement::g_before_leave_id->getSelfID());
            //OutputDebugStringA(temp);
            //OutputDebugStringA("LEAVE\n");
            hitTest(MouseLeave, pt);
        }
    }
}

void BaseMessage::initAll(NvpLevel* level)
{
    if (!level)
    {
        return;
    }

    size_t size = level->size();
    auto iter = level->begin();

    for (size_t i = 0; i < size - 1; ++i)
    {
        auto next = *(++iter);
        next->body.elem->msgRoute(MsgInit);
        initAll(next->body.sub);
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

BaseElement* BaseMessage::inRange(mousePt* pt, NvpLevel* level)
{
    ptSize pt_x = pt->x;
    ptSize pt_y = pt->y;
    size_t hit = -1;
    static BaseElement* hit_elem = nullptr;

    if (level)
    {
        auto head = *(level->begin());
        auto up_elem = head->head->up_elem;
        if (up_elem)
        {
            auto up_rect = up_elem->getRect();
            pt_x -= up_rect->left;
            pt_y -= up_rect->top;
        }
        
        auto size = level->size();
        auto content = *level;
        auto iter = content.begin();
        ++iter;
        for (size_t i = 0; i < size - 1; ++i)
        {
            auto rect = &(*iter)->body.rect;
            if (rect->left <= pt_x && rect->right > pt_x &&
            rect->top <= pt_y && rect->bottom > pt_y)
            {
                hit = i;
                hit_elem = (*iter)->body.elem;
            }
            else
            {
                ++iter;
            }
        }
        if (hit == -1)
        {
            BaseElement::g_hitTest_id = hit_elem;
            return hit_elem;
        }
        else if ((*iter)->body.sub)
        {
            mousePt pt_new;
            pt_new.x = pt_x;
            pt_new.y = pt_y;
            inRange(&pt_new, (*iter)->body.sub);
        }
        else
        {
            BaseElement::g_hitTest_id = (*iter)->body.elem;
            hit_elem = (*iter)->body.elem;
            return hit_elem;
        }
    }
    return hit_elem;
}
