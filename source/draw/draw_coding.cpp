/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "draw_coding.h"

struct NvpCodingRect
{
    BaseRect rect;
    NVP_DRAW_CODING
    (
        NvpCoding::codingSeq(dst, src, &rect.left, &rect.top, &rect.right, &rect.bottom);
    )
};

/*------------------------------------------------------------------------------
!current draw_cache coding method
 draw_cmd | style | draw_data
------------------------------------------------------------------------------*/

void NvpDrawCache::encodeSeq(std::string* dst)
{
    const char draw_cmd = getCommand();
    dst->append(&draw_cmd, 1);
    a_style.drawCoding(dst, nullptr);
    OptSwitch(CODING, nullptr, dst);
}

NvpDrawCache NvpDrawCache::decodeSeq(const char* src)
{
    const char draw_cmd = *src;
    NvpStyle style;
    style.drawCoding(nullptr, src + 1);
    return NvpDrawCache(style, static_cast<NvpDrawData::Command>(draw_cmd), src + 1 + 5);
}

void NvpDrawSeq::procSeq()
{
    if (getType() == READ)
    {
        readSeq();
    }
    else
    {
        writeSeq();
    }
}

/*------------------------------------------------------------------------------
!current plist array data top method
<array>
    <data>
    LayoutRect
    </data>
    <data>
    DrawCache #0
    </data>
    <data>
    DrawCache #1
    </data>
    <data>
    DrawCache #2
    </data>
    <data>
    DrawCache ...
    </data>
</array>
------------------------------------------------------------------------------*/

BaseElement* NvpDrawSeq::signalSeqEnd(BaseElement* base)
{
    if (base == nullptr)
    {
        base = NvpLayout::getTopNodeView();
    }

    auto node = NvpLayout::subElemGen("", createNewEvent(), base);
    NvpLayout::setBaseRect(node, rect);

    draw_seq->base_elem = node;
    node->setSelfDraw(draw_seq);

    return node;
}

void NvpDrawSeq::readSeq()
{
    auto offset = getRead()->offset;
    auto& plist = getRead()->plist;

    if (offset == 0)
    {
        draw_seq = new NvpDraw(nullptr);

        auto rect_data = plist.getValData();
        assert(rect_data.data_len == 16);
        NvpCodingRect rect_temp;
        rect_temp.drawCoding(nullptr, rect_data.data_ptr);
        rect = rect_temp.rect;
    }
    else
    {
        auto cache_data = plist.getValData();
        auto draw_cache = NvpDrawCache::decodeSeq(cache_data.data_ptr);
        draw_seq->pushDraw(draw_cache);
    }
}

void NvpDrawSeq::writeSeq()
{
    auto base = getWrite()->base;

    if (!base) return;

    auto& plist = getWrite()->plist;

    std::string rect_str;
    NvpCodingRect rect{ base->getRectRefUp() };
    rect.drawCoding(&rect_str, nullptr);

    NvpPlistPort::ValData rect_dat(rect_str.c_str(), rect_str.size());
    auto rect_data = NvpPlistPort::newData(rect_dat);
    plist.pushArrayItem(rect_data);

    if (base->getSelfDraw() == nullptr) return;

    auto& cache = base->getSelfDraw()->draw_cache;
    auto cache_size = cache.size();
    for (size_t i = 0; i < cache_size; ++i)
    {
        std::string str;
        cache[i].encodeSeq(&str);

        NvpPlistPort::ValData dat(str.c_str(), str.size());
        auto cache_data = NvpPlistPort::newData(dat);
        plist.pushArrayItem(cache_data);
    }
}
