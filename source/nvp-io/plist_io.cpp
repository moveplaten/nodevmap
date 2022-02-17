/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "plist_io.h"

void NvpPlistIO::inputAll(NvpPlistSeq* seq, NvpSysPort::File* file, const char* fallback)
{
    auto read = file->Read();
    file->Close();
    delete file;
    const char* xml_in = nullptr;
    if (read.buf == nullptr)
    {
        xml_in = fallback;
    }
    else
    {
        xml_in = read.buf;
    }

    NvpPlistPort root(xml_in);
    auto plist_array = root.queryDictKey("node");
    auto plist_mtx = root.queryDictKey("mtx");
    auto mtx_data = plist_mtx.getValData();

    NvpCodingMtx mtx;
    mtx.codingMtx(nullptr, mtx_data.data_ptr);
    NvpEventView::setCurrentMtx(mtx.current_mtx);

    NvpPlistIO io(seq);
    io.inputAllR(plist_array);
}

NvpBaseObj* NvpPlistIO::inputAllR(NvpPlistPort& plist, NvpBaseObj* base)
{
    uint32_t io_offset = 0;

    ++stack_level;
    assert(stack_level < MAX_STACK_LEVEL);

    for (;;)
    {
        if (plist.getType() != PLIST_ARRAY && plist.getType() != PLIST_NONE)
        {
            //io_seq->readSeq(plist, io_offset);
            io_seq->setRead(NvpPlistSeq::Read(plist, io_offset));
            io_seq->setType(NvpPlistSeq::READ);
            io_seq->procSeq();
            ++io_offset;
        }

        auto sub = NvpPlistPort::getSubFirst(plist);

        if (sub.getType() != PLIST_NONE)
        {
            stack_prev[stack_level + 1] = base;
            base = inputAllR(sub, base);
        }

        plist = NvpPlistPort::getNext(plist);

        if (plist.getType() == PLIST_NONE)
        {
            break;
        }
    }

    base = stack_prev[stack_level - 1];

    if (io_offset != 0)
    {
        base = io_seq->signalSeqEnd(base);
        prepareNewSeq();
    }

    --stack_level;

    return base;
}

void NvpPlistIO::prepareNewSeq()
{
    //io_seq->signalSeqEnd();

    auto seq_new = io_seq->createNewSeq();
    delete io_seq;
    io_seq = seq_new;
}

void NvpPlistIO::outputAll(NvpBaseObj* base, NvpPlistSeq* seq, NvpSysPort::File* file)
{
    auto plist_array = NvpPlistPort::newEmptyArray();
    NvpPlistIO io(seq);
    io.outputAllR(base, plist_array);

    NvpCodingMtx mtx;
    mtx.current_mtx = NvpEventView::event_view->current_mtx;
    std::string str;
    mtx.codingMtx(&str, nullptr);

    NvpPlistPort::ValData mtx_data(str.c_str(), str.size());
    auto plist_mtx = NvpPlistPort::newData(mtx_data);
    auto root = NvpPlistPort::newEmptyDict();
    root.insertDictKey(plist_mtx, "mtx");
    root.insertDictKey(plist_array, "node");

    auto xml = root.writeToXml();
    file->Write(xml.xml_str, xml.xml_len);
    file->Close();
    delete file;
}

void NvpPlistIO::outputAllR(NvpBaseObj* base, NvpPlistPort& plist)
{
    for (;;)
    {
        if (base)
        {
            auto array = NvpPlistPort::newEmptyArray();
            //io_seq->writeSeq(base, array);
            io_seq->setWrite(NvpPlistSeq::Write(base, array));
            io_seq->setType(NvpPlistSeq::WRITE);
            io_seq->procSeq();
            plist.pushArrayItem(array);
        }

        auto sub = NvpLayout::getSubFirst(base);

        if (sub)
        {
            auto sub_plist = NvpPlistPort::newEmptyArray();
            plist.pushArrayItem(sub_plist);
            outputAllR(sub, sub_plist);
        }

        base = NvpLayout::getNext(base);

        if (!base)
        {
            break;
        }
    }
}

NvpPlistIO::~NvpPlistIO()
{
    delete io_seq;
}
