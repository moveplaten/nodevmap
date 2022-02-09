/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include <vector>
#include <string>
#include <list>
#include <map>
#include "elem_storage.h"

struct BaseRect
{
    float left;
    float top;
    float right;
    float bottom;
};

class NvpBaseObj;
class NvpEvent;

class NvpLayout
{
public:
    NvpLayout() = delete;
    ~NvpLayout();

    static void setBaseRect(NvpBaseObj* base, const BaseRect& rect);

    static bool ptInRect(mousePt pt, BaseRect rect);

    static BaseRect& rectCorrect(BaseRect& rect);

    const BaseRect& getRectRefUp() const { return layout_body.ref_up; }
    
    const BaseRect& getRectRefTop() const { return layout_body.ref_top; }

    static NvpBaseObj* findSameLevel(const std::string& str, NvpBaseObj* base);

    static NvpBaseObj* getUpElem(NvpBaseObj* base);

    static NvpBaseObj* getSubFirst(NvpBaseObj* base);

    static NvpBaseObj* getNext(NvpBaseObj* base);
    
    static NvpBaseObj* getSubLast(NvpBaseObj* base);

    static NvpBaseObj* getNextReverse(NvpBaseObj* base);

    static NvpBaseObj* subElemGen(const std::string& str, NvpEvent* event,
        NvpBaseObj* up, bool be_top = true);

    static void subElemDel(NvpBaseObj* elem);

    static void moveToAllTop(NvpBaseObj* elem);

    static NvpBaseObj* getTopLayout() { return g_top_layout; }

    static NvpBaseObj* getTopNodeView() { return g_top_node_view; }

    static NvpBaseObj* getTopMenuStat() { return g_top_menu_stat; }

    static ElemIDStorage* getIDStorage() { return g_all_id_store; }

private:
    union NvpLayoutUnit;

    typedef std::map<std::string, NvpBaseObj*> ElemMap;
    typedef std::list<NvpLayoutUnit> NvpLevel;

    static NvpBaseObj* g_top_layout;    //0; the whole client area;
    static NvpBaseObj* g_top_node_view; //1;
    static NvpBaseObj* g_top_menu_stat; //1;
    
    static ElemIDStorage* g_all_id_store;

    struct NvpLayoutBody
    {
        BaseRect ref_up; //local space, ref to up level;
        BaseRect ref_top; //world space, ref to top level;

        NvpLevel* sub; //sub level;
    };

    struct NvpLayoutHead
    {
        NvpBaseObj* up_elem;
        NvpLevel* up_level;

        ElemMap* cur_map; //all elem name in this level;
        uint32_t cur_depth;
    };

    union NvpLayoutUnit //only pointers;
    {
        NvpLayoutUnit(NvpLayoutHead* _head) { head = _head; }
        NvpLayoutUnit(NvpBaseObj* _elem) { elem = _elem; }
        
        NvpLayoutHead* head; //head is always first, only first one is valid;
        NvpBaseObj* elem;
    };

    static NvpBaseObj* elemGen(const std::string& str, NvpEvent* event,
        NvpLevel* level, bool be_top = true);

    static bool elemDel(const std::string& str, NvpLevel* level);

    static NvpLevel* subLevelGen(NvpBaseObj* elem);

    static void initDefaultLayout();

    static NvpLayoutHead* getLayoutHead(NvpBaseObj* elem);

    NvpLayout(NvpLayoutBody& body, NvpLevel& level, NvpLevel::iterator iter)
        :layout_body(body), layout_level(level), layout_iter(iter) {}

    ////////////////////////////////////////////////////////////////////////////

    NvpLayoutBody layout_body;
    NvpLevel& layout_level;
    NvpLevel::iterator layout_iter;

    ////////////////////////////////////////////////////////////////////////////
};
