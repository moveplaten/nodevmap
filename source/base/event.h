#pragma once

typedef float ptSize;

struct mousePt
{
    float x;
    float y;
};

enum NvpSysEventType
{
    SysInit,
    MouseMove,
    MouseLDown,
    MouseLUp,
    MouseRDown,
    MouseRUp,
    MouseLDrag,
    
};

enum NvpUserEventType
{
    UserInit,
    MouseEnter,
    MouseLeave,
    MouseHover,
    
};

class BaseElement;

class NvpEventParam
{
public:
    NvpEventParam() : world_pt({ 0 }), local_pt({ 0 }) {}
    NvpEventParam(mousePt world) : world_pt(world), local_pt({ 0 }) {}
    NvpEventParam(mousePt world, mousePt local) : world_pt(world), local_pt(local) {}

    void setLocalPt(mousePt local) { local_pt = local; }

    mousePt getLocalPt() { return local_pt; }
    mousePt getWorldPt() { return world_pt; }

private:
    mousePt local_pt;
    mousePt world_pt;
};

class NvpSysEvent
{
public:
    virtual void mouseMove(BaseElement* base, NvpEventParam& param) {}
    virtual void mouseLDown(BaseElement* base, NvpEventParam& param) {}
    virtual void mouseLUp(BaseElement* base, NvpEventParam& param) {}
    virtual void mouseRDown(BaseElement* base, NvpEventParam& param) {}
    virtual void mouseRUp(BaseElement* base, NvpEventParam& param) {}
    virtual void mouseLDrag(BaseElement* base, NvpEventParam& param) {}

    friend class NvpEvent;
};

class NvpUserEvent
{
    virtual void userInit(BaseElement* base, NvpEventParam& param) {}

    friend class NvpEvent;
};

class NvpEvent : public NvpSysEvent, public NvpUserEvent
{
public:
    static void fromSysEvent(NvpSysEventType type, NvpEventParam& param);

    static mousePt worldToLocal(const mousePt world_pt, BaseElement* base);

    static BaseElement* hitLayout(const mousePt world_pt, BaseElement* base);

private:
    static void fromUserEvent(NvpUserEventType type, NvpEventParam& param);

    static void initAll(BaseElement* base, NvpEventParam& param);

    static void hitLayoutR(const mousePt world_pt, BaseElement* base, BaseElement** result);
};
