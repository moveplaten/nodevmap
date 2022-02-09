/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base.h"
#include "draw/nvp_draw.h"

void NvpEvent::fromSysEvent(NvpSysEventType type, NvpEventParam& param)
{
    static NvpBaseObj* mouse_capture = nullptr;

    NvpBaseObj* base;

    if (mouse_capture)
    {
        base = mouse_capture;
    }
    else
    {
        base = hitLayout(param.getWorldPt(), NvpLayout::getTopLayout());
    }
    
    if (!base)
    {
        return;
    }

    mousePt local_pt = worldToLocal(param.getWorldPt(), base);
    param.setLocalPt(local_pt);

    switch (type)
    {
    case SysInit:
    {
        initAll(NvpLayout::getTopLayout(), param);
    }
    break;

    case MouseMove:
    {
        base->self_event->mouseMove(base, param);
    }
    break;

    case MouseLDown:
    {
        base->self_event->mouseLDown(base, param);
    }
    break;

    case MouseLUp:
    {
        base->self_event->mouseLUp(base, param);
    }
    break;

    case MouseRDown:
    {
        base->self_event->mouseRDown(base, param);
    }
    break;

    case MouseRUp:
    {
        base->self_event->mouseRUp(base, param);
    }
    break;

    case MouseLDrag:
    {
        base->self_event->mouseLDrag(base, param);
    }
    break;

    default:
        break;
    }

    mouse_capture = param.getMouseCapture();

    NvpDrawPort::beginDraw();
}

void NvpEvent::initAll(NvpBaseObj* base, NvpEventParam& param)
{
    for (;;)
    {
        if (base)
        {
            if (!base->self_draw)
            {
                auto draw = new NvpDraw(base);
                base->self_draw = draw;
                base->self_event->userInit(base, param);
            }
        }

        auto sub = NvpLayout::getSubFirst(base);

        if (sub)
        {
            initAll(sub, param);
        }

        base = NvpLayout::getNext(base);

        if (!base)
        {
            break;
        }
    }
}

mousePt NvpEvent::worldToLocal(const mousePt world_pt, NvpBaseObj *base)
{
    mousePt local_pt;
    
    local_pt.x = world_pt.x - base->getRectRefTop().left;
    local_pt.y = world_pt.y - base->getRectRefTop().top;

    return local_pt;
}

NvpBaseObj* NvpEvent::hitLayout(const mousePt world_pt, NvpBaseObj* base)
{
    NvpBaseObj* result = nullptr;
    
    hitLayoutR(world_pt, base, &result);
    return result;
}

void NvpEvent::hitLayoutR(const mousePt world_pt, NvpBaseObj* base, NvpBaseObj** result)
{
    for (;;)
    {
        bool hit = false;

        if (base)
        {
            auto rect = base->getRectRefTop();

            if (rect.left <= world_pt.x && rect.right > world_pt.x &&
                rect.top <= world_pt.y && rect.bottom > world_pt.y)
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
                hitLayoutR(world_pt, sub, result);
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
