#include "base.h"

NvpBuild* nvpBuild = nullptr;

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

NvpLayoutHead* NvpBuild::getLayoutHead(NvpLayoutBody* current)
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

NvpLevel* NvpBuild::subLevelGen(BaseElement* elem)
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

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, BaseElement* up, bool be_top)
{
    if (!nvpBuild)
    {
        ElemGenerator(str, msg_type, msg_act, (NvpLevel*)nullptr, be_top);
        up = nvpBuild->g_top_node_view;
    }

    auto level = nvpBuild->subLevelGen(up);
    ElemGenerator(str, msg_type, msg_act, level, be_top);
}

ElemGenerator::ElemGenerator(const std::string& str,
    MsgBaseType msg_type, BaseAction* msg_act, NvpLevel* level, bool be_top)
{
    static AllElem* temp_all;
    
    if (!nvpBuild)
    {
        static AllElem all_elem;
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
            NvpLayoutBody null_layout;
            memset(&null_layout, 0, sizeof(NvpLayoutBody));
            auto id = temp_all->storeOneElem(&null_layout);
            auto layout = temp_all->readOneElem(id);
            level->push_back((NvpLayout*)layout);
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
