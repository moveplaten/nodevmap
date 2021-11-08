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

class NvpBuild
{
public:
    void setBaseRect(BaseElement* base, const BaseRect& rect);

    BaseElement* getSubFirst(BaseElement* base);

    BaseElement* getNext(BaseElement* base);
    
    BaseElement* getSubLast(BaseElement* base);

    BaseElement* getNextReverse(BaseElement* base);

    BaseElement* subElemGen(const std::string& str, MsgBaseType msg_type,
        BaseAction* msg_act, BaseElement* up, bool be_top = true);

    void subElemDel(BaseElement* elem);

    void moveToAllTop(BaseElement* elem);

    BaseElement* const g_top_layout;    //0; the whole client area;
    BaseElement* const g_top_node_view; //1;
    BaseElement* const g_top_menu_stat; //1;

private:
    union NvpLayout;
    struct NvpLayoutHead;
    struct NvpLayoutBody;

    typedef ElemStorage<100000, NvpLayoutBody> AllElem;
    typedef std::map<std::string, BaseElement*> ElemMap;
    typedef std::list<NvpLayout*> NvpLevel;

    NvpBuild(BaseElement* const top_layout, BaseElement* const top_node_view,
        BaseElement* const top_menu_stat, AllElem* const all_elem_store)
        :g_top_layout(top_layout), g_top_node_view(top_node_view),
        g_top_menu_stat(top_menu_stat), g_all_elem_store(all_elem_store) {}

    BaseElement* elemGen(const std::string& str, MsgBaseType msg_type,
        BaseAction* msg_act, NvpLevel* level, bool be_top = true);

    bool elemDel(const std::string& str, NvpLevel* level);

    NvpLevel* subLevelGen(BaseElement* elem);

    void initDefaultLayout(AllElem* const all_elem);

    NvpLayoutHead* getLayoutHead(NvpLayoutBody* current);

    AllElem* const g_all_elem_store;

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

    union NvpLayout
    {
        NvpLayoutHead* head; //head is always first, only first one is valid;
        NvpLayoutBody body;
    };

    friend class ElemGenerator;
    friend class BaseElement;
};


/// GLOBAL nvpBuild;
extern NvpBuild* nvpBuild;


class ElemGenerator
{
public:
    ElemGenerator(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act,
        BaseElement* up = nvpBuild ? nvpBuild->g_top_node_view : nullptr,
        bool be_top = true);

    ~ElemGenerator() {}

    BaseElement* gen_elem = nullptr;

private:
    ElemGenerator(const std::string& str, MsgBaseType msg_type,
        BaseAction* msg_act, NvpBuild::NvpLevel* level, bool be_top = true);

    friend class NvpBuild;
};
