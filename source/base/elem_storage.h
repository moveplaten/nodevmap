#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef uint64_t elemIDSize;

class ElemStorage
{
public:
    elemIDSize storeOneElem(const void* content);
    const void* readOneElem(const elemIDSize id);
    //void changeOneElem(const elemIDSize id, const void* content);
    void deleteOneElem(const elemIDSize id);
    
    elemIDSize getTotalUsed() { return current_max_used - reused_count; }
    elemIDSize getTotalMax() { return current_max_used; }

    ElemStorage(elemIDSize capacity, elemIDSize content_size)
        :max_capacity(capacity), one_content_size(content_size) {}
    ~ElemStorage() {}

private:
    struct ElemContent {};
    void store(const void* dst, const void* src);
    void* getContents() { return contents; }
    elemIDSize* getReused() { return reused_elems; }
    elemIDSize getReusedCount() { return reused_count; }

    const elemIDSize max_capacity;
    const elemIDSize one_content_size;

    ElemContent* const contents //like BaseElemContent contents[max_capacity];
        = (ElemContent* const)malloc(max_capacity * one_content_size);
    elemIDSize current_max_used = 0;

    elemIDSize* const reused_elems
        = (elemIDSize* const)malloc(max_capacity * sizeof(elemIDSize));
    elemIDSize reused_count = 0;

    struct BaseShape
    {
        long l;
        long t;
        long r;
        long b;
        void* unknown;
    };
    elemIDSize(*p_reused_elems)[1000] = (elemIDSize(*)[1000])reused_elems;
    BaseShape(*p_contents)[1000] = (BaseShape(*)[1000])contents; //only debug use;

    friend class BaseMessage;
};
