#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include "elem_storage.h"

#include <Windows.h>

typedef uint32_t msgTypeSize;

typedef struct
{
    float x;
    float y;
}mousePt;

typedef float ptSize;

struct BaseShape;
typedef ElemStorage<NULL, BaseShape> StoreBaseShape;

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

struct BaseRect
{
    float left;
    float top;
    float right;
    float bottom;
};

struct BaseShape
{
    BaseRect rect;
    NvpDraw* draw = nullptr;
    BaseElement* elem = nullptr;
};

class BaseElement
{
public:
    elemIDSize getSelfID() const { return self_id; }
    const std::string& getSelfName() { return self_name; }
    auto getBaseShapes()
    {
        return base_shapes;
    }

    void deleteSelf()
    {
        BaseRect rect = { 0 };
        setRect(&rect);
        base_shapes->deleteOneElem(self_id);
        auto content = base_shapes->readOneElem(self_id);
        delete content->draw;
        content->draw = nullptr;
        content->elem = nullptr;
        delete this;
    }

    const BaseRect* getRect()
    {
        BaseShape* content = base_shapes->readOneElem(self_id);
        return &(content->rect);
    }
    void setRect(const BaseRect* rect)
    {
        BaseShape* content = base_shapes->readOneElem(self_id);
        content->rect = *rect;
    }

    elemIDSize getIncreaseID() { return base_shapes->getTotalUsed(); }
    BaseElement* getElementByID(elemIDSize id)
    {
        BaseShape* content = base_shapes->readOneElem(id);
        return content->elem;
    }
    static BaseElement* getNowHitID() { return g_hitTest_id; }

    void linkMsg(MsgBaseType msg_type, BaseAction* msg_act);

    BaseElement::BaseElement(const elemIDSize id,
        const char* name, StoreBaseShape* const shapes);

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

    StoreBaseShape* const base_shapes;

    friend class BaseMessage;
};
