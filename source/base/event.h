/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

typedef float ptSize;

struct NvpPoint
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

class NvpBaseObj;

class NvpEventParam
{
public:
    NvpEventParam() : world_pt({ 0 }), local_pt({ 0 }) {}
    NvpEventParam(NvpPoint world) : world_pt(world), local_pt({ 0 }) {}
    NvpEventParam(NvpPoint world, NvpPoint local) : world_pt(world), local_pt(local) {}

    void setLocalPt(NvpPoint local) { local_pt = local; }

    NvpPoint getLocalPt() { return local_pt; }
    NvpPoint getWorldPt() { return world_pt; }

    void wantCaptureMouse(NvpBaseObj* base) { capture_mouse = base; }
    void freeCaptureMouse() { capture_mouse = nullptr; }
    NvpBaseObj* getMouseCapture() { return capture_mouse; }

private:
    NvpPoint local_pt;
    NvpPoint world_pt;

    NvpBaseObj* capture_mouse = nullptr;
};

class NvpSysEvent
{
public:
    virtual void mouseMove(NvpBaseObj* base, NvpEventParam& param) {}
    virtual void mouseLDown(NvpBaseObj* base, NvpEventParam& param) {}
    virtual void mouseLUp(NvpBaseObj* base, NvpEventParam& param) {}
    virtual void mouseRDown(NvpBaseObj* base, NvpEventParam& param) {}
    virtual void mouseRUp(NvpBaseObj* base, NvpEventParam& param) {}
    virtual void mouseLDrag(NvpBaseObj* base, NvpEventParam& param) {}

    friend class NvpEvent;
};

class NvpUserEvent
{
    virtual void userInit(NvpBaseObj* base, NvpEventParam& param) {}

    friend class NvpEvent;
};

class NvpEvent : public NvpSysEvent, public NvpUserEvent
{
public:
    static void fromSysEvent(NvpSysEventType type, NvpEventParam& param);

    static NvpPoint worldToLocal(const NvpPoint world_pt, NvpBaseObj* base);

    static NvpBaseObj* hitLayout(const NvpPoint world_pt, NvpBaseObj* base);

    static void initAll(NvpBaseObj* base, NvpEventParam& param);

private:
    static void fromUserEvent(NvpUserEventType type, NvpEventParam& param);

    static void hitLayoutR(const NvpPoint world_pt, NvpBaseObj* base, NvpBaseObj** result);
};
