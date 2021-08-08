#pragma once

#include <stdint.h>
#include <string>
#include <map>

#include <Windows.h>

typedef uint32_t idSize;
typedef uint32_t msgTypeSize;
typedef RECT baseRect;
typedef POINT mousePt;
typedef LONG ptSize;

#define MAX_ELEM_ONE_PAGE 100000

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
    void hitTest(MsgBaseType msg_type, mousePt* pt);
    void setHwnd(HWND hwnd) { g_hwnd = hwnd; }

protected:
    //virtual void msgRoute(MsgBaseType msg_type, mousePt* pt) = 0;

private:
    void initAll(mousePt* pt);
    bool checkLeave();
    BaseElement* inRange(mousePt* pt);
};


/// GLOBAL baseMsg;
static BaseMessage* const baseMsg = nullptr;


class BaseAction : public BaseMessage
{
public:
    mousePt pt;

    virtual void realAction(BaseElement* base) = 0;

    BaseAction::BaseAction() { pt.x = 0; pt.y = 0; }

    BaseAction::~BaseAction() {}

private:
    //virtual void msgRoute(MsgBaseType msg_type, mousePt* pt) {}
};


class BaseElement
{
public:
    idSize getSelfID() const { return self_id; }

    const baseRect* getRect() { return &g_real_rect[self_id]; }
    void setRect(const baseRect* rect) { g_real_rect[self_id] = *rect; }

    static idSize getIncreaseID() { return g_increase_id; }
    static BaseElement* getElementByID(idSize node_id)
    { return g_node_id[node_id]; }
    static BaseElement* getNowHitID() { return g_hitTest_id; }

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement();

    BaseElement::~BaseElement();

private:
    void msgRoute(MsgBaseType msg_type, mousePt* pt);

    /// <summary>
    /// TODO: use dry array struct for simple experiment purpose;
    /// need to improve performance if necessary;
    /// </summary>
    static baseRect g_real_rect[MAX_ELEM_ONE_PAGE];
    static BaseElement* g_node_id[MAX_ELEM_ONE_PAGE];

    volatile static idSize g_increase_id;
    static BaseElement* g_hitTest_id;
    static BaseElement* g_before_leave_id;

    const idSize self_id = g_increase_id;
    bool self_visible = true;
    
    struct LinkedMsg
    {
        MsgBaseType linked_msg_type = MsgNone;
        BaseAction* linked_msg_act = nullptr;
        LinkedMsg* next_linked_msg = nullptr;
    } linked_msg;
    msgTypeSize linked_msg_size = 0;

    //BaseElement* prev = nullptr;
    //BaseElement* next = nullptr;
    friend class BaseMessage;
};

typedef std::map<std::string, BaseElement*> ElemMap;

#define ELEM_GEN(x, y, z)\
ElementGenerator x##y##z(#x, y, &z);

class ElementGenerator
{
public:
    ElementGenerator::ElementGenerator(const std::string& str,
        MsgBaseType msg_type, BaseAction* msg_act);

    ElementGenerator::~ElementGenerator() {}

private:
    static ElemMap* g_elements_map;
};

static void elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act)
{
    ElementGenerator(str, msg_type, msg_act);
}
