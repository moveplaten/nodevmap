/*
 * This file is part of nodevmap open source project.
 *
 * For conditions of distribution and use, see copyright notice in LICENSE file.
 *
 * For more detailed information : https://github.com/moveplaten
 */

#include "test_plist_04.h"
#include <math.h>

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

        static int random = -1; ++random;
        random %= 4;
        switch (random)
        {
        case 0:
        {
            NvpDrawCache text(NvpDrawData::Text_Left_Right);
            text.setColor(col);
            text.text_left_right->setText("github");
            text.text_left_right->setStart({ 0, 0 });
            text.text_left_right->setFontSize(15);
            base->getSelfDraw()->pushDraw(text);
            NvpDrawCache line(NvpDrawData::One_Line);
            line.setColor({ 220, 220, 220 });
            line.one_line->setPoint1({ 0, 0 });
            line.one_line->setPoint2({ rect.right - rect.left, rect.bottom - rect.top });
            base->getSelfDraw()->pushDraw(line);
        }
        break;
        case 1:
        {
            NvpDrawCache text(NvpDrawData::Text_Left_Right);
            text.setColor(col);
            text.text_left_right->setText("com");
            text.text_left_right->setStart({ 0, 0 });
            text.text_left_right->setFontSize(15);
            base->getSelfDraw()->pushDraw(text);
        }
        break;
        case 2:
        {
            NvpDrawCache text(NvpDrawData::Text_Left_Right);
            text.setColor(col);
            text.text_left_right->setText("moveplaten");
            text.text_left_right->setStart({ 0, 0 });
            text.text_left_right->setFontSize(15);
            base->getSelfDraw()->pushDraw(text);
            NvpDrawCache line(NvpDrawData::One_Line);
            line.setColor({ 220, 220, 220 });
            line.one_line->setPoint1({ 0, 0 });
            line.one_line->setPoint2({ rect.right - rect.left, rect.bottom - rect.top });
            base->getSelfDraw()->pushDraw(line);
        }
        break;
        case 3:
        {
            NvpDrawCache text(NvpDrawData::Text_Left_Right);
            text.setColor(col);
            text.text_left_right->setText("nodevmap");
            text.text_left_right->setStart({ 0, 0 });
            text.text_left_right->setFontSize(15);
            base->getSelfDraw()->pushDraw(text);
        }
        break;
        default:
            break;
        }
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
static const char* io_png_name = "test_01.png";

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

class PlistSeqCache : public NvpDrawSeq
{
    NvpEvent* createNewEvent()
    {
        return new RandomColorNode;
    }
    NvpPlistSeq* createNewSeq()
    {
        return new PlistSeqCache;
    }
};

static void saveAllNode()
{
    auto node1 = NvpLayout::getSubFirst(NvpLayout::getTopNodeView());
    auto file = NvpUtil::fileInExePath(io_file_name);
    NvpPlistIO::outputAll(node1, new PlistSeqCache, file);

    NvpImageIO::outputAll(io_png_name);
}

static void initAllNode()
{
    auto file = NvpUtil::fileInExePath(io_file_name);
    NvpPlistIO::inputAll(new PlistSeqCache, file, plist_array_04);
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

namespace test
{
    struct TestBase
    {
        TestBase(void* ptr, size_t size)
        {
            memset(ptr, 0, size);
        }
    };

    struct TestNoPtr : TestBase
    {
        TestNoPtr(bool empty = false) : TestBase(this, sizeof(TestNoPtr))
        {
            if (empty)
            {
                memset(this, 0, sizeof(TestNoPtr));
            }
        }
        void coding(std::string* dst, const char* src)
        {
            NvpCoding::codingSeq(dst, src, &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8,
                &_9, &_10, &rect.left, &rect.top, &rect.right, &rect.bottom, reinterpret_cast<char*>(&opt));
        }
        float _1 = 1.11;
        double _2 = 2.222;
        uint32_t _3 = 33333;
        bool _4 = true;
        int64_t _5 = 5555555;
        int32_t _6 = -666666;
        char _7 = 'A';
        uint8_t _8 = 250;
        char _9 = 'B';
        float _10 = -10.10;
        BaseRect rect = { 12.2, -13.3, 14.4, -15.5 };
        enum Opt : uint8_t { Opt1, Opt2 };
        Opt opt = Opt2;
    };

    struct TestPtr : TestBase
    {
        TestPtr(bool empty = false) : TestBase(this, sizeof(TestPtr))
        {
            if (empty)
            {
                memset(this, 0, sizeof(TestPtr));
            }
        }
        void coding(std::string* dst, const char* src)
        {
            NvpCoding::codingSeq(dst, src, &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8,
                &_9, &_10, &rect.left, &rect.top, &rect.right, &rect.bottom, reinterpret_cast<char*>(&opt));
        }
        float _1 = 1.11;
        double _2 = 2.222;
        uint32_t _3 = 33333;
        bool _4 = true;
        std::string _5 = " Test Test Test Test Test Test Test Test 5555 ";
        int32_t _6 = -666666;
        char _7 = 'A';
        uint8_t _8 = 250;
        char _9 = 'B';
        std::string _10 = "Test Test Test Test Test Test Test Test 10101010101010";
        BaseRect rect = { 12.22, -13.33, 14.44, -15.55 };
        enum Opt : uint8_t { Opt1, Opt2 };
        Opt opt = Opt2;
    };

    static void codingTest()
    {
        {
            TestNoPtr a;
            std::string dst;
            a.coding(&dst, nullptr);
            TestNoPtr b(true);
            b.coding(nullptr, dst.c_str());
            assert(memcmp(&a, &b, sizeof(TestNoPtr)) == 0);
        }

        {
            TestPtr a;
            std::string dst;
            a.coding(&dst, nullptr);
            TestPtr b(true);
            b.coding(nullptr, dst.c_str());
            assert(memcmp(a._5.c_str(), b._5.c_str(), a._5.size()) == 0);
            assert(memcmp(a._10.c_str(), b._10.c_str(), a._10.size()) == 0);
        }
    }

    static void cacheTest()
    {
        NvpDrawCache cache_text(NvpDrawData::Text_Left_Right);
        cache_text.setColor({ 222, 200, 200, 100 });
        cache_text.setStyle(NvpStyle::Frame);
        cache_text.text_left_right->setText(" Test Test Test Test Test Test Test Test 10101010101010");
        cache_text.text_left_right->setStart({ 22.222, -11.111 });
        cache_text.text_left_right->setFontSize(13.333);
        std::string cache_text_str;
        cache_text.encodeSeq(&cache_text_str);

        auto cache_text_new = NvpDrawCache::decodeSeq(cache_text_str.c_str());
        auto text_size = cache_text.text_left_right->getText().size();
        auto text_ptr = cache_text.text_left_right->getText().c_str();
        auto text_ptr2 = cache_text_new.text_left_right->getText().c_str();
        assert(memcmp(text_ptr, text_ptr2, text_size) == 0);

        NvpDrawData::Rect_Same_Elem_ r;
        std::string dst;
        r.drawCoding(&dst, nullptr);
    }
}

void nvp_app_init()
{
    test::codingTest();
    test::cacheTest();

    NvpLayout::subElemGen("top_node_view", new TopNodeView, NvpLayout::getTopLayout());
    NvpLayout::subElemGen("RightMouseMenu", new RightMouseMenu, NvpLayout::getTopLayout());
    initAllNode();
}
