#include "base.h"
#include <stdio.h>

BaseMessage* const baseMsg = nullptr;
BaseElement* BaseMessage::g_now_hit_id = nullptr;
BaseElement* BaseMessage::g_mouse_drag_id = nullptr;
BaseElement* BaseMessage::g_before_leave_id = nullptr;
mousePt BaseMessage::g_last_downL_pt = { 0 };

void BaseMessage::hitTest(MsgBaseType msg_type, mousePt* pt)
{
    if (msg_type == MsgInit)
    {
        initAll(nvpBuild->g_top_layout);
    }
    else if (msg_type == MouseEnter)
    {
        g_now_hit_id->msgRoute(MouseEnter, pt);
        //char temp[100];
        //sprintf(temp, "%lld", g_now_hit_id->getSelfID());
        //OutputDebugStringA(temp);
        //OutputDebugStringA("Enter\n");
    }
    else if (msg_type == MouseLeave)
    {
        g_before_leave_id->msgRoute(MouseLeave, pt);
        hitTest(MouseEnter, pt);
    }
    else if (msg_type == MouseMove_MouseLButtonDown && g_mouse_drag_id)
    {
        g_mouse_drag_id->msgRoute(MouseMove_MouseLButtonDown, pt);
    }
    else if (msg_type == MouseLButtonDown && g_now_hit_id)
    {
        if (g_now_hit_id->canBeTop())
        {
            nvpBuild->moveToAllTop(g_now_hit_id);
        }
        
        mousePtToLocal(g_now_hit_id, pt);
        g_mouse_drag_id = g_now_hit_id;
        g_now_hit_id->msgRoute(MouseLButtonDown, pt);
    }
    else
    {
        BaseElement* fetch = inRange(pt, nvpBuild->g_top_layout);

        if (fetch)
        {
            fetch->msgRoute(msg_type, pt);
        }
    }

    if (msg_type == MouseMove)
    {
        auto temp_id = g_before_leave_id;
        if (checkLeave() && temp_id != nullptr)
        {
            char temp[100];
            //sprintf(temp, "%lld", g_before_leave_id->getSelfID());
            //OutputDebugStringA(temp);
            //OutputDebugStringA("LEAVE\n");
            hitTest(MouseLeave, pt);
        }
    }
}

void BaseMessage::mousePtToLocal(BaseElement* base, mousePt* pt)
{
    g_last_downL_pt.x = pt->x - base->getRectRefTop()->left;
    g_last_downL_pt.y = pt->y - base->getRectRefTop()->top;
}

void BaseMessage::initAll(BaseElement* base)
{
    for (;;)
    {
        if (base)
        {
            base->msgRoute(MsgInit);
        }

        auto sub = nvpBuild->getSubLast(base);

        if (sub)
        {
            initAll(sub);
        }

        base = nvpBuild->getNextReverse(base);
        
        if (!base)
        {
            break;
        }
    }
}

bool BaseMessage::checkLeave()
{
    static BaseElement* prev_hit = nullptr;
    BaseElement* now_hit = g_now_hit_id;
    size_t check_sub = prev_hit - now_hit;

    if (check_sub == 0)
    {
        g_before_leave_id = now_hit;
        return false;
    }
    else
    {
        g_before_leave_id = prev_hit;
        prev_hit = now_hit;
        return true;
    }
}

BaseElement* BaseMessage::inRange(mousePt* pt, BaseElement* base)
{
    for (;;)
    {
        bool hit = false;

        if (base)
        {
            auto rect = base->getRectRefTop();
            
            if (rect->left <= pt->x && rect->right > pt->x &&
                rect->top <= pt->y && rect->bottom > pt->y)
            {
                g_now_hit_id = base;
                hit = true;
            }
        }
        
        if (hit)
        {
            auto sub = nvpBuild->getSubLast(base);

            if (sub)
            {
                inRange(pt, sub);
            }

            break;
        }

        base = nvpBuild->getNextReverse(base);

        if (!base)
        {
            break;
        }
    }

    return g_now_hit_id;
}
