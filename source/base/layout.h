#pragma once

#include <string>
#include <list>
#include <map>
#include "elem_storage.h"
#include "draw/draw.h"

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

struct BaseRect
{
    float left;
    float top;
    float right;
    float bottom;
};

class BaseElement;
class BaseAction;

#define ELEM_GEN(x, y, z)\
ElemGenerator x##y##z(#x, y, &z);

#define ELEM_GEN_FULL(x, y, z, l)\
ElemGenerator x##y##z##l(#x, y, &z, l);

union NvpLayout;
struct NvpLayoutHead;
struct NvpLayoutBody;
typedef std::list<NvpLayout*> NvpLevel;

extern NvpLevel* g_top_layout; //the whole client area, should init global;

//below are all default;
extern NvpLevel* g_top_node_view;
extern NvpLevel* g_top_menu_bar;
extern NvpLevel* g_top_status_bar;

class ElemGenerator
{
public:
    ElemGenerator::ElemGenerator(const std::string& str,
        MsgBaseType msg_type, BaseAction* msg_act, NvpLevel* level = g_top_node_view);

    ElemGenerator::~ElemGenerator() {}

    void initDefaultLayout();
};

BaseElement* elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act,
    NvpLevel* level = g_top_node_view);

bool elemDel(const std::string& str);

NvpLevel* subLevelGen(BaseElement* elem);

void subLevelDel(BaseElement* elem);

NvpLayoutHead* getLayoutHead(NvpLayoutBody* current);

typedef ElemStorage<100000, NvpLayoutBody> AllElem;
typedef std::map<std::string, BaseElement*> ElemMap;

extern AllElem* g_all_elem_store;
extern ElemMap* g_all_elem_map;

struct NvpLayoutBody
{
    BaseRect rect; //local space, ref to up level;
    BaseElement* elem;
    NvpDraw* draw;
    NvpLevel* sub; //sub level;
};

struct NvpLayoutHead
{
    void* null_head; //should be first?
    BaseElement* up_elem;
    NvpLevel* up_level;
    uint32_t cur_depth;
};

union NvpLayout
{
    NvpLayoutHead* head; //head is always first, only first one is valid;
    NvpLayoutBody body;
};
