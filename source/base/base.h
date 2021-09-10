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
    void initAll(mousePt* pt);
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
    const std::string& getSelfName() { return self_name; }
    auto getSelfLayout()
    {
        return self_layout;
    }

    void deleteSelf()
    {
        g_all_elem_store->deleteOneElem(self_id);
        self_layout->draw->deleteSelf();
        self_level->erase(self_iter);
        auto content = g_all_elem_store->readOneElem(self_id);
        content->rect = { 0 };
        content->elem = 0;
        content->draw = 0;
        content->sub = 0;
        delete this;
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
    static BaseElement* getNowHitID() { return g_hitTest_id; }

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement(const elemIDSize id, const char* name,
        NvpLayout* const layout, NvpLevel* const level,
        NvpLevel::iterator const iter);

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

    NvpLayout* const self_layout;
    NvpLevel* const self_level;
    NvpLevel::iterator const self_iter;

    friend class BaseMessage;
};
