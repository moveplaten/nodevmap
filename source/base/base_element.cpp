#include "base.h"

#ifdef TEMP_TEST_0
void BaseElement::linkMsg(MsgBaseType msg_type, BaseAction* msg_act)
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

void BaseElement::msgRoute(MsgBaseType msg_type, mousePt* pt)
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
void BaseElement::linkMsg(MsgBaseType msg_type, BaseAction* msg_act)
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

void BaseElement::msgRoute(MsgBaseType msg_type, mousePt* pt)
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
        act->realAction(this);
    }
}
#endif // !TEMP_TEST_0


void BaseAction::mousePtToLocal(BaseElement* base, mousePt* pt)
{
    world_pt = *pt;

    local_pt.x = pt->x - base->getRectRefTop()->left;
    local_pt.y = pt->y - base->getRectRefTop()->top;
    //char temp[100];
    //sprintf(temp, "\nworld_x = %f, world_y = %f\nlocal_x = %f, local_y = %f\n\n", world_pt.x, world_pt.y, local_pt.x, local_pt.y);
    //OutputDebugStringA(temp);
}

BaseElement::BaseElement(const elemIDSize id, const std::string& name,
    NvpLayoutBody* const layout, NvpLevel* const level,
    NvpLevel::iterator const iter)
    :self_id(id), self_name(name), self_layout(layout), self_level(level),
    self_iter(iter)
{
    if (self_layout && self_level)
    {
        self_layout->elem = this;
    }
}

BaseElement::~BaseElement()
{
    baseMsg->g_before_leave_id = nullptr;
    baseMsg->g_mouse_drag_id = nullptr;
    baseMsg->g_now_hit_id = nullptr;

    if (self_draw)
    {
        delete self_draw;
    }
    
    if (self_level)
    {
        self_level->erase(self_iter);
    }

    nvpBuild->g_all_elem_store->deleteOneElem(self_id);
}
