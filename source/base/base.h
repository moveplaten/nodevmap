#pragma once

#include "layout.h"

#include <Windows.h>

typedef uint32_t msgTypeSize;

typedef struct
{
    float x;
    float y;
}mousePt;

typedef float ptSize;

//#define TEMP_TEST_0

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
    static BaseElement* g_now_hit_id;
    static BaseElement* g_mouse_drag_id;
    static BaseElement* g_before_leave_id;
    
    void initAll(NvpLevel* level);
    bool checkLeave();
    BaseElement* inRange(mousePt* pt, NvpLevel* level);
};


/// GLOBAL baseMsg;
extern BaseMessage* const baseMsg;


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

class BaseElement
{
public:
    elemIDSize getSelfID() const { return self_id; }
    const std::string& getSelfName() { return *self_name; }
    auto getSelfLayout()
    {
        return self_layout;
    }

    auto getSelfLevel()
    {
        return self_level;
    }

    const BaseRect* getRect()
    {
        return &(self_layout->rect);
    }
    void setRect(const BaseRect* rect)
    {
        self_layout->rect = *rect;
    }

    elemIDSize getIncreaseID() { return g_all_elem_store->getTotalUsed(); }
    //BaseElement* getElementByID(elemIDSize id)
    //{
    //    BaseShape* content = base_shapes->readOneElem(id);
    //    return content->elem;
    //}
    //static BaseElement* getNowHitID() { return g_now_hit_id; }

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement(const elemIDSize id, const std::string* name,
        NvpLayoutBody* const layout, NvpLevel* const level,
        NvpLevel::iterator const iter);

    BaseElement::~BaseElement();

    void msgRoute(MsgBaseType msg_type, mousePt* pt = nullptr);

private:
    typedef std::map<MsgBaseType, BaseAction*> MsgActMap;

    const elemIDSize self_id;
    const std::string* const self_name;
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

    NvpLayoutBody* const self_layout;
    NvpLevel* const self_level;
    NvpLevel::iterator const self_iter;

    friend class BaseMessage;
};
