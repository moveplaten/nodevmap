#include "elem_storage.h"

elemIDSize ElemStorage::storeOneElem(const ElemContent* content)
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

const ElemContent* ElemStorage::readOneElem(const elemIDSize id)
{
    return contents + id * one_content_size;
}

void ElemStorage::changeOneElem(const elemIDSize id, const ElemContent* content)
{
    store(contents + id * one_content_size, content);
}

void ElemStorage::deleteOneElem(const elemIDSize id)
{
    reused_elems[reused_count] = id;
    ++reused_count;
}

void ElemStorage::store(const ElemContent* dst, const ElemContent* src)
{
    memcpy((void*)dst, src, one_content_size);
}
