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

#include "base/event.h"

struct BaseRect;

class BaseElement;

struct NvpColor
{
    static NvpColor randomColor();

    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
};

typedef mousePt NvpXyCoord;

class NvpDrawPort
{
private:
    //where define these functions
    //port-win/direct2d/d2d_main.cpp
    //port-osx/quartz/qtz_main.mm

    
    static void beginDraw();

    //draw all str from left to right just one line;
    //start from base rect left-top coord;
    static void drawTextLeftRight(NvpXyCoord start, const std::string& str,
        ptSize font_size, NvpColor colo);

    static void drawOneLine(NvpXyCoord p1, NvpXyCoord p2, NvpColor colo);

    static void fillRect(const BaseRect& rect, NvpColor colo);
    static void frameRect(const BaseRect& rect, NvpColor colo); //only border;

    static void inputImage(const char* file_name);
    static void outputImage(const char* file_name);

    static void outputPDF(const char* file_name);

    friend class NvpDrawReal;
    friend class NvpEvent;
    friend class D2dUtil;
    friend class NvpImageIO;
    friend class NvpPdfIO;
};

class NvpDrawCache;

class NvpDraw
{
public:
    static void drawAll(BaseElement* base);

    void pushDraw(NvpDrawCache& cache);

    NvpDrawCache* getDraw(size_t order); //order from 0;

    NvpDraw(BaseElement* base);

    ~NvpDraw();

private:
    void drawOneCache(const NvpDrawCache& cache, const BaseElement& base);
    
    BaseElement* base_elem;

    std::vector<NvpDrawCache> draw_cache;

    friend class NvpDrawSeq;
};
