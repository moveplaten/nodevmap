#include "base.h"

NvpLevel* g_top_layout = nullptr;
NvpLevel* g_top_node_view = nullptr;
NvpLevel* g_top_menu_bar = nullptr;
NvpLevel* g_top_status_bar = nullptr;
AllElem* g_all_elem_store = nullptr;
//ElemMap* g_all_elem_map = nullptr;

BaseElement* elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act,
    NvpLevel* level)
{
    auto result = ElemGenerator(str, msg_type, msg_act, level);
    return result.gen_elem;
}

bool elemDel(const std::string& str, NvpLevel* level)
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
        subLevelDel(base);
        return true;
    }
}

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, NvpLevel* level)
{
    if (!g_all_elem_store)
    {
        static AllElem all_elem;
        g_all_elem_store = &all_elem;
        initDefaultLayout();
    }

    if (level)
    {
        auto head = *(level->begin());
        auto elem_map = head->head->cur_map;
        auto ret = elem_map->find(str);
        if (ret == elem_map->end())
        {
            NvpLayoutBody null_layout;
            memset(&null_layout, 0, sizeof(NvpLayoutBody));
            auto id = g_all_elem_store->storeOneElem(&null_layout);
            auto layout = g_all_elem_store->readOneElem(id);
            level->push_back((NvpLayout*)layout);
            auto end = level->end();
            auto iter = --end;

            auto result = elem_map->insert({ str, nullptr });
            auto& content = result.first;
            auto& str_ref = content->first;
            
            BaseElement* base = new BaseElement(id, str_ref, layout, level, iter);
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

NvpLevel* subLevelGen(BaseElement* elem)
{
    if (!elem)
    {
        return nullptr;
    }
    
    auto layout = elem->getSelfLayout();
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
    head_info->up_level = elem->getSelfLevel();
    auto elem_map = new ElemMap;
    head_info->cur_map = elem_map;
    auto depth = getLayoutHead(layout)->cur_depth;
    head_info->cur_depth = ++depth;
    
    sub_level->push_back((NvpLayout*)head_info);
    
    return sub_level;
}

void subLevelDel(BaseElement* elem)
{
    if (!elem)
    {
        return;
    }

    auto sub_level = elem->getSelfLayout()->sub;
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
        subLevelDel(next->body.elem);
    }

    elem_map->clear();
    delete elem_map;
    sub_level->clear();
    delete sub_level;
}

NvpLayoutHead* getLayoutHead(NvpLayoutBody* current)
{
    if (!current)
    {
        return nullptr;
    }
    if (!current->elem)
    {
        return nullptr;
    }
    
    auto cur_level = current->elem->getSelfLevel();
    auto head_layout = *(cur_level->begin());
    
    return head_layout->head;
}

void ElemGenerator::initDefaultLayout()
{
    if (g_top_layout)
    {
        return;
    }
    
    g_top_layout = new NvpLevel;
    
    auto head_info = new NvpLayoutHead;
    memset(head_info, 0, sizeof(NvpLayoutHead));
    head_info->null_head = head_info;
    auto elem_map = new ElemMap;
    head_info->cur_map = elem_map;
    g_top_layout->push_back((NvpLayout*)head_info);
    
    auto top_elem = elemGen("top_layout", MsgNone, nullptr, g_top_layout);

    auto sub_level = subLevelGen(top_elem);

    auto node_view = elemGen("node_view_layout", MsgNone, nullptr, sub_level);
    auto menu_bar = elemGen("menu_bar_layout", MsgNone, nullptr, sub_level);
    auto status_bar = elemGen("status_bar_layout", MsgNone, nullptr, sub_level);

    g_top_node_view = subLevelGen(node_view);
    g_top_menu_bar = subLevelGen(menu_bar);
    g_top_status_bar = subLevelGen(status_bar);
}
