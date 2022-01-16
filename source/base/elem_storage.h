/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef uint32_t elemIDSize;

template<elemIDSize N, typename T> class ElemStorage
{
public:
    elemIDSize storeOneElem(const T* content)
    {
        if (reused_count)
        {
            const elemIDSize id = reused_elems[--reused_count];
            contents[id] = *content;
            return id;
        }
        else
        {
            const elemIDSize id = current_max_used++;
            contents[id] = *content;
            return id;
        }
    }

    T* readOneElem(const elemIDSize id)
    {
        return contents + id;
    }

    void deleteOneElem(const elemIDSize id)
    {
        reused_elems[reused_count] = id;
        ++reused_count;
        memset(contents + id, 0, one_content_size); //set all to zero;
    }
    
    elemIDSize getTotalUsed() { return current_max_used - reused_count; }
    elemIDSize getTotalMax() { return current_max_used; }

    //ElemStorage(elemIDSize capacity, elemIDSize content_size)
    //    :max_capacity(capacity), one_content_size(content_size) {}
    ~ElemStorage()
    {
        free(contents);
        free(reused_elems);
    }

private:
    const T* getContents() { return contents; }
    const elemIDSize* getReused() { return reused_elems; }
    elemIDSize getReusedCount() { return reused_count; }

    const elemIDSize max_capacity = N;
    const elemIDSize one_content_size = sizeof(T);

    T* const contents //like BaseElemContent contents[max_capacity];
        = (T* const)malloc(max_capacity * one_content_size);
    elemIDSize current_max_used = 0;

    elemIDSize* const reused_elems
        = (elemIDSize* const)malloc(max_capacity * sizeof(elemIDSize));
    elemIDSize reused_count = 0;

    
#ifdef _DEBUG
    elemIDSize(*p_reused_elems)[1000] = (elemIDSize(*)[1000])reused_elems;
    T(*p_contents)[1000] = (T(*)[1000])contents; 
#endif
    //only debug use;
};


class ElemIDStorage
{
public:
    elemIDSize storeOneElem()
    {
        if (reused_count)
        {
            const elemIDSize id = reused_elems[--reused_count];
            return id;
        }
        else
        {
            const elemIDSize id = current_max_used++;
            return id;
        }
    }

    void deleteOneElem(const elemIDSize id)
    {
        reused_elems[reused_count] = id;
        ++reused_count;
    }
    
    elemIDSize getTotalUsed() { return current_max_used - reused_count; }
    elemIDSize getTotalMax() { return current_max_used; }

    ElemIDStorage() = default;
    
    ~ElemIDStorage()
    {
        free(reused_elems);
    }

private:
    const elemIDSize max_capacity = INT32_MAX / 1000;

    elemIDSize current_max_used = 0;

    elemIDSize* const reused_elems
        = (elemIDSize* const)malloc(max_capacity * sizeof(elemIDSize));
    
    elemIDSize reused_count = 0;

    
#ifdef _DEBUG
    elemIDSize(*p_reused_elems)[1000] = (elemIDSize(*)[1000])reused_elems;
#endif
    //only debug use;
};
