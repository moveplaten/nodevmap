/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

struct NvpRect
{
    float left;
    float top;
    float right;
    float bottom;
};

class NvpEvent;
class NvpBaseObj;
class NvpLayoutCache;

class NvpLayoutBuild
{
public:
    virtual ~NvpLayoutBuild() = default;
    virtual NvpBaseObj* getTop() = 0;
    virtual NvpBaseObj* getNodeView() = 0;
    virtual NvpBaseObj* getMenuStat() = 0;
};

class NvpLayout
{
public:
    NvpLayout(NvpLayoutCache* cache);
    ~NvpLayout();

    static NvpLayoutBuild* Build();

    const NvpRect& getRectRefUp() const;

    const NvpRect& getRectRefTop() const;

    static NvpBaseObj* getUpLayout(NvpBaseObj* base);

    static NvpBaseObj* getSubFirst(NvpBaseObj* base);

    static NvpBaseObj* getNext(NvpBaseObj* base);

    static NvpBaseObj* getSubLast(NvpBaseObj* base);

    static NvpBaseObj* getNextReverse(NvpBaseObj* base);

    static void setLayoutRect(NvpBaseObj* base, const NvpRect& rect);

    static bool hitLayoutRect(NvpPoint pt, NvpRect rect);

    static NvpBaseObj* createLayout(NvpEvent* event, NvpBaseObj* up, bool top = true);
    static void destroyLayout(NvpBaseObj* base);

private:
    NvpLayoutCache* layout_cache;

    static NvpLayoutBuild* layout_build;

    friend class NvpLayoutCache;
};
