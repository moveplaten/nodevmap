#pragma once

#include "nvp_plist.h"
#include "base/base.h"

class NvpPlistSeq
{
public:
    virtual ~NvpPlistSeq() = default;

protected:
    virtual NvpPlistSeq* createNewSeq() = 0;
    virtual BaseElement* signalSeqEnd(BaseElement* base) = 0;
    virtual void readSeq(const NvpPlistPort& plist, uint32_t offset) = 0;
    virtual void writeSeq(BaseElement* base, NvpPlistPort& plist) = 0;

    friend class NvpPlistIO;
};

constexpr int MAX_STACK_LEVEL = 100;

class NvpPlistIO
{
public:
    NvpPlistIO(NvpPlistSeq* seq) : io_seq(seq), stack_level(-1), stack_prev{ nullptr } {}

    NvpPlistIO(const NvpPlistIO& io) = delete;
    NvpPlistIO& operator=(const NvpPlistIO& io) = delete;

    BaseElement* inputAll(const NvpPlistPort& plist, BaseElement* base = nullptr);

    NvpPlistPort outputAll(BaseElement* base);

    ~NvpPlistIO();

private:
    void prepareNewSeq();
    void outputAllR(BaseElement* base, NvpPlistPort& plist);

    int stack_level;
    NvpPlistSeq* io_seq;
    BaseElement* stack_prev[MAX_STACK_LEVEL];
};
