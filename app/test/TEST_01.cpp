#include "test_plist_01.h"

class TopNodeView : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        NvpColor col = { 40, 40, 45 };
        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);
    }

    void mouseLDown(BaseElement* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("RightMouseMenu", base);
        elem->getSelfEvent()->mouseLDown(elem, param);
    }

    void mouseRDown(BaseElement* base, NvpEventParam& param) override
    {
        auto elem = NvpLayout::findSameLevel("RightMouseMenu", base);
        elem->getSelfEvent()->mouseRDown(elem, param);
    }
};

////////////////////////////////////////////////////////////////////////////////

class RandomColorNode : public NvpEvent
{
    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        static double tr, tg, tb;
        tr += 0.3; tg += 0.2, tb += 0.1;
        int r = 255 * sin(tr + rand() / 1000);
        int g = 255 * cos(tg + rand() / 1000);
        int b = 255 * sin(tb + rand() / 1000);
        NvpColor col = { static_cast<uint8_t>(abs(r)), static_cast<uint8_t>(abs(g)), static_cast<uint8_t>(abs(b)) };

        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);

        auto world_pt = param.getWorldPt();
        BaseRect rect;
        rect.left = world_pt.x;
        rect.top = world_pt.y;
        rect.right = rect.left + 60;
        rect.bottom = rect.top + 60;
        NvpLayout::setBaseRect(base, rect);
    }
};

class RandomColorNode2 : public NvpEvent
{
public:
    static void userInit2(BaseElement* base, BaseRect rect, NvpColor colo)
    {
        base->setSelfDraw(new NvpDraw(base));
        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(colo);
        base->getSelfDraw()->pushDraw(one_rect);
        NvpLayout::setBaseRect(base, rect);
    }
};

////////////////////////////////////////////////////////////////////////////////

static const char* io_file_name = "test_01.plist";

class PlistSeqRecColo : public NvpPlistSeq
{
protected:
    NvpPlistSeq* createNewSeq() override
    {
        return new PlistSeqRecColo;
    }

    void signalSeqEnd() override
    {
        static uint32_t order = 0;
        auto node_name = std::to_string(order); ++order;
        auto node = NvpLayout::subElemGen(node_name, new RandomColorNode2, NvpLayout::getTopNodeView());
        RandomColorNode2::userInit2(node, rect, colo);
    }

    void readSeq(const NvpPlistPort& plist, uint32_t offset) override
    {
        switch (offset)
        {
        case 0:
            rect.left = plist.getValReal();
            break;

        case 1:
            rect.top = plist.getValReal();
            break;

        case 2:
            rect.right = plist.getValReal();
            break;

        case 3:
            rect.bottom = plist.getValReal();
            break;

        case 4:
            colo.Red = plist.getValUint();
            break;

        case 5:
            colo.Green = plist.getValUint();
            break;

        case 6:
            colo.Blue = plist.getValUint();
            break;

        case 7:
            colo.Alpha = plist.getValUint();
            break;

        default:
            break;
        }
    }

    void writeSeq(BaseElement* base, NvpPlistPort& plist) override
    {
        auto array = NvpPlistPort::newEmptyArray();
        auto rect = base->getRectRefUp();
        auto colo = base->getSelfDraw()->getDraw(0)->getColor();
        auto rect_l = NvpPlistPort::newReal(rect.left);
        auto rect_t = NvpPlistPort::newReal(rect.top);
        auto rect_r = NvpPlistPort::newReal(rect.right);
        auto rect_b = NvpPlistPort::newReal(rect.bottom);
        auto colo_r = NvpPlistPort::newUint(colo.Red);
        auto colo_g = NvpPlistPort::newUint(colo.Green);
        auto colo_b = NvpPlistPort::newUint(colo.Blue);
        auto colo_a = NvpPlistPort::newUint(colo.Alpha);
        array.pushArrayItem(rect_l);
        array.pushArrayItem(rect_t);
        array.pushArrayItem(rect_r);
        array.pushArrayItem(rect_b);
        array.pushArrayItem(colo_r);
        array.pushArrayItem(colo_g);
        array.pushArrayItem(colo_b);
        array.pushArrayItem(colo_a);
        plist.pushArrayItem(array);
    }

private:
    BaseRect rect{ 0 };
    NvpColor colo{ 0 };
};

static void saveAllNode()
{
    auto io = NvpPlistIO(new PlistSeqRecColo);
    auto node1 = NvpLayout::getSubFirst(NvpLayout::getTopNodeView());
    auto out = io.outputAll(node1);
    auto xml = out.writeToXml();
    NvpUtil::writeExePath(xml.xml_str, xml.xml_len, io_file_name, "wb");
}

static void initAllNode()
{
    const char* xml_in = nullptr;
    auto read = NvpUtil::readExePath(io_file_name, "rb");
    if (read.buf == nullptr)
    {
        xml_in = plist_array_01;
    }
    else
    {
        xml_in = read.buf;
    }
    NvpPlistPort root(xml_in);

    auto io = NvpPlistIO(new PlistSeqRecColo);
    io.inputAll(root);
}

////////////////////////////////////////////////////////////////////////////////

class RightMouseMenu : public NvpEvent
{
    void clearMenu(BaseElement* base)
    {
        auto font = base->getSelfDraw()->getDraw(1);
        font->text_left_right->setStart({ 0, 0 });
        font->text_left_right->setText(" ");

        font = base->getSelfDraw()->getDraw(2);
        font->text_left_right->setStart({ 0, 0 });
        font->text_left_right->setText(" ");

        NvpLayout::setBaseRect(base, { 0 });
    }

    void userInit(BaseElement* base, NvpEventParam& param) override
    {
        NvpColor col = { 120, 120, 120 };
        NvpDrawCache one_rect(NvpDrawData::Rect_Same_Elem);
        one_rect.setColor(col);
        base->getSelfDraw()->pushDraw(one_rect);

        NvpColor col_font = { 230, 230, 230 };
        NvpDrawCache font(NvpDrawData::Text_Left_Right);
        font.setColor(col_font);
        base->getSelfDraw()->pushDraw(font);

        NvpDrawCache font2(NvpDrawData::Text_Left_Right);
        font2.setColor(col_font);
        base->getSelfDraw()->pushDraw(font2);
    }

    void mouseLDown(BaseElement* base, NvpEventParam& param) override
    {
        auto world_pt = param.getWorldPt();
        if (NvpLayout::ptInRect(world_pt, rect_up))
        {
            clearMenu(base);
            auto order = NvpLayout::getIDStorage()->getTotalUsed();
            auto node_name = std::to_string(order); ++order;
            auto node = NvpLayout::subElemGen(node_name, new RandomColorNode, NvpLayout::getTopNodeView());
            NvpEvent::initAll(node, param);
        }
        else if (NvpLayout::ptInRect(world_pt, rect_dn))
        {
            clearMenu(base);
            saveAllNode();
        }
    }

    void mouseRDown(BaseElement* base, NvpEventParam& param) override
    {
        auto world_pt = param.getWorldPt();
        auto font = base->getSelfDraw()->getDraw(1);
        font->text_left_right->setStart({ 10, 12 });
        font->text_left_right->setText("NEW  NODE");

        auto font2 = base->getSelfDraw()->getDraw(2);
        font2->text_left_right->setStart({ 10, 28 });
        font2->text_left_right->setText("SAVE  ALL");

        BaseRect rect1;
        rect1.left = world_pt.x;
        rect1.top = world_pt.y;
        rect1.right = rect1.left + 90;
        rect1.bottom = rect1.top + 15;
        rect_up = rect1;
        BaseRect rect2;
        rect2.left = rect1.left;
        rect2.top = rect1.bottom;
        rect2.right = rect1.right;
        rect2.bottom = rect2.top + 15;
        rect_dn = rect2;
        rect1.bottom += 15;
        NvpLayout::setBaseRect(base, rect1);
    }

    BaseRect rect_up;
    BaseRect rect_dn;
};

////////////////////////////////////////////////////////////////////////////////

void nvp_app_init()
{
    NvpLayout::subElemGen("top_node_view", new TopNodeView, NvpLayout::getTopLayout());
    NvpLayout::subElemGen("RightMouseMenu", new RightMouseMenu, NvpLayout::getTopLayout());
    initAllNode();
}
