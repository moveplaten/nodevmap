#include "base.h"

BaseElement* BaseElement::g_before_leave_id = nullptr;
BaseElement* BaseElement::g_hitTest_id = nullptr;

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

void BaseAction::mousePtToLocal(BaseElement* base, mousePt* pt)
{
    world_pt = *pt;
    local_pt.x = pt->x - base->getRect()->left;
    local_pt.y = pt->y - base->getRect()->top;
}

void BaseElement::msgRoute(MsgBaseType msg_type, mousePt* pt)
{
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

BaseElement::BaseElement(ElemStorage* const store, const elemIDSize id)
    :elem_stores(store), self_id(id)
{
    if (store)
    {
        BaseContent content;
        content.rect = { 0 };
        content.elem = this;
        store->changeOneElem(self_id, &content);
    }
}

BaseElement::~BaseElement()
{

}

ElemMap* ElementGenerator::g_elements_map = nullptr;
BaseContent* g_content; //for debug use

ElementGenerator::ElementGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act)
{
    static ElemStorage store(MAX_ELEM_ONE_PAGE, sizeof(BaseContent));

    if (!g_elements_map)
    {
        static ElemMap elements;
        g_elements_map = &elements;
        BaseMessage::g_store = &store;
        ::g_content = (BaseContent*)store.getContents();
    }

    auto ret = g_elements_map->find(str);
    if (ret == g_elements_map->end())
    {
        BaseContent content;
        elemIDSize id = store.storeOneElem(&content);
        BaseElement* base = new BaseElement(&store, id);
        base->linkMsg(msg_type, msg_act);
        g_elements_map->insert({ str, base });
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        base->linkMsg(msg_type, msg_act);
    }
}
