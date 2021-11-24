#include "base.h"
#include "draw/draw.h"

void NvpEvent::fromSysEvent(NvpSysEventType type, NvpEventParam& param)
{
    BaseElement* base = hitLayout(param.getWorldPt(), NvpLayout::getTopLayout());

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

    NvpDrawPort::beginDraw();
}

void NvpEvent::initAll(BaseElement* base, NvpEventParam& param)
{
    for (;;)
    {
        if (base)
        {
            auto draw = new NvpDraw(base);
            base->self_draw = draw;

            base->self_event->userInit(base, param);
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

mousePt NvpEvent::worldToLocal(const mousePt world_pt, BaseElement *base)
{
    mousePt local_pt;
    
    local_pt.x = world_pt.x - base->getRectRefTop().left;
    local_pt.y = world_pt.y - base->getRectRefTop().top;

    return local_pt;
}

BaseElement* NvpEvent::hitLayout(const mousePt world_pt, BaseElement* base)
{
    BaseElement* result = nullptr;
    
    hitLayoutR(world_pt, base, &result);
    return result;
}

void NvpEvent::hitLayoutR(const mousePt world_pt, BaseElement* base, BaseElement** result)
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
