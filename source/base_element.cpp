#include "base.h"

BaseElement* BaseElement::g_before_leave_id = nullptr;
BaseElement* BaseElement::g_hitTest_id = nullptr;
volatile idSize BaseElement::g_increase_id = 0;
baseRect BaseElement::g_real_rect[MAX_ELEM_ONE_PAGE];
BaseElement* BaseElement::g_node_id[MAX_ELEM_ONE_PAGE];

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
    if (linked_msg.linked_msg_type == msg_type)
    {
        linked_msg.linked_msg_act->pt = *pt;
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
                next_linked_msg->linked_msg_act->pt = *pt;
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
            fetch_msg_act->realAction(this);
        }
    }
}

BaseElement::BaseElement()
{
    g_node_id[self_id] = this;
    ++g_increase_id;
    //prev = BaseElement::g_prev;
    //BaseElement::g_prev = this;
    //prev ? prev->next = this : prev;
}

BaseElement::~BaseElement()
{

}
