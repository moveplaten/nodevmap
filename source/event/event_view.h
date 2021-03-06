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
#include "util/nvp_util.h"

class NvpEventRef;

class NvpEventView
{
public:
    static void mouseEvent(NvpEventRef& event);
    static NvpPoint convertPt(const NvpPoint& pt);
    static void setDpiScale(float x, float y);
    static void handleView(float width, float height, bool set = true);
    static float calcView(bool coord_x, float value);
    static void setCurrentMtx(const NvpMatrix32& mtx);
    static NvpEventView* event_view;

    float trans_x = 0;
    float trans_y = 0;
    float scale_x = 1;
    float scale_y = 1;
    float center_x = 0;
    float center_y = 0;

    float dpi_scale_x = 1;
    float dpi_scale_y = 1;

    float view_width = 0;
    float view_height = 0;

    NvpMatrix32 current_mtx;
    NvpPoint last_pt{ 0 };
};

class NvpCodingMtx
{
public:
    NvpMatrix32 current_mtx;
    void codingMtx(std::string* dst, const char* src)
    {
        NvpCoding::codingSeq(dst, src, &current_mtx._11, &current_mtx._12,
            &current_mtx._21, &current_mtx._22, &current_mtx._31, &current_mtx._32);
    }
};
