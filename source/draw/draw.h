#pragma once

#include <vector>
#include <string>

struct BaseRect;

class BaseElement;

struct NvpColor
{
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
};

struct NvpXyCoord
{
    float x;
    float y;
};

enum RecordOption
{
    NoneDraw,
    Draw
};

class NvpDrawPort
{
public:
    virtual void beginDraw() = 0;

    //draw all str from left to right just one line;
    //start from base rect left-top coord;
    virtual void drawTextFromLToR(NvpXyCoord start, const std::string& str, NvpColor colo) = 0;

    virtual void fillRect(const BaseRect& rect, NvpColor colo) = 0;
    virtual void frameRect(const BaseRect& rect, NvpColor colo) = 0; //only border;
    
};

class NvpDraw
{
public:
    void Record(BaseElement* base, size_t offset, NvpColor* cols = nullptr,
        RecordOption opt = NoneDraw,
        const BaseRect* rect = nullptr, NvpDraw* draw = nullptr);

    NvpDraw* getOneDraw(BaseElement* base, size_t offset); //offset from 0;

    void drawAll(BaseElement* base);
    
    NvpDrawPort* getDrawPort() { return g_draw_port; }

    void initDrawPort(NvpDrawPort* draw_port)
    {
        if (!g_draw_port)
        {
            g_draw_port = draw_port;
        }
    }

    NvpDraw()
    {
    }

    ~NvpDraw()
    {
    }

protected:
    virtual void realDraw(BaseElement* base) = 0;
    virtual void setColor(NvpColor* cols) = 0;

    static NvpDrawPort* g_draw_port;
};


/// GLOBAL nvpDraw;
extern NvpDraw* const nvpDraw;


class NvpDrawOneColor : public NvpDraw
{
public:
    virtual void setColor(NvpColor* cols) override
    {
        color = cols[0];
    }

    NvpColor color;
};

class NvpDrawTwoColor : public NvpDraw
{
public:
    virtual void setColor(NvpColor* cols) override
    {
        color[0] = cols[0];
        color[1] = cols[1];
    }

    NvpColor color[2];
};

class NvpDrawTextOneLineOneColor : public NvpDrawOneColor
{
public:
    virtual void realDraw(BaseElement* base) override;

    NvpXyCoord start_pt = { 0 };
    std::string str_to_draw;
};

class NvpFrameOneRect : public NvpDrawOneColor
{
public:
    virtual void realDraw(BaseElement* base) override;
};

class NvpFrameFiveRect : public NvpDrawTwoColor
{
public:
    NvpFrameFiveRect(int p /*1~50*/)
    {
        percent = p / 100.0f;
    }
    void setPersent(int p) { percent = p / 100.0f; }
    virtual void realDraw(BaseElement* base) override;

    float percent;
};

class NvpFillOneRect : public NvpFrameOneRect
{
public:
    virtual void realDraw(BaseElement* base) override;
};

class NvpFillFiveRect : public NvpFrameFiveRect
{
public:
    NvpFillFiveRect(int p /*1~50*/) : NvpFrameFiveRect(p) {}
    
    virtual void realDraw(BaseElement* base) override;
};
