/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "base/nvp_base.h"
#include "layout_priv.h"

NvpLayoutBuild* NvpLayout::layout_build = nullptr;

NvpLayout::NvpLayout(NvpLayoutCache* cache) : layout_cache(cache)
{

}

NvpLayout::~NvpLayout()
{
    delete layout_cache;
}

NvpLayoutBuild* NvpLayout::Build()
{
    if (!layout_build)
    {
        layout_build = NvpLayoutCache::initLayout();
    }
    return layout_build;
}

const NvpRect& NvpLayout::getRectRefUp() const
{
    return layout_cache->layout_body.up;
}

const NvpRect& NvpLayout::getRectRefTop() const
{
    return layout_cache->layout_body.top;
}

NvpBaseObj* NvpLayout::getUpLayout(NvpBaseObj* base)
{
    auto info = NvpLayoutCache::getLayoutInfo(base);

    if (info)
    {
        return info->up;
    }

    return nullptr;
}

NvpBaseObj* NvpLayout::getSubFirst(NvpBaseObj* base)
{
    if (!base)
    {
        return nullptr;
    }

    if (!base->self_layout.layout_cache->layout_body.sub)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_cache->layout_body.sub->begin();

    if (++iter == base->self_layout.layout_cache->layout_body.sub->end())
    {
        return nullptr;
    }

    return (*iter).base;
}

NvpBaseObj* NvpLayout::getNext(NvpBaseObj* base)
{
    if (!base)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_cache->layout_iter;

    if (++iter == base->self_layout.layout_cache->layout_level->end())
    {
        return nullptr;
    }

    return (*iter).base;
}

NvpBaseObj* NvpLayout::getSubLast(NvpBaseObj* base)
{
    if (!base)
    {
        return nullptr;
    }

    if (!base->self_layout.layout_cache->layout_body.sub)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_cache->layout_body.sub->end();

    if (--iter == base->self_layout.layout_cache->layout_body.sub->begin())
    {
        return nullptr;
    }

    return (*iter).base;
}

NvpBaseObj* NvpLayout::getNextReverse(NvpBaseObj* base)
{
    if (!base)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_cache->layout_iter;

    if (--iter == base->self_layout.layout_cache->layout_level->begin())
    {
        return nullptr;
    }

    return (*iter).base;
}

NvpBaseObj* NvpLayout::createLayout(NvpEvent* event, NvpBaseObj* up, bool top)
{
    return NvpLayoutCache::createObj(event, NvpLayoutCache::createLevel(up), top);
}

void NvpLayout::destroyLayout(NvpBaseObj* base)
{
    if (base)
    {
        auto sub_level = base->self_layout.layout_cache->layout_body.sub;
        delete base;

        if (sub_level)
        {
            size_t size = sub_level->size();
            auto iter = sub_level->begin();
            delete (*iter).info;

            for (size_t i = 0; i < size - 1; ++i)
            {
                auto iter = sub_level->begin();
                auto next = *(++iter);
                destroyLayout(next.base);
            }

            sub_level->clear();
            delete sub_level;
        }
    }
}

void NvpLayout::setLayoutRect(NvpBaseObj* base, const NvpRect& rect)
{
    //ref_up is input from user;
    base->self_layout.layout_cache->layout_body.up = rect;

    auto iter = base->self_layout.layout_cache->layout_level->begin();
    auto up = (*iter).info->up;

    if (up)
    {
        auto up_rect = up->self_layout.layout_cache->layout_body.top;
        auto& view_rect = base->self_layout.layout_cache->layout_body.top;

        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;

        view_rect.left = rect.left + up_rect.left;
        view_rect.top = rect.top + up_rect.top;
        view_rect.right = view_rect.left + width;
        view_rect.bottom = view_rect.top + height;
    }
    else
    {
        base->self_layout.layout_cache->layout_body.top = rect;
    }
}

bool NvpLayout::hitLayoutRect(NvpPoint pt, NvpRect rect)
{
    if (rect.left <= pt.x && rect.right > pt.x &&
        rect.top <= pt.y && rect.bottom > pt.y)
    {
        return true;
    }
    return false;
}
