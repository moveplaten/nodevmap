/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#pragma once

#include "nvp_plist.h"
#include "base/nvp_base.h"
#include "util/nvp_util.h"

class NvpPlistSeq
{
public:
    enum ProcType { READ, WRITE };

    struct Read
    {
        Read(const NvpPlistPort& plist_, uint32_t offset_) :
            plist(plist_), offset(offset_) {}

        const NvpPlistPort& plist;
        uint32_t offset;
    };

    struct Write
    {
        Write(NvpBaseObj* base_, const NvpPlistPort& plist_) :
            base(base_), plist(plist_) {}

        NvpBaseObj* base;
        const NvpPlistPort& plist;
    };

    const Read* getRead() const { return read; }
    const Write* getWrite() const { return write; }
    ProcType getType() const { return type; }

    virtual ~NvpPlistSeq() = default;

protected:
    virtual NvpPlistSeq* createNewSeq() = 0;
    virtual NvpBaseObj* signalSeqEnd(NvpBaseObj* base) = 0;
    //virtual void readSeq(const NvpPlistPort& plist, uint32_t offset) = 0;
    //virtual void writeSeq(NvpBaseObj* base, NvpPlistPort& plist) = 0;
    virtual void procSeq() = 0;

private:
    void setRead(const Read& read_) { read = &read_; }
    void setWrite(const Write& write_) { write = &write_; }
    void setType(ProcType type_) { type = type_; }

    ProcType type;
    const Read* read{ nullptr };
    const Write* write{ nullptr };

    friend class NvpPlistIO;
};

constexpr int MAX_STACK_LEVEL = 100;

class NvpPlistIO
{
public:
    NvpPlistIO(NvpPlistSeq* seq) : io_seq(seq), stack_level(-1), stack_prev{ nullptr } {}

    NvpPlistIO(const NvpPlistIO& io) = delete;
    NvpPlistIO& operator=(const NvpPlistIO& io) = delete;

    static void inputAll(NvpPlistSeq* seq, NvpSysPort::File* file, const char* fallback);

    static void outputAll(NvpBaseObj* base, NvpPlistSeq* seq, NvpSysPort::File* file);

    ~NvpPlistIO();

private:
    void prepareNewSeq();
    void outputAllR(NvpBaseObj* base, NvpPlistPort& plist);
    NvpBaseObj* inputAllR(NvpPlistPort& plist, NvpBaseObj* base = nullptr);

    int stack_level;
    NvpPlistSeq* io_seq;
    NvpBaseObj* stack_prev[MAX_STACK_LEVEL];
};
