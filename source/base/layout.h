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

class BaseElement;
class NvpEvent;

class NvpLayout
{
public:
    NvpLayout() = delete;
    ~NvpLayout();

    static void setBaseRect(BaseElement* base, const BaseRect& rect);

    const BaseRect& getRectRefUp() const { return layout_body.ref_up; }
    
    const BaseRect& getRectRefTop() const { return layout_body.ref_top; }

    static BaseElement* findSameLevel(const std::string& str, BaseElement* base);

    static BaseElement* getSubFirst(BaseElement* base);

    static BaseElement* getNext(BaseElement* base);
    
    static BaseElement* getSubLast(BaseElement* base);

    static BaseElement* getNextReverse(BaseElement* base);

    static BaseElement* subElemGen(const std::string& str, NvpEvent* event,
        BaseElement* up, bool be_top = true);

    static void subElemDel(BaseElement* elem);

    static void moveToAllTop(BaseElement* elem);

    static BaseElement* getTopLayout() { return g_top_layout; }

    static BaseElement* getTopNodeView() { return g_top_node_view; }

    static BaseElement* getTopMenuStat() { return g_top_menu_stat; }

private:
    union NvpLayoutUnit;

    typedef std::map<std::string, BaseElement*> ElemMap;
    typedef std::list<NvpLayoutUnit> NvpLevel;

    static BaseElement* g_top_layout;    //0; the whole client area;
    static BaseElement* g_top_node_view; //1;
    static BaseElement* g_top_menu_stat; //1;
    
    static ElemIDStorage* g_all_id_store;

    struct NvpLayoutBody
    {
        BaseRect ref_up; //local space, ref to up level;
        BaseRect ref_top; //world space, ref to top level;

        NvpLevel* sub; //sub level;
    };

    struct NvpLayoutHead
    {
        BaseElement* up_elem;
        NvpLevel* up_level;

        ElemMap* cur_map; //all elem name in this level;
        uint32_t cur_depth;
    };

    union NvpLayoutUnit //only pointers;
    {
        NvpLayoutUnit(NvpLayoutHead* _head) { head = _head; }
        NvpLayoutUnit(BaseElement* _elem) { elem = _elem; }
        
        NvpLayoutHead* head; //head is always first, only first one is valid;
        BaseElement* elem;
    };

    static BaseElement* elemGen(const std::string& str, NvpEvent* event,
        NvpLevel* level, bool be_top = true);

    static bool elemDel(const std::string& str, NvpLevel* level);

    static NvpLevel* subLevelGen(BaseElement* elem);

    static void initDefaultLayout();

    static NvpLayoutHead* getLayoutHead(BaseElement* elem);

    NvpLayout(NvpLayoutBody& body, NvpLevel& level, NvpLevel::iterator iter)
        :layout_body(body), layout_level(level), layout_iter(iter) {}

    ////////////////////////////////////////////////////////////////////////////

    NvpLayoutBody layout_body;
    NvpLevel& layout_level;
    NvpLevel::iterator layout_iter;

    ////////////////////////////////////////////////////////////////////////////
};
