/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "nvp_base.h"
#include "draw/nvp_draw.h"

NvpBaseObj::NvpBaseObj(NvpLayout& layout, NvpEvent* event, bool top) :
    self_layout(layout), self_event(event), self_top(top), self_draw(nullptr)
{

}

NvpBaseObj::~NvpBaseObj()
{
    if (self_draw)
    {
        delete self_draw;
    }
    if (self_event)
    {
        delete self_event;
    }
}
