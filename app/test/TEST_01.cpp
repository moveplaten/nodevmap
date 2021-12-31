#include "test_plist_03.h"

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

static bool hit_node = false;
static BaseElement* hit_elem = nullptr;

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
        auto up = NvpLayout::getUpElem(base);
        auto rect_up = up->getRectRefTop();
        auto width = rect_up.right - rect_up.left;
        auto height = rect_up.bottom - rect_up.top;
        BaseRect rect;
        rect.left = world_pt.x - rect_up.left;
        rect.top = world_pt.y - rect_up.top;
        
        if (up->getSelfName() == "top_node_view")
        {
            rect.right = rect.left + 150;
            rect.bottom = rect.top + 150;
        }
        else
        {
            rect.right = rect.left + width * 0.5;
            rect.bottom = rect.top + height * 0.5;
        }

        NvpLayout::setBaseRect(base, rect);
    }

    void mouseRDown(BaseElement* base, NvpEventParam& param) override
    {
        auto right_menu = NvpLayout::findSameLevel("RightMouseMenu", NvpLayout::getTopNodeView());
        if (right_menu)
        {
            hit_node = true;
            right_menu->getSelfEvent()->mouseRDown(right_menu, param);
            hit_elem = base;
            hit_node = false;
        }
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

    void mouseRDown(BaseElement* base, NvpEventParam& param) override
    {
        auto right_menu = NvpLayout::findSameLevel("RightMouseMenu", NvpLayout::getTopNodeView());
        if (right_menu)
        {
            hit_node = true;
            right_menu->getSelfEvent()->mouseRDown(right_menu, param);
            hit_elem = base;
            hit_node = false;
        }
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

    BaseElement* signalSeqEnd(BaseElement* base) override
    {
        if (base == nullptr)
        {
            base = NvpLayout::getTopNodeView();
        }
        auto node = NvpLayout::subElemGen("", new RandomColorNode2, base);
        RandomColorNode2::userInit2(node, rect, colo);
        return node;
    }

    void encode32Bit(char dst[4], const uint32_t& bit_32)
    {
        uint8_t* const rep = reinterpret_cast<uint8_t*>(dst);

        rep[0] = static_cast<uint8_t>(bit_32);
        rep[1] = static_cast<uint8_t>(bit_32 >> 8);
        rep[2] = static_cast<uint8_t>(bit_32 >> 16);
        rep[3] = static_cast<uint8_t>(bit_32 >> 24);
    }

    void decode32Bit(const char src[4], uint32_t& bit_32)
    {
        const uint8_t* const rep = reinterpret_cast<const uint8_t*>(src);

        bit_32 = (static_cast<uint32_t>(rep[0])) |
            (static_cast<uint32_t>(rep[1]) << 8) |
            (static_cast<uint32_t>(rep[2]) << 16) |
            (static_cast<uint32_t>(rep[3]) << 24);
    }

    template<typename T>
    void procSeqT(T& t, NvpPlistPort::PlistType type)
    {
        if (getType() == READ)
        {
            auto data = getRead()->plist.getValData();
            T temp;
            decode32Bit(data.data_ptr, reinterpret_cast<uint32_t&>(temp));
            t = temp;
        }
        else
        {
            char c[4] = { 0 };
            encode32Bit(c, reinterpret_cast<const uint32_t&>(t));
            NvpPlistPort::ValData dat(c, 4);
            auto data = NvpPlistPort::newData(dat);
            getWrite()->plist.pushArrayItem(data);
        }
    }

    void switchOffset(int offset)
    {
        switch (offset)
        {
        case 0:
            procSeqT(rect.left, PLIST_REAL);
            break;

        case 1:
            procSeqT(rect.top, PLIST_REAL);
            break;

        case 2:
            procSeqT(rect.right, PLIST_REAL);
            break;

        case 3:
            procSeqT(rect.bottom, PLIST_REAL);
            break;

        case 4:
            procSeqT(colo, PLIST_UINT);
            break;

        default:
            break;
        }
    }

    void procSeq() override
    {
        if (getType() == READ)
        {
            switchOffset(getRead()->offset);
        }
        else
        {
            rect = getWrite()->base->getRectRefUp();
            colo = getWrite()->base->getSelfDraw()->getDraw(0)->getColor();
            for (int i = 0; i < offset_count; ++i)
            {
                switchOffset(i);
            }
        }
    }

private:
    const int offset_count = 5;
    BaseRect rect{ 0 };
    NvpColor colo{ 0 };
};

static void saveAllNode()
{
    auto node1 = NvpLayout::getSubFirst(NvpLayout::getTopNodeView());
    auto out = NvpPlistIO::outputAll(node1, new PlistSeqRecColo);
    auto xml = out.writeToXml();
    NvpUtil::writeExePath(xml.xml_str, xml.xml_len, io_file_name, "wb");
}

static void initAllNode()
{
    const char* xml_in = nullptr;
    auto read = NvpUtil::readExePath(io_file_name, "rb");
    if (read.buf == nullptr)
    {
        xml_in = plist_array_03;
    }
    else
    {
        xml_in = read.buf;
    }
    NvpPlistPort root(xml_in);
    NvpPlistIO::inputAll(root, new PlistSeqRecColo);
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

        font = base->getSelfDraw()->getDraw(3);
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

        NvpDrawCache font3(NvpDrawData::Text_Left_Right);
        font3.setColor(col_font);
        base->getSelfDraw()->pushDraw(font3);
    }

    void mouseLDown(BaseElement* base, NvpEventParam& param) override
    {
        auto world_pt = param.getWorldPt();
        if (NvpLayout::ptInRect(world_pt, rect_new))
        {
            clearMenu(base);
            BaseElement* node = nullptr;
            if (hit_elem->getSelfName() != "RightMouseMenu")
            {
                node = NvpLayout::subElemGen("", new RandomColorNode, hit_elem);
            }
            else
            {
                node = NvpLayout::subElemGen("", new RandomColorNode, NvpLayout::getTopNodeView());
            }
            NvpEvent::initAll(node, param);
        }
        else if (NvpLayout::ptInRect(world_pt, rect_save))
        {
            clearMenu(base);
            saveAllNode();
        }
        else if (NvpLayout::ptInRect(world_pt, rect_dele))
        {
            clearMenu(base);
            if (hit_elem)
            {
                if (hit_elem->getSelfName() == "RightMouseMenu")
                {
                    hit_elem = nullptr;
                    return;
                }
            }
            NvpLayout::subElemDel(hit_elem);
            hit_elem = nullptr;
        }
    }

    void mouseRDown(BaseElement* base, NvpEventParam& param) override
    {
        hit_elem = base;
        auto world_pt = param.getWorldPt();
        auto font = base->getSelfDraw()->getDraw(1);
        font->text_left_right->setStart({ 10, 12 });
        if (hit_node)
        {
            font->text_left_right->setText("SUB  NODE");
        }
        else
        {
            font->text_left_right->setText("NEW  NODE");
        }

        auto font2 = base->getSelfDraw()->getDraw(2);
        font2->text_left_right->setStart({ 10, 28 });
        font2->text_left_right->setText("DELE  NODE");

        auto font3 = base->getSelfDraw()->getDraw(3);
        font3->text_left_right->setStart({ 10, 43 });
        font3->text_left_right->setText("SAVE  ALL");

        BaseRect rect1;
        rect1.left = world_pt.x;
        rect1.top = world_pt.y;
        rect1.right = rect1.left + 90;
        rect1.bottom = rect1.top + 15;
        rect_new = rect1;
        BaseRect rect2;
        rect2.left = rect1.left;
        rect2.top = rect1.bottom;
        rect2.right = rect1.right;
        rect2.bottom = rect2.top + 15;
        rect_dele = rect2;
        rect1.bottom += 15;
        BaseRect rect3;
        rect3.left = rect1.left;
        rect3.top = rect2.bottom;
        rect3.right = rect1.right;
        rect3.bottom = rect3.top + 15;
        rect_save = rect3;
        rect1.bottom += 15;
        NvpLayout::setBaseRect(base, rect1);
    }

    BaseRect rect_new;
    BaseRect rect_dele;
    BaseRect rect_save;
};

////////////////////////////////////////////////////////////////////////////////

void nvp_app_init()
{
    NvpLayout::subElemGen("top_node_view", new TopNodeView, NvpLayout::getTopLayout());
    NvpLayout::subElemGen("RightMouseMenu", new RightMouseMenu, NvpLayout::getTopLayout());
    initAllNode();
}
