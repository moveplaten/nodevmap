#pragma once

#include "nvp_plist.h"
#include "base/base.h"

class NvpPlistSeq
{
public:
    virtual ~NvpPlistSeq() = default;

protected:
    virtual NvpPlistSeq* createNewSeq() = 0;
    virtual void signalSeqEnd() = 0;
    virtual void readSeq(const NvpPlistPort& plist, uint32_t offset) = 0;
    virtual void writeSeq(BaseElement* base, NvpPlistPort& plist) = 0;

    friend class NvpPlistIO;
};

class NvpPlistIO
{
public:
    NvpPlistIO(NvpPlistSeq* seq) : io_seq(seq), io_offset(0) {}

    void inputAll(const NvpPlistPort& plist);

    NvpPlistPort outputAll(BaseElement* base);

    ~NvpPlistIO();

private:
    void prepareNewSeq();
    void outputAllR(BaseElement* base, NvpPlistPort& plist);

    uint32_t io_offset;
    NvpPlistSeq* io_seq;
};
