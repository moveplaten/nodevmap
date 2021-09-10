#pragma once

#include <vector>

struct BaseRect;
class BaseElement;

struct NvpColor
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
};

enum RecordOption
{
    Begin,
    End,
    BeginEnd,
    None,
};

class NvpDraw
{
public:
    void Record(const BaseRect& rec, NvpColor col, RecordOption opt = BeginEnd);

    NvpDraw::NvpDraw()
    {
    }

    NvpDraw::~NvpDraw()
    {
    }

    void realDraw();

    void deleteSelf()
    {
        delete this;
    }

    int record_offset = 0;
    std::vector<BaseRect> recs;
    std::vector<NvpColor> cols;
    BaseElement* elem = nullptr;

protected:
    virtual void doBegin() = 0;
    virtual void doDraw() = 0;
    virtual void doEnd() = 0;

private:

};
