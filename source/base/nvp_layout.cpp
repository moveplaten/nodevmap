#include "base.h"

NvpBuild* nvpBuild = nullptr;

void NvpBuild::setBaseRect(BaseElement* base, const BaseRect& rect)
{
    base->self_layout->ref_up = rect; //ref_up is input from user;

    auto iter = base->self_level->begin();

    auto up_elem = (*iter)->head->up_elem;

    if (up_elem)
    {
        auto up_view_rect = up_elem->self_layout->ref_top;
        auto view_rect = &(base->self_layout->ref_top);

        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;

        view_rect->left = rect.left + up_view_rect.left;
        view_rect->top = rect.top + up_view_rect.top;
        view_rect->right = view_rect->left + width;
        view_rect->bottom = view_rect->top + height;
    }
    else if (base->getSelfName() == "top_layout")
    {
        base->self_layout->ref_top = rect;
    }
}

BaseElement* NvpBuild::findSameLevel(const std::string& str, BaseElement* base)
{
    auto head = *(base->self_level->begin());
    auto elem_map = head->head->cur_map;
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

BaseElement* NvpBuild::getSubFirst(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    if (!base->self_layout->sub)
    {
        return nullptr;
    }

    auto iter = base->self_layout->sub->begin();

    if (++iter == base->self_layout->sub->end())
    {
        return nullptr;
    }

    return (*iter)->body.elem;
}

BaseElement* NvpBuild::getNext(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    auto iter = base->self_iter;

    if (++iter == base->self_level->end())
    {
        return nullptr;
    }

    return (*iter)->body.elem;
}

BaseElement* NvpBuild::getSubLast(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    if (!base->self_layout->sub)
    {
        return nullptr;
    }

    auto iter = base->self_layout->sub->end();

    if (--iter == base->self_layout->sub->begin())
    {
        return nullptr;
    }

    return (*iter)->body.elem;
}

BaseElement* NvpBuild::getNextReverse(BaseElement* base)
{
    if (!base)
    {
        return nullptr;
    }

    auto iter = base->self_iter;

    if (--iter == base->self_level->begin())
    {
        return nullptr;
    }

    return (*iter)->body.elem;
}

BaseElement* NvpBuild::subElemGen(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, BaseElement* up, bool be_top)
{
    auto level = subLevelGen(up);
    return elemGen(str, msg_type, msg_act, level, be_top);
}

void NvpBuild::subElemDel(BaseElement* elem)
{
    if (!elem)
    {
        return;
    }

    auto sub_level = elem->self_layout->sub;
    delete elem;
    if (!sub_level)
    {
        return;
    }

    size_t size = sub_level->size();
    auto iter = sub_level->begin();
    auto header = *iter;
    auto elem_map = header->head->cur_map;
    delete header->head;
    
    for (size_t i = 0; i < size - 1; ++i)
    {
        auto iter = sub_level->begin();
        auto next = *(++iter);
        subElemDel(next->body.elem);
    }

    elem_map->clear();
    delete elem_map;
    sub_level->clear();
    delete sub_level;
}

void NvpBuild::moveToAllTop(BaseElement* elem)
{
    elem->self_level->erase(elem->self_iter);

    auto layout = (NvpLayout*)elem->self_layout;
    auto iter = elem->self_level->end();
    
    auto ret_iter = elem->self_level->insert(iter, layout);
    elem->self_iter = ret_iter;
}

NvpBuild::NvpLayoutHead* NvpBuild::getLayoutHead(NvpLayoutBody* current)
{
    if (!current)
    {
        return nullptr;
    }
    if (!current->elem)
    {
        return nullptr;
    }
    
    auto cur_level = current->elem->self_level;
    auto head_layout = *(cur_level->begin());
    
    return head_layout->head;
}

BaseElement* NvpBuild::elemGen(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, NvpLevel* level, bool be_top)
{
    auto result = ElemGenerator(str, msg_type, msg_act, level, be_top);
    return result.gen_elem;
}

bool NvpBuild::elemDel(const std::string& str, NvpLevel* level)
{
    auto head = *(level->begin());
    auto elem_map = head->head->cur_map;
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

NvpBuild::NvpLevel* NvpBuild::subLevelGen(BaseElement* elem)
{
    if (!elem)
    {
        return nullptr;
    }
    
    auto layout = elem->self_layout;
    if (layout->sub)
    {
        return layout->sub;
    }
    
    auto sub_level = new NvpLevel;
    layout->sub = sub_level;
    
    auto head_info = new NvpLayoutHead;
    memset(head_info, 0, sizeof(NvpLayoutHead));
    head_info->null_head = head_info;
    head_info->up_elem = elem;
    head_info->up_level = elem->self_level;
    auto elem_map = new ElemMap;
    head_info->cur_map = elem_map;
    auto depth = getLayoutHead(layout)->cur_depth;
    head_info->cur_depth = ++depth;
    
    sub_level->push_back((NvpLayout*)head_info);
    
    return sub_level;
}

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, BaseElement* up, bool be_top)
{
    if (!nvpBuild)
    {
        ElemGenerator(str, msg_type, msg_act, (NvpBuild::NvpLevel*)nullptr, be_top);
        up = nvpBuild->g_top_node_view;
    }

    auto level = nvpBuild->subLevelGen(up);
    ElemGenerator(str, msg_type, msg_act, level, be_top);
}

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, NvpBuild::NvpLevel* level, bool be_top)
{
    static NvpBuild::AllElem* temp_all;
    
    if (!nvpBuild)
    {
        static NvpBuild::AllElem all_elem;
        temp_all = &all_elem;
        nvpBuild->initDefaultLayout(&all_elem);
    }

    if (level)
    {
        auto head = *(level->begin());
        auto elem_map = head->head->cur_map;
        auto ret = elem_map->find(str);
        if (ret == elem_map->end())
        {
            NvpBuild::NvpLayoutBody null_layout;
            memset(&null_layout, 0, sizeof(NvpBuild::NvpLayoutBody));
            auto id = temp_all->storeOneElem(&null_layout);
            auto layout = temp_all->readOneElem(id);
            level->push_back((NvpBuild::NvpLayout*)layout);
            auto end = level->end();
            auto iter = --end;

            auto result = elem_map->insert({ str, nullptr });
            auto& content = result.first;
            auto& str_ref = content->first;
            
            BaseElement* base = new BaseElement(id, str_ref, layout, level, iter, be_top);
            gen_elem = base;
            base->linkMsg(msg_type, msg_act);
            content->second = base;
        }
        else
        {
            auto elem = *ret;
            BaseElement* base = elem.second;
            gen_elem = base;
            base->linkMsg(msg_type, msg_act);
        }  
    }
}

void NvpBuild::initDefaultLayout(AllElem* const all_elem)
{
    if (nvpBuild)
    {
        return;
    }

    nvpBuild = (NvpBuild*)0x1; //break loop;

    auto top_level = new NvpLevel;

    auto head_info = new NvpLayoutHead;
    memset(head_info, 0, sizeof(NvpLayoutHead));
    head_info->null_head = head_info;
    auto elem_map = new ElemMap;
    head_info->cur_map = elem_map;
    top_level->push_back((NvpLayout*)head_info);

    auto top_layout = elemGen("top_layout", MsgNone, nullptr, top_level, false);

    auto top_node_view = subElemGen("top_node_view", MsgNone, nullptr, top_layout, false);
    auto top_menu_stat = subElemGen("top_menu_stat", MsgNone, nullptr, top_layout, false);

    nvpBuild = new NvpBuild(top_layout, top_node_view, top_menu_stat, all_elem);
}
