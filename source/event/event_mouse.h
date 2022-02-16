/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

struct NvpPoint
{
    float x;
    float y;
};

class NvpBaseObj;
class NvpEventRef;

class NvpEventMouse
{
public:
    enum Type
    {
        MouseMove,
        MouseLDown,
        MouseLUp,
        MouseRDown,
        MouseRUp,
        MouseLDrag,
        WheelUp,
        WheelDown,
        WheelDrag,
        WheelPush,
    };

    static void mouseEvent(NvpEventRef& event);

    static NvpPoint convertPt(NvpPoint world, NvpBaseObj* base);

    static NvpBaseObj* hitLayout(NvpPoint world, NvpBaseObj* base);

private:
    static void hitLayoutR(NvpPoint world, NvpBaseObj* base, NvpBaseObj** result);
};
