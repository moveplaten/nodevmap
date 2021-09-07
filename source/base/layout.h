#pragma once

#include "base.h"

typedef std::map<std::string, BaseElement*> ElemMap;

#define ELEM_GEN(x, y, z)\
ElemGenerator x##y##z(#x, y, &z);

class ElemGenerator
{
public:
    ElemGenerator::ElemGenerator(const std::string& str,
        MsgBaseType msg_type, BaseAction* msg_act);

    ElemGenerator::~ElemGenerator() {}

//private:
    static ElemMap* g_node_view_map;
};

BaseElement* elemGen(const std::string& str, MsgBaseType msg_type, BaseAction* msg_act);

bool elemDel(const std::string& str);

typedef ElemStorage<100000, BaseShape> NodeView;
typedef ElemStorage<1000, BaseShape> MenuBar;
typedef ElemStorage<1000, BaseShape> StatusBar;

extern NodeView* g_node_view;
extern MenuBar* g_menu_bar;
extern StatusBar* g_status_bar;
