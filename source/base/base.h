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
//#define TEMP_TEST_0

enum MsgBaseType
{
    MsgNone,
    MsgInit,
    MouseMove,
    MouseLeave,
    MouseLButtonDown,
    MouseLButtonUp,
    MouseRButtonDown,
    MouseRButtonUp,
    MouseMove_MouseLButtonDown,

};

class BaseElement;

class BaseMessage
{
public:
    static HWND g_hwnd;
    static ElemStorage* g_store_shapes;
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

    BaseElement* elem = nullptr;
};

class BaseElement
{
public:
    elemIDSize getSelfID() const { return self_id; }
    const std::string& getSelfName() { return self_name; }

    void deleteSelf()
    {
        baseRect rect = { 0 };
        setRect(&rect);
        base_shapes->deleteOneElem(self_id);
        delete this;
    }

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

    elemIDSize getIncreaseID() { return base_shapes->getTotalUsed(); }
    BaseElement* getElementByID(elemIDSize id)
    {
        BaseShape* content = (BaseShape*)base_shapes->readOneElem(id);
        return content->elem;
    }
    static BaseElement* getNowHitID() { return g_hitTest_id; }

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement(const elemIDSize id,
        const char* name, ElemStorage* const shapes);

    BaseElement::~BaseElement();

    void msgRoute(MsgBaseType msg_type, mousePt* pt); //private may better;

private:
    typedef std::map<MsgBaseType, BaseAction*> MsgActMap;

    static BaseElement* g_hitTest_id;
    static BaseElement* g_before_leave_id;
    static BaseElement* g_mouse_snap_id;

    const elemIDSize self_id;
    const std::string self_name;
    bool self_visible = true;
    const char valid_tag = 'v'; //check valid after delete one;

#ifndef TEMP_TEST_0
    MsgActMap msg_act_map;
#endif // !TEMP_TEST_0

#ifdef TEMP_TEST_0
    struct LinkedMsg
    {
        MsgBaseType linked_msg_type = MsgNone;
        BaseAction* linked_msg_act = nullptr;
        LinkedMsg* next_linked_msg = nullptr;
    } linked_msg;
    msgTypeSize linked_msg_size = 0;
#endif // TEMP_TEST_0

    ElemStorage* const base_shapes;

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

//private:
    static ElemMap* g_elements_map;
};

static BaseElement* elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act)
{
    ElementGenerator(str, msg_type, msg_act);
    auto ret = ElementGenerator::g_elements_map->find(str);
    auto elem = *ret;
    BaseElement* base = elem.second;
    return base;
}

static bool elemDel(const std::string& str)
{
    auto ret = ElementGenerator::g_elements_map->find(str);
    if (ret == ElementGenerator::g_elements_map->end())
    {
        return false;
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        ElementGenerator::g_elements_map->erase(str);
        base->deleteSelf();
        return true;
    }
}