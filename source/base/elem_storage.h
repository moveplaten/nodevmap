#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef uint64_t elemIDSize;

class ElemContent
{
public:
    ElemContent() {}
    ~ElemContent() {}

private:

};


class ElemStorage
{
public:
    elemIDSize storeOneElem(const ElemContent* content);
    const ElemContent* readOneElem(const elemIDSize id);
    void changeOneElem(const elemIDSize id, const ElemContent* content);
    void deleteOneElem(const elemIDSize id);
    ElemContent* getContents() { return contents; }
    elemIDSize getTotalUsed() { return current_max_used - reused_count; }

    ElemStorage(elemIDSize capacity, elemIDSize content_size)
        :max_capacity(capacity), one_content_size(content_size) {}
    ~ElemStorage() {}

private:
    void store(const ElemContent* dst, const ElemContent* src);
    const elemIDSize max_capacity;
    const elemIDSize one_content_size;

    ElemContent* const contents //like BaseElemContent contents[max_capacity];
        = (ElemContent* const)malloc(max_capacity * one_content_size);
    elemIDSize current_max_used = 0;

    elemIDSize* const reused_elems
        = (elemIDSize* const)malloc(max_capacity * sizeof(elemIDSize));
    elemIDSize reused_count = 0;
};
