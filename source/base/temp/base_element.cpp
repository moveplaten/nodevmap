/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base.h"
#include "draw/nvp_draw.h"

#if 0
#ifdef TEMP_TEST_0
void NvpBaseObj::linkMsg(MsgBaseType msg_type, BaseAction* msg_act)
{
    if (linked_msg_size == 0)
    {
        linked_msg.linked_msg_type = msg_type;
        linked_msg.linked_msg_act = msg_act;
        ++linked_msg_size;
    }
    else
    {
        LinkedMsg* last_linked_msg = (LinkedMsg*)malloc(sizeof(LinkedMsg));
        last_linked_msg->linked_msg_type = msg_type;
        last_linked_msg->linked_msg_act = msg_act;
        last_linked_msg->next_linked_msg = nullptr;
        LinkedMsg** next_linked_msg = &linked_msg.next_linked_msg;

        for (msgTypeSize i = 0; i < linked_msg_size - 1; ++i)
        {
            next_linked_msg = &((*next_linked_msg)->next_linked_msg);
        }

        *next_linked_msg = last_linked_msg;
        ++linked_msg_size;
    }
}

void NvpBaseObj::msgRoute(MsgBaseType msg_type, NvpPoint* pt)
{
    if (self_id > BaseMessage::g_store_shapes->getTotalMax())
    {
        return;
    }

    if (linked_msg.linked_msg_type == msg_type)
    {
        linked_msg.linked_msg_act->mousePtToLocal(this, pt);
        linked_msg.linked_msg_act->realAction(this);
    }
    else if (linked_msg.next_linked_msg)
    {
        LinkedMsg* next_linked_msg = linked_msg.next_linked_msg;
        BaseAction* fetch_msg_act = nullptr;

        for (msgTypeSize i = 0; i < linked_msg_size - 1; ++i)
        {
            if (next_linked_msg->linked_msg_type == msg_type)
            {
                fetch_msg_act = next_linked_msg->linked_msg_act;
                break;
            }
            else
            {
                next_linked_msg = next_linked_msg->next_linked_msg;
            }
        }

        if (fetch_msg_act)
        {
            fetch_msg_act->mousePtToLocal(this, pt);
            fetch_msg_act->realAction(this);
        }
    }
}
#endif // TEMP_TEST_0


#ifndef TEMP_TEST_0
void NvpBaseObj::linkMsg(MsgBaseType msg_type, BaseAction* msg_act)
{
    auto ret = msg_act_map.find(msg_type);
    if (ret == msg_act_map.end())
    {
        msg_act_map.insert({ msg_type, msg_act });
    }
    else
    {
        return;
    }
}

void NvpBaseObj::msgRoute(MsgBaseType msg_type, NvpPoint* pt)
{
    auto ret = msg_act_map.find(msg_type);
    if (ret == msg_act_map.end())
    {
        return;
    }
    else
    {
        auto temp = *ret;
        BaseAction* act = temp.second;
        
        if (pt)
        {
            act->mousePtToLocal(this, pt);
        }
        
        act->prepareDraw(this, msg_type);
        
        act->realAction(this);
    }
}
#endif // !TEMP_TEST_0


void BaseAction::prepareDraw(NvpBaseObj* base, MsgBaseType type)
{
    if (type == MsgInit)
    {
        auto draw = new NvpDraw(base);
        base->self_draw = draw;
    }
    
    this->nvp_draw = base->self_draw;
}

void BaseAction::mousePtToLocal(NvpBaseObj* base, NvpPoint* pt)
{
    world_pt = *pt;

    local_pt.x = pt->x - base->getRectRefTop().left;
    local_pt.y = pt->y - base->getRectRefTop().top;
    //char temp[100];
    //sprintf(temp, "\nworld_x = %f, world_y = %f\nlocal_x = %f, local_y = %f\n\n", world_pt.x, world_pt.y, local_pt.x, local_pt.y);
    //OutputDebugStringA(temp);
}
#endif

NvpBaseObj::NvpBaseObj(
    NvpLayout& layout, NvpEvent* event, const bool top)
    :
    self_event(event), self_layout(layout), self_top(top)
{

}

NvpBaseObj::~NvpBaseObj()
{
    if (self_draw)
    {
        delete self_draw;
    }
}
