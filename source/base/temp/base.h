/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include "event/nvp_event.h"
#include "layout/nvp_layout.h"

class NvpDraw;

#if 0
class BaseMessage
{
public:
    void hitTest(MsgBaseType msg_type, NvpPoint* pt);
    
    NvpPoint getLastMouseLDown() { return g_last_downL_pt; }

protected:
    //virtual void msgRoute(MsgBaseType msg_type, NvpPoint* pt) = 0;

private:
    static NvpBaseObj* g_now_hit_id;
    static NvpBaseObj* g_mouse_drag_id;
    static NvpBaseObj* g_before_leave_id;
    
    static NvpPoint g_last_downL_pt; //local space;

    void initAll(NvpBaseObj* base);
    bool checkLeave();
    NvpBaseObj* inRange(NvpPoint* pt, NvpBaseObj* base);

    void mousePtToLocal(NvpBaseObj* base, NvpPoint* pt);

    friend class NvpBaseObj;
};


/// GLOBAL baseMsg;
extern BaseMessage* const baseMsg;


class BaseAction
{
public:
    NvpPoint local_pt;
    NvpPoint world_pt;

    virtual void realAction(NvpBaseObj* base) = 0;

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
    void mousePtToLocal(NvpBaseObj* base, NvpPoint* pt);

    void prepareDraw(NvpBaseObj* base, MsgBaseType type);

    friend class NvpBaseObj;
};
#endif

class NvpBaseObj
{
public:
    //elemIDSize getSelfID() const { return self_id; }
    
    //const std::string& getSelfName() { return self_name; }

    bool canBeTop() { return self_top; }

    const NvpRect& getRectRefUp() const
    {
        return self_layout.getRectRefUp();
    }
    
    const NvpRect& getRectRefTop() const
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

    void setSelfEvent(NvpEvent* event)
    {
        self_event = event;
    }
    //void setRect(const NvpRect* rect) //use NvpDraw::Record to set;
    //{
    //    self_layout->ref_up = *rect;
    //}

    //elemIDSize getIncreaseID()
    //{
    //    return nvpBuild->g_all_elem_store->getTotalUsed();
    //}
    //NvpBaseObj* getElementByID(elemIDSize id)
    //{
    //    BaseShape* content = base_shapes->readOneElem(id);
    //    return content->elem;
    //}
    //static NvpBaseObj* getNowHitID() { return g_now_hit_id; }

    NvpBaseObj(
        NvpLayout& layout, NvpEvent* event, const bool top = true);

    ~NvpBaseObj();

private:
    //const elemIDSize self_id;
    //const std::string& self_name;
    const bool self_top;
    bool self_visible = true;
    NvpDraw* self_draw = nullptr;

    NvpEvent* self_event;

    NvpLayout self_layout;

    friend class NvpLayoutCache;
    friend class NvpLayout;
    friend class NvpEvent;
};
