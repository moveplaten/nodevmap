#pragma once

#include <stdint.h>
#include <Windows.h>

typedef unsigned __int32 idSize;
typedef unsigned __int32 msgTypeSize;
typedef RECT baseRect;
typedef POINT mousePt;

enum MsgBaseType
{
    MsgNone,
    MsgInit,
    MouseMove,
    MouseLeave,
    MouseLButtonDown,
    MouseLButtonUp,
    MouseMove_MouseLButtonDown,

};

class BaseElement;

class BaseMessage
{
public:
    static HWND g_hwnd;
    static void hitTest(MsgBaseType msg_type, mousePt* pt);
    static void setHwnd(HWND hwnd) { g_hwnd = hwnd; }
    static bool checkLeave();

protected:
    virtual void msgRoute(MsgBaseType msg_type, mousePt* pt) = 0;

private:
    static BaseElement* inRange(mousePt* pt);
    static void initAll(mousePt* pt);
};


/// GLOBAL baseMsg;
static const BaseMessage* const baseMsg = nullptr;


class BaseAction : public BaseMessage
{
public:
    mousePt pt;

    virtual void realAction(BaseElement* base) {}

    BaseAction::BaseAction() { pt.x = 0; pt.y = 0; }

    BaseAction::~BaseAction() {}

private:
    virtual void msgRoute(MsgBaseType msg_type, mousePt* pt) {}
};


class BaseElement : public BaseMessage
{
public:
    static BaseElement* g_before_leave_id;
    static BaseElement* g_hitTest_id;
    static idSize g_increase_id;
    static BaseElement* g_prev;
    static BaseElement* g_first_element;

    idSize getSelfID() const { return self_id; }
    baseRect* getRect() { return &base_rect; }
    BaseElement* getNext() const { return next; }
    void setRect(baseRect* rect) { base_rect = *rect; }

    virtual void msgRoute(MsgBaseType msg_type, mousePt* pt) override;

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement();

    BaseElement::~BaseElement();

private:
    const idSize self_id = g_increase_id;

    bool self_visible = true;
    baseRect base_rect;

    struct LinkedMsg
    {
        MsgBaseType linked_msg_type = MsgNone;
        BaseAction* linked_msg_act = nullptr;
        LinkedMsg* next_linked_msg = nullptr;
    } linked_msg;
    msgTypeSize linked_msg_size = 0;

    BaseElement* prev = nullptr;
    BaseElement* next = nullptr;
};


class VirtualElement
{
public:
    BaseElement* base;

    VirtualElement::VirtualElement()
    {
        base = new BaseElement;

        if (!BaseElement::g_first_element)
        {
            BaseElement::g_first_element = base;
        }
    }

    VirtualElement::~VirtualElement() {}
};
