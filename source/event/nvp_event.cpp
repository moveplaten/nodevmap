/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/nvp_base.h"
#include "draw/nvp_draw.h"

void NvpEvent::handleEvent(Type type, NvpEventRef& event)
{
    if (type == Mouse)
    {
        NvpEventMouse::mouseEvent(event);
    }

    NvpDrawPort::beginDraw();
}

void NvpEvent::initAll(NvpBaseObj* base, NvpEventRef& event)
{
    for (;;)
    {
        if (base)
        {
            if (!base->self_draw)
            {
                auto draw = new NvpDraw(base);
                base->self_draw = draw;

                if (base->self_event)
                {
                    base->self_event->base = base;
                    base->self_event->initEvent(event);
                }
            }
        }

        auto sub = NvpLayout::getSubFirst(base);

        if (sub)
        {
            initAll(sub, event);
        }

        base = NvpLayout::getNext(base);

        if (!base)
        {
            break;
        }
    }
}
