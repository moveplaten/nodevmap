/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/nvp_base.h"
#include "layout_priv.h"

NvpLayoutCache::NvpLayoutCache(LayoutLevel* level)
{
    memset(this, 0, sizeof(NvpLayoutCache));
    layout_level = level;
}

NvpLayoutCache::~NvpLayoutCache()
{
    if (layout_iter != layout_level->end())
    {
        layout_level->erase(layout_iter);
    }
}

NvpBaseObj* NvpLayoutCache::createObj(NvpEvent* event, LayoutLevel* level, bool top)
{
    if (level)
    {
        NvpLayoutCache* layout_cache = new NvpLayoutCache(level);
        NvpLayout layout = NvpLayout(nullptr);
        NvpBaseObj* base_obj = new NvpBaseObj(layout, event, top);
        base_obj->self_layout.layout_cache = layout_cache;

        LayoutUnion layout_union(base_obj);
        level->push_back(layout_union);
        base_obj->self_layout.layout_cache->layout_iter = --(level->end());

        return base_obj;
    }

    return nullptr;
}

NvpLayoutCache::LayoutLevel* NvpLayoutCache::createLevel(NvpBaseObj* base)
{
    if (base)
    {
        auto layout_cache = base->self_layout.layout_cache;

        if (layout_cache->layout_body.sub)
        {
            return layout_cache->layout_body.sub;
        }

        auto layout_level = new LayoutLevel;
        layout_cache->layout_body.sub = layout_level;
        auto layout_info = new LayoutInfo;
        memset(layout_info, 0, sizeof(LayoutInfo));

        layout_info->up = base;
        layout_info->depth = getLayoutInfo(base)->depth++;

        LayoutUnion layout_union(layout_info);
        layout_level->push_back(layout_union);

        return layout_level;
    }

    return nullptr;
}

NvpLayoutCache::LayoutInfo* NvpLayoutCache::getLayoutInfo(NvpBaseObj* base)
{
    if (base)
    {
        auto level = base->self_layout.layout_cache->layout_level;

        return (*level->begin()).info;
    }

    return nullptr;
}

NvpLayoutBuild* NvpLayoutCache::initLayout()
{
    auto top_level = new LayoutLevel;
    auto layout_info = new LayoutInfo;
    memset(layout_info, 0, sizeof(LayoutInfo));
    LayoutUnion layout_union(layout_info);
    top_level->push_back(layout_union);

    auto layout_build = new NvpLayoutBuild_0;
    layout_build->top = createObj(nullptr, top_level, false);
    layout_build->node_view = NvpLayout::createLayout(nullptr, layout_build->top, false);
    layout_build->menu_stat = NvpLayout::createLayout(nullptr, layout_build->top, false);

    return layout_build;
}
