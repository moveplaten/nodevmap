#include "layout.h"

NodeView* g_node_view = nullptr;
MenuBar* g_menu_bar = nullptr;
StatusBar* g_status_bar = nullptr;

BaseElement* elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act)
{
    ElemGenerator(str, msg_type, msg_act);
    auto ret = ElemGenerator::g_node_view_map->find(str);
    auto elem = *ret;
    BaseElement* base = elem.second;
    return base;
}

bool elemDel(const std::string& str)
{
    auto ret = ElemGenerator::g_node_view_map->find(str);
    if (ret == ElemGenerator::g_node_view_map->end())
    {
        return false;
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        ElemGenerator::g_node_view_map->erase(str);
        base->deleteSelf();
        return true;
    }
}

ElemMap* ElemGenerator::g_node_view_map = nullptr;

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act)
{
    static NodeView node_view;
    static MenuBar menu_bar;
    static StatusBar status_bar;

    if (!g_node_view_map)
    {
        static ElemMap elements;
        g_node_view_map = &elements;
        g_node_view = &node_view;
        g_menu_bar = &menu_bar;
        g_status_bar = &status_bar;
    }

    auto ret = g_node_view_map->find(str);
    if (ret == g_node_view_map->end())
    {
        BaseShape content;
        BaseElement* base;
        if (str == "menu_bar")
        {
            elemIDSize id = g_menu_bar->storeOneElem(&content);
            base = new BaseElement(id, str.c_str(), (StoreBaseShape*)g_menu_bar);
        }
        else if (str == "status_bar")
        {
            elemIDSize id = g_status_bar->storeOneElem(&content);
            base = new BaseElement(id, str.c_str(), (StoreBaseShape*)g_status_bar);
        }
        else
        {
            elemIDSize id = node_view.storeOneElem(&content);
            base = new BaseElement(id, str.c_str(), (StoreBaseShape*)g_node_view);
        }
        base->linkMsg(msg_type, msg_act);
        g_node_view_map->insert({ str, base });
    }
    else
    {
        auto elem = *ret;
        BaseElement* base = elem.second;
        base->linkMsg(msg_type, msg_act);
    }
}
