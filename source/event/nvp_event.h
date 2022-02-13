/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include "event_mouse.h"

class NvpBaseObj;

class NvpEventRef
{
public:
    void setLocalPt(NvpPoint local) { local_pt = local; }
    NvpPoint getLocalPt() const { return local_pt; }

    void setWorldPt(NvpPoint world) { world_pt = world; }
    NvpPoint getWorldPt() const { return world_pt; }

    void setMouseType(NvpEventMouse::Type type) { type_mouse = type; }
    NvpEventMouse::Type getMouseType() const { return type_mouse; }

private:
    NvpPoint local_pt{ 0 };
    NvpPoint world_pt{ 0 };
    NvpEventMouse::Type type_mouse;
};

class NvpEvent
{
public:
    enum Type
    {
        Mouse,
        Keyboard,
    };
    virtual ~NvpEvent() = default;
    virtual void initEvent(NvpEventRef& event) {}
    virtual void mouseEvent(NvpEventRef& event) {}

    NvpBaseObj* base;

public:
    static void handleEvent(Type type, NvpEventRef& event);
    static void initAll(NvpBaseObj* base, NvpEventRef& event);
};
