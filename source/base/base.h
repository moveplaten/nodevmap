#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include "elem_storage.h"

#include <Windows.h>

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
    static ElemStorage* g_store_shapes;
    static ElemStorage* g_store_instance;
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
    mousePt local_pt;
    mousePt world_pt;

    virtual void realAction(BaseElement* base) = 0;

    BaseAction::BaseAction()
    {
        local_pt.x = 0; local_pt.y = 0;
        world_pt.x = 0; world_pt.y = 0;
    }

    BaseAction::~BaseAction() {}

private:
    void mousePtToLocal(BaseElement* base, mousePt* pt);

    friend class BaseElement;
};

struct BaseShape
{
    baseRect rect;
};

struct ElemInstance
{
    BaseElement* elem = nullptr;
};

class BaseElement
{
public:
    elemIDSize getSelfID() const { return self_id; }

    const baseRect* getRect()
    {
        BaseShape* content = (BaseShape*)base_shapes->readOneElem(self_id);
        return &(content->rect);
    }
    void setRect(const baseRect* rect)
    {
        BaseShape* content = (BaseShape*)base_shapes->readOneElem(self_id);
        content->rect = *rect;
    }

    elemIDSize getIncreaseID() { return elem_instance->getTotalUsed(); }
    BaseElement* getElementByID(elemIDSize id)
    {
        ElemInstance* content = (ElemInstance*)elem_instance->readOneElem(id);
        return content->elem;
    }
    static BaseElement* getNowHitID() { return g_hitTest_id; }

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement(const elemIDSize id,
        ElemStorage* const shapes, ElemStorage* const instance);

    BaseElement::~BaseElement();

private:
    void msgRoute(MsgBaseType msg_type, mousePt* pt);

    static BaseElement* g_hitTest_id;
    static BaseElement* g_before_leave_id;

    const elemIDSize self_id;
    bool self_visible = true;
    
    struct LinkedMsg
    {
        MsgBaseType linked_msg_type = MsgNone;
        BaseAction* linked_msg_act = nullptr;
        LinkedMsg* next_linked_msg = nullptr;
    } linked_msg;
    msgTypeSize linked_msg_size = 0;

    ElemStorage* const base_shapes;
    ElemStorage* const elem_instance;

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
