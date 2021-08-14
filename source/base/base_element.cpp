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

BaseElement::BaseElement(const elemIDSize id,
    ElemStorage* const shapes, ElemStorage* const instance)
    :self_id(id), base_shapes(shapes), elem_instance(instance)
{
    if (shapes && instance)
    {
        BaseShape* content_shapes = (BaseShape*)base_shapes->readOneElem(self_id);
        ElemInstance* content_inst = (ElemInstance*)elem_instance->readOneElem(self_id);
        content_shapes->rect = { 0 };
        content_inst->elem = this;
    }
}

BaseElement::~BaseElement()
{

}

ElemMap* ElementGenerator::g_elements_map = nullptr;

ElementGenerator::ElementGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act)
{
    static ElemStorage store_shapes(MAX_ELEM_ONE_PAGE, sizeof(BaseShape));
    static ElemStorage elem_inst(MAX_ELEM_ONE_PAGE, sizeof(ElemInstance));

    if (!g_elements_map)
    {
        static ElemMap elements;
        g_elements_map = &elements;
        BaseMessage::g_store_shapes = &store_shapes;
        BaseMessage::g_store_instance = &elem_inst;
    }

    auto ret = g_elements_map->find(str);
    if (ret == g_elements_map->end())
    {
        ElemInstance content;
        elemIDSize id = elem_inst.storeOneElem(&content);
        store_shapes.storeOneElem(&content);
        BaseElement* base = new BaseElement(id, &store_shapes, &elem_inst);
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
