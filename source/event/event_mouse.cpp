/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/nvp_base.h"

void NvpEventMouse::mouseEvent(NvpEventRef& event)
{
    if (event.getMouseType() == NvpEventMouse::WheelUp ||
        event.getMouseType() == NvpEventMouse::WheelDown)
    {
        NvpEventView::mouseEvent(event);
        return;
    }

    NvpBaseObj* base = nullptr;
    base = hitLayout(event.getWorldPt(), NvpLayout::Build()->getTop());

    if (!base)
    {
        return;
    }
    if (!base->getSelfEvent())
    {
        return;
    }

    NvpPoint local = convertPt(event.getWorldPt(), base);
    event.setLocalPt(local);

    base->getSelfEvent()->base = base;
    base->getSelfEvent()->mouseEvent(event);
}

NvpPoint NvpEventMouse::convertPt(NvpPoint world, NvpBaseObj* base)
{
    NvpPoint local;
    
    local.x = world.x - base->getRectRefTop().left;
    local.y = world.y - base->getRectRefTop().top;

    return local;
}

NvpBaseObj* NvpEventMouse::hitLayout(NvpPoint world, NvpBaseObj* base)
{
    NvpBaseObj* result = nullptr;
    hitLayoutR(world, base, &result);

    return result;
}

void NvpEventMouse::hitLayoutR(NvpPoint world, NvpBaseObj* base, NvpBaseObj** result)
{
    for (;;)
    {
        bool hit = false;

        if (base)
        {
            auto rect = base->getRectRefTop();

            if (rect.left <= world.x && rect.right > world.x &&
                rect.top <= world.y && rect.bottom > world.y)
            {
                *result = base;
                hit = true;
            }
        }

        if (hit)
        {
            auto sub = NvpLayout::getSubLast(base);

            if (sub)
            {
                hitLayoutR(world, sub, result);
            }

            break;
        }

        base = NvpLayout::getNextReverse(base);

        if (!base)
        {
            break;
        }
    }
}
