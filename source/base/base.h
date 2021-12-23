#pragma once

#include "event.h"
#include "layout.h"

class NvpDraw;

#if 0
class BaseMessage
{
public:
    void hitTest(MsgBaseType msg_type, mousePt* pt);
    
    mousePt getLastMouseLDown() { return g_last_downL_pt; }

protected:
    //virtual void msgRoute(MsgBaseType msg_type, mousePt* pt) = 0;

private:
    static BaseElement* g_now_hit_id;
    static BaseElement* g_mouse_drag_id;
    static BaseElement* g_before_leave_id;
    
    static mousePt g_last_downL_pt; //local space;

    void initAll(BaseElement* base);
    bool checkLeave();
    BaseElement* inRange(mousePt* pt, BaseElement* base);

    void mousePtToLocal(BaseElement* base, mousePt* pt);

    friend class BaseElement;
};


/// GLOBAL baseMsg;
extern BaseMessage* const baseMsg;


class BaseAction
{
public:
    mousePt local_pt;
    mousePt world_pt;

    virtual void realAction(BaseElement* base) = 0;

    BaseAction()
    {
        local_pt.x = 0; local_pt.y = 0;
        world_pt.x = 0; world_pt.y = 0;
        nvp_draw = nullptr;
    }

    ~BaseAction() {}

protected:
    NvpDraw* nvp_draw;

private:
    void mousePtToLocal(BaseElement* base, mousePt* pt);

    void prepareDraw(BaseElement* base, MsgBaseType type);

    friend class BaseElement;
};
#endif

class BaseElement
{
public:
    elemIDSize getSelfID() const { return self_id; }
    
    const std::string& getSelfName() { return self_name; }

    bool canBeTop() { return self_top; }

    const BaseRect& getRectRefUp() const
    {
        return self_layout.getRectRefUp();
    }
    
    const BaseRect& getRectRefTop() const
    {
        return self_layout.getRectRefTop();
    }

    NvpDraw* getSelfDraw()
    {
        return self_draw;
    }

    void setSelfDraw(NvpDraw* draw)
    {
        self_draw = draw;
    }

    NvpEvent* getSelfEvent()
    {
        return self_event;
    }
    //void setRect(const BaseRect* rect) //use NvpDraw::Record to set;
    //{
    //    self_layout->ref_up = *rect;
    //}

    //elemIDSize getIncreaseID()
    //{
    //    return nvpBuild->g_all_elem_store->getTotalUsed();
    //}
    //BaseElement* getElementByID(elemIDSize id)
    //{
    //    BaseShape* content = base_shapes->readOneElem(id);
    //    return content->elem;
    //}
    //static BaseElement* getNowHitID() { return g_now_hit_id; }

    BaseElement(const elemIDSize id, const std::string& name,
        NvpLayout& layout, NvpEvent* event, const bool top = true);

    ~BaseElement();

private:
    const elemIDSize self_id;
    const std::string& self_name;
    const bool self_top;
    bool self_visible = true;
    NvpDraw* self_draw = nullptr;

    NvpEvent* self_event;

    NvpLayout self_layout;

    friend class NvpLayout;
    friend class NvpEvent;
};
