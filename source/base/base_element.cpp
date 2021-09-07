#include "base.h"

BaseElement* BaseElement::g_before_leave_id = nullptr;
BaseElement* BaseElement::g_hitTest_id = nullptr;
BaseElement* BaseElement::g_mouse_snap_id = nullptr;

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
    if (valid_tag != 'v')
    {
        return;
    }

    auto ret = msg_act_map.find(msg_type);
    if (ret == msg_act_map.end())
    {
        return;
    }
    else
    {
        auto temp = *ret;
        BaseAction* act = temp.second;
        act->mousePtToLocal(this, pt);
        act->realAction(this);
    }
}
#endif // !TEMP_TEST_0

void BaseAction::mousePtToLocal(BaseElement* base, mousePt* pt)
{
    world_pt = *pt;
    local_pt.x = pt->x - base->getRect()->left;
    local_pt.y = pt->y - base->getRect()->top;
}

BaseElement::BaseElement(const elemIDSize id,
    const char* name, StoreBaseShape* const shapes)
    :self_id(id), self_name(name), base_shapes(shapes)
{
    if (base_shapes)
    {
        BaseShape* content_shapes = base_shapes->readOneElem(self_id);
        content_shapes->rect = { 0 };
        content_shapes->elem = this;
    }
}

BaseElement::~BaseElement()
{

}
