#include "base.h"

NvpLevel* g_top_layout = nullptr;
NvpLevel* g_top_node_view = nullptr;
NvpLevel* g_top_menu_bar = nullptr;
NvpLevel* g_top_status_bar = nullptr;
AllElem* g_all_elem_store = nullptr;
ElemMap* g_all_elem_map = nullptr;

BaseElement* elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act,
    NvpLevel* level)
{
    ElemGenerator(str, msg_type, msg_act, level);
    auto ret = g_all_elem_map->find(str);
    if (ret == g_all_elem_map->end())
    {
        return nullptr;
    }
    auto elem = *ret;
    BaseElement* base = elem.second;
    return base;
}

bool elemDel(const std::string& str)
{
    auto ret = g_all_elem_map->find(str);
    if (ret == g_all_elem_map->end())
    {
        return false;
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        g_all_elem_map->erase(str);
        base->deleteSelf();
        return true;
    }
}

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, NvpLevel* level)
{
    if (!g_all_elem_map)
    {
        static ElemMap elements;
        static AllElem all_elem;
        g_all_elem_map = &elements;
        g_all_elem_store = &all_elem;
        initDefaultLayout();
    }

    if (level)
    {
        auto ret = g_all_elem_map->find(str);
        if (ret == g_all_elem_map->end())
        {
            NvpLayout null_layout;
            auto id = g_all_elem_store->storeOneElem(&null_layout);
            auto layout = g_all_elem_store->readOneElem(id);
            level->push_back(layout);
            auto end = level->end();
            auto iter = --end;
            BaseElement* base = new BaseElement(id, str.c_str(), layout, level, iter);
            base->linkMsg(msg_type, msg_act);
            g_all_elem_map->insert({ str, base });
        }
        else
        {
            auto elem = *ret;
            BaseElement* base = elem.second;
            base->linkMsg(msg_type, msg_act);
        }  
    }
}

NvpLevel* subLevelGen(NvpLayout* current)
{
    if (!current)
    {
        return nullptr;
    }
    if (current->sub)
    {
        return nullptr;
    }
    auto sub_level = new NvpLevel;
    current->sub = sub_level;
    return sub_level;
}

void ElemGenerator::initDefaultLayout()
{
    if (g_top_layout)
    {
        return;
    }
    
    g_top_layout = new NvpLevel;
    auto top_elem = elemGen("top_layout", MsgNone, nullptr, g_top_layout);

    auto sub_level = subLevelGen(top_elem->getSelfLayout());

    auto node_view = elemGen("node_view_layout", MsgNone, nullptr, sub_level);
    auto menu_bar = elemGen("menu_bar_layout", MsgNone, nullptr, sub_level);
    auto status_bar = elemGen("status_bar_layout", MsgNone, nullptr, sub_level);

    g_top_node_view = subLevelGen(node_view->getSelfLayout());
    g_top_menu_bar = subLevelGen(menu_bar->getSelfLayout());
    g_top_status_bar = subLevelGen(status_bar->getSelfLayout());
}
