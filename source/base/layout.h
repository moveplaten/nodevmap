#pragma once

#include <vector>
#include <string>
#include <list>
#include <map>
#include "elem_storage.h"

enum MsgBaseType
{
    MsgNone,
    MsgInit,
    MouseMove,
    MouseEnter,
    MouseLeave,
    MouseLButtonDown,
    MouseLButtonUp,
    MouseRButtonDown,
    MouseRButtonUp,
    MouseMove_MouseLButtonDown,

};

typedef float ptSize;

struct mousePt
{
    float x;
    float y;
};

struct BaseRect
{
    float left;
    float top;
    float right;
    float bottom;
};

class BaseElement;
class BaseAction;
class NvpDraw;

#define ELEM_GEN(x, y, z)\
ElemGenerator x##y##z(#x, y, &z);

#define ELEM_GEN_FULL(x, y, z, l)\
ElemGenerator x##y##z(#x, y, &z, l);

class NvpLayout
{
public:
    NvpLayout() = delete;
    ~NvpLayout();

    static void setBaseRect(BaseElement* base, const BaseRect& rect);

    void setBaseElem(BaseElement* base) { layout_body.elem = base; }

    const BaseRect& getRectRefUp() const { return layout_body.ref_up; }
    
    const BaseRect& getRectRefTop() const { return layout_body.ref_top; }

    static BaseElement* findSameLevel(const std::string& str, BaseElement* base);

    static BaseElement* getSubFirst(BaseElement* base);

    static BaseElement* getNext(BaseElement* base);
    
    static BaseElement* getSubLast(BaseElement* base);

    static BaseElement* getNextReverse(BaseElement* base);

    static BaseElement* subElemGen(const std::string& str, MsgBaseType msg_type,
        BaseAction* msg_act, BaseElement* up, bool be_top = true);

    static void subElemDel(BaseElement* elem);

    static void moveToAllTop(BaseElement* elem);

    static BaseElement* getTopLayout() { return g_top_layout; }

    static BaseElement* getTopNodeView() { return g_top_node_view; }

    static BaseElement* getTopMenuStat() { return g_top_menu_stat; }

private:
    union NvpLayoutUnit;

    typedef std::map<std::string, BaseElement*> ElemMap;
    typedef std::list<NvpLayoutUnit*> NvpLevel;

    static BaseElement* g_top_layout;    //0; the whole client area;
    static BaseElement* g_top_node_view; //1;
    static BaseElement* g_top_menu_stat; //1;
    
    static ElemIDStorage* g_all_id_store;

    struct NvpLayoutBody
    {
        BaseRect ref_up; //local space, ref to up level;
        BaseRect ref_top; //world space, ref to top level;

        BaseElement* elem;
        NvpLevel* sub; //sub level;
    };

    struct NvpLayoutHead
    {
        void* null_head; //should be first?

        BaseElement* up_elem;
        NvpLevel* up_level;

        ElemMap* cur_map; //all elem name in this level;
        uint32_t cur_depth;
    };

    union NvpLayoutUnit
    {
        NvpLayoutHead* head; //head is always first, only first one is valid;
        NvpLayoutBody body;
    };

    static BaseElement* elemGen(const std::string& str, MsgBaseType msg_type,
        BaseAction* msg_act, NvpLevel* level, bool be_top = true);

    static bool elemDel(const std::string& str, NvpLevel* level);

    static NvpLevel* subLevelGen(BaseElement* elem);

    static void initDefaultLayout();

    static NvpLayoutHead* getLayoutHead(NvpLayoutBody* current);

    NvpLayout(NvpLayoutBody& body, NvpLevel& level, NvpLevel::iterator iter)
        :layout_body(body), layout_level(level), layout_iter(iter)
    {
        layout_body.elem = nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////

    NvpLayoutBody layout_body;
    NvpLevel& layout_level;
    NvpLevel::iterator layout_iter;

    ////////////////////////////////////////////////////////////////////////////

    friend class ElemGenerator;
};

class ElemGenerator
{
public:
    ElemGenerator(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act,
        BaseElement* up = NvpLayout::g_top_layout ? NvpLayout::g_top_node_view : nullptr,
        bool be_top = true);

    ~ElemGenerator() {}

    BaseElement* gen_elem = nullptr;

private:
    ElemGenerator(const std::string& str, MsgBaseType msg_type,
        BaseAction* msg_act, NvpLayout::NvpLevel* level, bool be_top = true);

    friend class NvpLayout;
};
