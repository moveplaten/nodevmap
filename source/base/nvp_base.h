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

class NvpBaseObj
{
public:
    bool getSelfTop() const { return self_top; }

    const NvpRect& getRectRefUp() const { return self_layout.getRectRefUp(); }

    const NvpRect& getRectRefTop() const { return self_layout.getRectRefTop(); }

    NvpDraw* getSelfDraw() const { return self_draw; }

    void setSelfDraw(NvpDraw* draw) { self_draw = draw; }

    NvpEvent* getSelfEvent() const { return self_event; }

    void setSelfEvent(NvpEvent* event) { self_event = event; }

    NvpBaseObj(NvpLayout& layout, NvpEvent* event, const bool top = true);
    NvpBaseObj(const NvpBaseObj&) = delete;
    NvpBaseObj& operator=(const NvpBaseObj&) = delete;
    ~NvpBaseObj();

private:
    const bool self_top;

    NvpDraw* self_draw;

    NvpEvent* self_event;

    NvpLayout self_layout;

    friend class NvpLayoutCache;
    friend class NvpLayout;
    friend class NvpEvent;
};
