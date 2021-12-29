#include "plist_io.h"

void NvpPlistIO::inputAll(NvpPlistPort& plist, NvpPlistSeq* seq)
{
    NvpPlistIO io(seq);
    io.inputAllR(plist);
}

BaseElement* NvpPlistIO::inputAllR(NvpPlistPort& plist, BaseElement* base)
{
    int io_offset = 0;

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

NvpPlistPort NvpPlistIO::outputAll(BaseElement* base, NvpPlistSeq* seq)
{
    auto root = NvpPlistPort::newEmptyArray();
    NvpPlistIO io(seq);
    io.outputAllR(base, root);

    return root;
}

void NvpPlistIO::outputAllR(BaseElement* base, NvpPlistPort& plist)
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
