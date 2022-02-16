/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include "draw/draw_matrix.h"
#include "event_mouse.h"

class NvpEventRef;

class NvpEventView
{
public:
    static void mouseEvent(NvpEventRef& event);
    static NvpPoint convertPt(const NvpPoint& pt);
    static void setDpiScale(float x, float y);
    static NvpEventView* event_view;

    float trans_x = 0;
    float trans_y = 0;
    float scale_x = 1;
    float scale_y = 1;
    float center_x = 0;
    float center_y = 0;

    float dpi_scale_x = 1;
    float dpi_scale_y = 1;

    NvpMatrix32 current_mtx;
    NvpPoint last_pt{ 0 };
};
