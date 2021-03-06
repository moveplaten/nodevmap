/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include <vector>
#include <string>
#include <cassert>

#include "event/nvp_event.h"
#include "draw_matrix.h"

struct NvpRect;

class NvpBaseObj;

struct NvpColor
{
    static NvpColor randomColor();

    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
};

typedef NvpPoint NvpXyCoord;

class NvpDrawPort
{
public:
    //where define these functions
    //port-win/direct2d/d2d_main.cpp
    //port-osx/quartz/qtz_main.mm

    
    static void beginDraw();

    static void setDrawMatrix(const NvpMatrix32& matrix);

    //draw all str from left to right just one line;
    //start from base rect left-top coord;
    static void drawTextLeftRight(NvpXyCoord start, const std::string& str,
        float font_size, NvpColor colo);

    static void drawOneLine(NvpXyCoord p1, NvpXyCoord p2, NvpColor colo);

    static void fillRect(const NvpRect& rect, NvpColor colo);
    static void frameRect(const NvpRect& rect, NvpColor colo); //only border;

    static void inputImage(const char* file_name);
    static void outputImage(const char* file_name);

    static void outputPDF(const char* file_name);
};

class NvpDrawCache;

class NvpDraw
{
public:
    static void drawAll(NvpBaseObj* base);

    void pushDraw(NvpDrawCache& cache);

    NvpDrawCache* getDraw(size_t order); //order from 0;

    NvpDraw(NvpBaseObj* base);

    ~NvpDraw();

private:
    void drawOneCache(const NvpDrawCache& cache, const NvpBaseObj& base);
    
    NvpBaseObj* base_obj;

    std::vector<NvpDrawCache> draw_cache;

    friend class NvpDrawSeq;
};
