/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include <list>
#include "nvp_layout.h"

class NvpLayoutCache
{
private:
    struct LayoutInfo
    {
        NvpBaseObj* up;
        uint32_t depth;
    };

    union LayoutUnion
    {
        LayoutUnion(LayoutInfo* info_) : info(info_) {}
        LayoutUnion(NvpBaseObj* base_) : base(base_) {}
        LayoutInfo* info;
        NvpBaseObj* base;
    };

    typedef std::list<LayoutUnion> LayoutLevel;

    struct LayoutBody
    {
        NvpRect up;
        NvpRect top;
        LayoutLevel* sub;
    };

    NvpLayoutCache(LayoutLevel* level);
    ~NvpLayoutCache();

    static NvpBaseObj* createObj(NvpEvent* event, LayoutLevel* level, bool top = true);
    static LayoutLevel* createLevel(NvpBaseObj* base);
    static LayoutInfo* getLayoutInfo(NvpBaseObj* base);
    static NvpLayoutBuild* initLayout();

    LayoutBody layout_body;
    LayoutLevel* layout_level;
    LayoutLevel::iterator layout_iter;

    friend class NvpLayout;
};

class NvpLayoutBuild_0 : public NvpLayoutBuild
{
    NvpBaseObj* getTop() override { return top; }
    NvpBaseObj* getNodeView() override { return node_view; }
    NvpBaseObj* getMenuStat() override { return menu_stat; }

    NvpBaseObj* top;
    NvpBaseObj* node_view;
    NvpBaseObj* menu_stat;

    friend class NvpLayoutCache;
};
