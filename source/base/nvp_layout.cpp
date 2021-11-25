#include "base.h"

BaseElement* NvpLayout::g_top_layout = nullptr;
BaseElement* NvpLayout::g_top_node_view = nullptr;
BaseElement* NvpLayout::g_top_menu_stat = nullptr;
ElemIDStorage* NvpLayout::g_all_id_store = nullptr;

void NvpLayout::setBaseRect(BaseElement* base, const BaseRect& rect)
{
    //ref_up is input from user;
    base->self_layout.layout_body.ref_up = rectCorrect(const_cast<BaseRect&>(rect));

    auto iter = base->self_layout.layout_level.begin();

    auto up_elem = (*iter).head->up_elem;

    if (up_elem)
    {
        auto up_view_rect = up_elem->self_layout.layout_body.ref_top;
        auto& view_rect = base->self_layout.layout_body.ref_top;

        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;

        view_rect.left = rect.left + up_view_rect.left;
        view_rect.top = rect.top + up_view_rect.top;
        view_rect.right = view_rect.left + width;
        view_rect.bottom = view_rect.top + height;
    }
    else if (base->getSelfName() == "top_layout")
    {
        base->self_layout.layout_body.ref_top = rect;
    }
}

BaseRect& NvpLayout::rectCorrect(BaseRect& rect)
{
    float temp;

    if (rect.left > rect.right)
    {
        temp = rect.left; rect.left = rect.right; rect.right = temp;
    }

    if (rect.top > rect.bottom)
    {
        temp = rect.top; rect.top = rect.bottom; rect.bottom = temp;
    }

    return rect;
}

BaseElement* NvpLayout::findSameLevel(const std::string& str, BaseElement* base)
{
    auto head = *(base->self_layout.layout_level.begin());
    auto elem_map = head.head->cur_map;
    auto ret = elem_map->find(str);

    if (ret == elem_map->end())
    {
        return nullptr;
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        return base;
    }
}

BaseElement* NvpLayout::getSubFirst(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    if (!base->self_layout.layout_body.sub)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_body.sub->begin();

    if (++iter == base->self_layout.layout_body.sub->end())
    {
        return nullptr;
    }

    return (*iter).elem;
}

BaseElement* NvpLayout::getNext(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_iter;

    if (++iter == base->self_layout.layout_level.end())
    {
        return nullptr;
    }

    return (*iter).elem;
}

BaseElement* NvpLayout::getSubLast(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    if (!base->self_layout.layout_body.sub)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_body.sub->end();

    if (--iter == base->self_layout.layout_body.sub->begin())
    {
        return nullptr;
    }

    return (*iter).elem;
}

BaseElement* NvpLayout::getNextReverse(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    auto iter = base->self_layout.layout_iter;

    if (--iter == base->self_layout.layout_level.begin())
    {
        return nullptr;
    }

    return (*iter).elem;
}

BaseElement* NvpLayout::subElemGen(const std::string& str,
    NvpEvent* event, BaseElement* up, bool be_top)
{
    if (!g_top_layout)
    {
        elemGen(str, event, (NvpLayout::NvpLevel*)nullptr, be_top);
        up = NvpLayout::g_top_layout;
    }
    
    auto level = subLevelGen(up);
    return elemGen(str, event, level, be_top);
}

void NvpLayout::subElemDel(BaseElement* elem)
{
    if (!elem)
    {
        return;
    }

    auto sub_level = elem->self_layout.layout_body.sub;
    delete elem;
    if (!sub_level)
    {
        return;
    }

    size_t size = sub_level->size();
    auto iter = sub_level->begin();
    auto header = *iter;
    auto elem_map = header.head->cur_map;
    delete header.head;
    
    for (size_t i = 0; i < size - 1; ++i)
    {
        auto iter = sub_level->begin();
        auto next = *(++iter);
        subElemDel(next.elem);
    }

    elem_map->clear();
    delete elem_map;
    sub_level->clear();
    delete sub_level;
}

void NvpLayout::moveToAllTop(BaseElement* elem)
{
    elem->self_layout.layout_level.erase(elem->self_layout.layout_iter);

    NvpLayoutUnit unit(elem);
    auto iter = elem->self_layout.layout_level.end();
    
    auto ret_iter = elem->self_layout.layout_level.insert(iter, unit);
    elem->self_layout.layout_iter = ret_iter;
}

NvpLayout::NvpLayoutHead* NvpLayout::getLayoutHead(BaseElement* elem)
{
    if (!elem)
    {
        return nullptr;
    }
    
    auto& cur_level = elem->self_layout.layout_level;
    auto head_layout = *(cur_level.begin());
    
    return head_layout.head;
}

BaseElement* NvpLayout::elemGen(const std::string& str,
    NvpEvent* event, NvpLevel* level, bool be_top)
{
    if (!g_top_layout)
    {
        static ElemIDStorage id_store;
        g_all_id_store = &id_store;
        initDefaultLayout();
    }

    if (level)
    {
        auto head = *(level->begin());
        auto elem_map = head.head->cur_map;
        auto ret = elem_map->find(str);
        if (ret == elem_map->end())
        {
            NvpLayoutBody null_layout;
            memset(&null_layout, 0, sizeof(NvpLayoutBody));

            auto null_iter = level->end();
            NvpLayout layout(null_layout, *level, null_iter);

            auto result = elem_map->insert({ str, nullptr });
            auto& content = result.first;
            auto& str_ref = content->first;

            auto id = NvpLayout::g_all_id_store->storeOneElem();

            BaseElement* base = new BaseElement(id, str_ref, layout, event, be_top);

            NvpLayoutUnit unit(base);

            level->push_back(unit);
            auto iter = --(level->end());
            base->self_layout.layout_iter = iter;

            content->second = base;

            return base;
        }
        else
        {
            auto elem = *ret;
            BaseElement* base = elem.second;

            base->self_event = event;
            return base;
        }
    }

    return nullptr;
}

bool NvpLayout::elemDel(const std::string& str, NvpLevel* level)
{
    auto head = *(level->begin());
    auto elem_map = head.head->cur_map;
    auto ret = elem_map->find(str);
    if (ret == elem_map->end())
    {
        return false;
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        elem_map->erase(str);
        subElemDel(base);
        return true;
    }
}

NvpLayout::NvpLevel* NvpLayout::subLevelGen(BaseElement* elem)
{
    if (!elem)
    {
        return nullptr;
    }
    
    auto layout = &elem->self_layout;
    
    if (layout->layout_body.sub)
    {
        return layout->layout_body.sub;
    }
    
    auto sub_level = new NvpLevel;
    layout->layout_body.sub = sub_level;
    
    auto head_info = new NvpLayoutHead;
    memset(head_info, 0, sizeof(NvpLayoutHead));
    
    head_info->up_elem = elem;
    head_info->up_level = &elem->self_layout.layout_level;
    auto elem_map = new ElemMap;
    head_info->cur_map = elem_map;
    auto depth = getLayoutHead(elem)->cur_depth;
    head_info->cur_depth = ++depth;

    NvpLayoutUnit unit(head_info);
    sub_level->push_back(unit);
    
    return sub_level;
}

void NvpLayout::initDefaultLayout()
{
    if (g_top_layout)
    {
        return;
    }

    g_top_layout = (BaseElement*)0x1; //break loop;

    auto top_level = new NvpLevel;

    auto head_info = new NvpLayoutHead;
    memset(head_info, 0, sizeof(NvpLayoutHead));

    auto elem_map = new ElemMap;
    head_info->cur_map = elem_map;
    NvpLayoutUnit unit(head_info);
    top_level->push_back(unit);

    auto null_event = new NvpEvent;

    g_top_layout = elemGen("top_layout", null_event, top_level, false);

    g_top_node_view = subElemGen("top_node_view", null_event, g_top_layout, false);
    g_top_menu_stat = subElemGen("top_menu_stat", null_event, g_top_layout, false);
}

NvpLayout::~NvpLayout()
{
    if (layout_iter != layout_level.end())
    {
        layout_level.erase(layout_iter);

        auto content = *layout_iter;
        g_all_id_store->deleteOneElem(content.elem->self_id);
    }
}
