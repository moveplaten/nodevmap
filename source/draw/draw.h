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
    NoneDraw,
    Draw
};

class NvpDrawPort
{
public:
    virtual void beginDraw() = 0;
    
    virtual void fillRect(const BaseRect& rect, NvpColor colo) = 0;
    virtual void frameRect(const BaseRect& rect, NvpColor colo) = 0; //only border;
    
};

class NvpDraw
{
public:
    void Record(BaseElement* base, NvpColor* cols = nullptr,
        RecordOption opt = NoneDraw,
        const BaseRect* rect = nullptr, NvpDraw* draw = nullptr);

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


class NvpFrameOneRect : public NvpDraw
{
public:
    virtual void realDraw(BaseElement* base) override;
    virtual void setColor(NvpColor* cols) override
    {
        color = cols[0];
    }

    NvpColor color;
};

class NvpFrameFiveRect : public NvpDraw
{
public:
    NvpFrameFiveRect(int p /*1~50*/)
    {
        percent = p / 100.0f;
    }
    void setPersent(int p) { percent = p / 100.0f; }
    virtual void realDraw(BaseElement* base) override;
    virtual void setColor(NvpColor* cols) override
    {
        color[0] = cols[0];
        color[1] = cols[1];
    }

    NvpColor color[2];
    float percent;
};

class NvpFillOneRect : public NvpDraw
{
public:
    virtual void realDraw(BaseElement* base) override;
    virtual void setColor(NvpColor* cols) override
    {
        color = cols[0];
    }

    NvpColor color;
};

class NvpFillFiveRect : public NvpDraw
{
public:
    NvpFillFiveRect(int p /*1~50*/)
    {
        percent = p / 100.0f;
    }
    void setPersent(int p) { percent = p / 100.0f; }
    virtual void realDraw(BaseElement* base) override;
    virtual void setColor(NvpColor* cols) override
    {
        color[0] = cols[0];
        color[1] = cols[1];
    }

    NvpColor color[2];
    float percent;
};
