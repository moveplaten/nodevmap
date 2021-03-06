/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base.h"
#include "draw/draw.h"
#include <stdio.h>

BaseMessage* const baseMsg = nullptr;
NvpBaseObj* BaseMessage::g_now_hit_id = nullptr;
NvpBaseObj* BaseMessage::g_mouse_drag_id = nullptr;
NvpBaseObj* BaseMessage::g_before_leave_id = nullptr;
NvpPoint BaseMessage::g_last_downL_pt = { 0 };

void BaseMessage::hitTest(MsgBaseType msg_type, NvpPoint* pt)
{
    static uint8_t hit_test_depth = 0;

    ++hit_test_depth; //do not change it;

    if (msg_type == MsgInit)
    {
        initAll(NvpLayout::getTopLayout());
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
            NvpLayout::moveToAllTop(g_now_hit_id);
        }
        
        mousePtToLocal(g_now_hit_id, pt);
        g_mouse_drag_id = g_now_hit_id;
        g_now_hit_id->msgRoute(MouseLButtonDown, pt);
    }
    else
    {
        NvpBaseObj* fetch = inRange(pt, NvpLayout::getTopLayout());

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

    --hit_test_depth;

    if (hit_test_depth == 0)
    {
        NvpDrawPort::beginDraw();
    }
}

void BaseMessage::mousePtToLocal(NvpBaseObj* base, NvpPoint* pt)
{
    g_last_downL_pt.x = pt->x - base->getRectRefTop().left;
    g_last_downL_pt.y = pt->y - base->getRectRefTop().top;
}

void BaseMessage::initAll(NvpBaseObj* base)
{
    for (;;)
    {
        if (base)
        {
            base->msgRoute(MsgInit);
        }

        auto sub = NvpLayout::getSubFirst(base);

        if (sub)
        {
            initAll(sub);
        }

        base = NvpLayout::getNext(base);
        
        if (!base)
        {
            break;
        }
    }
}

bool BaseMessage::checkLeave()
{
    static NvpBaseObj* prev_hit = nullptr;
    NvpBaseObj* now_hit = g_now_hit_id;
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

NvpBaseObj* BaseMessage::inRange(NvpPoint* pt, NvpBaseObj* base)
{
    for (;;)
    {
        bool hit = false;

        if (base)
        {
            auto& rect = base->getRectRefTop();
            
            if (rect.left <= pt->x && rect.right > pt->x &&
                rect.top <= pt->y && rect.bottom > pt->y)
            {
                g_now_hit_id = base;
                hit = true;
            }
        }
        
        if (hit)
        {
            auto sub = NvpLayout::getSubLast(base);

            if (sub)
            {
                inRange(pt, sub);
            }

            break;
        }

        base = NvpLayout::getNextReverse(base);

        if (!base)
        {
            break;
        }
    }

    return g_now_hit_id;
}
