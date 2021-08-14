#include "elem_storage.h"

elemIDSize ElemStorage::storeOneElem(const void* content)
{
    if (reused_count)
    {
        elemIDSize id = reused_elems[--reused_count];
        store(contents + id * one_content_size, content);
        return id;
    }
    else
    {
        elemIDSize id = current_max_used++;
        store(contents + id * one_content_size, content);
        return id;
    }
}

const void* ElemStorage::readOneElem(const elemIDSize id)
{
    return contents + id * one_content_size;
}

void ElemStorage::deleteOneElem(const elemIDSize id)
{
    reused_elems[reused_count] = id;
    ++reused_count;
}

void ElemStorage::store(const void* dst, const void* src)
{
    memcpy((void*)dst, src, one_content_size);
}
