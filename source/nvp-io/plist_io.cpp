#include "plist_io.h"

void NvpPlistIO::inputAll(const NvpPlistPort& plist)
{
    io_offset = 0;

    for (;;)
    {
        if (plist.getType() != PLIST_ARRAY && plist.getType() != PLIST_NONE)
        {
            io_seq->readSeq(plist, io_offset);
        }

        auto sub = NvpPlistPort::getSubFirst(plist);

        if (sub.getType() != PLIST_NONE)
        {
            prepareNewSeq();
            inputAll(sub);
        }

        const_cast<NvpPlistPort&>(plist) = NvpPlistPort::getNext(plist);

        if (plist.getType() == PLIST_NONE)
        {
            io_offset = 0;
            io_seq->signalSeqEnd();
            prepareNewSeq();
            break;
        }

        ++io_offset;
    }
}

void NvpPlistIO::prepareNewSeq()
{
    //io_seq->signalSeqEnd();

    auto seq_new = io_seq->createNewSeq();
    delete io_seq;
    io_seq = seq_new;
}

NvpPlistPort NvpPlistIO::outputAll(BaseElement* base)
{
    auto root = NvpPlistPort::newEmptyArray();
    outputAllR(base, root);

    return root;
}

void NvpPlistIO::outputAllR(BaseElement* base, NvpPlistPort& plist)
{
    for (;;)
    {
        if (base)
        {
            io_seq->writeSeq(base, plist);
        }

        auto sub = NvpLayout::getSubFirst(base);

        if (sub)
        {
            outputAllR(sub, plist);
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
