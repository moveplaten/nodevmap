#include "base.h"

HWND BaseMessage::g_hwnd = nullptr;

void BaseMessage::hitTest(MsgBaseType msg_type, mousePt* pt)
{
    if (msg_type == MsgInit)
    {
        initAll(pt);
    }
    if (msg_type == MouseLeave)
    {
        BaseElement::g_before_leave_id->msgRoute(MouseLeave, pt);
    }
    else
    {
        BaseElement* fetch = inRange(pt);

        if (fetch)
        {
            fetch->msgRoute(msg_type, pt);
        }
    }
}

void BaseMessage::initAll(mousePt* pt)
{
    BaseElement* fetch = BaseElement::g_first_element;

    for (idSize i = 0; i < BaseElement::g_increase_id - 1; ++i)
    {
        fetch->msgRoute(MsgInit, pt);
        fetch = fetch->getNext();
    }
}

bool checkLeave()
{
    bool resetStamp1 = false;
    static BaseElement* stamp[2] = { nullptr, nullptr };

    if (stamp[0] == nullptr)
    {
        stamp[0] = BaseElement::g_hitTest_id;
        resetStamp1 = true;
    }
    else
    {
        stamp[1] = BaseElement::g_hitTest_id;
    }

    size_t check_sub = stamp[0] - stamp[1];
    BaseElement* p_leave_id;

    if (resetStamp1) { p_leave_id = stamp[1]; stamp[1] = nullptr; }
    else { p_leave_id = stamp[0]; stamp[0] = nullptr; }

    if (check_sub == 0) return false;
    else
    {
        BaseElement::g_before_leave_id = p_leave_id;
        return true;
    }
}

bool BaseMessage::checkLeave()
{
    static BaseElement* prev_hit = nullptr;
    size_t check_sub = prev_hit - BaseElement::g_hitTest_id;

    if (check_sub == 0)
    {
        return false;
    }
    else
    {
        BaseElement::g_before_leave_id = prev_hit;
        prev_hit = BaseElement::g_hitTest_id;
        return true;
    }
}

BaseElement* BaseMessage::inRange(mousePt* pt)
{
    BaseElement* fetch = BaseElement::g_first_element;
    if (PtInRect(fetch->getRect(), *pt))
    {
        BaseElement::g_hitTest_id = fetch;
        return fetch;
    }

    for (idSize i = 0; i < BaseElement::g_increase_id - 1; ++i)
    {
        fetch = fetch->getNext();
        if (PtInRect(fetch->getRect(), *pt))
        {
            BaseElement::g_hitTest_id = fetch;
            return fetch;
        }
    }

    BaseElement::g_hitTest_id = nullptr;

    return nullptr;
}
