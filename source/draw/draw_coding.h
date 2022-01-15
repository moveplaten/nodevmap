#pragma once

#include "draw_priv.h"
#include "nvp-io/plist_io.h"

class NvpDrawSeq : public NvpPlistSeq
{
public:
    NvpDrawSeq() : draw_seq(nullptr), rect{ 0 } {}

protected:
    virtual NvpEvent* createNewEvent() = 0;
    //virtual NvpPlistSeq* createNewSeq() = 0;

    virtual void procSeq() override;
    virtual BaseElement* signalSeqEnd(BaseElement* base) override;

private:
    void readSeq();
    void writeSeq();

    NvpDraw* draw_seq;
    BaseRect rect;
};
