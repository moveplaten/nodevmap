#include "nvp_plist.h"

NvpPlistPort::NvpPlistPort(const char* xml_in) :
    one_plist(nullptr), key_string(nullptr), one_type(PLIST_NONE),
    gen_by_query(false), is_freed(false)
{
    if (!xml_in) return;

    auto xml_len = strlen(xml_in);
    plist_from_xml(xml_in, xml_len, &one_plist);

    if (one_plist) initVal();
}

NvpPlistPort::NvpPlistPort(const NvpPlistPort& plist) :
    one_plist(nullptr), key_string(nullptr), one_type(PLIST_NONE),
    gen_by_query(false), is_freed(false)
{
    auto copy = plist_copy(plist.one_plist);

    if (copy)
    {
        this->one_plist = copy;
        initVal();
    }
}

NvpPlistPort& NvpPlistPort::operator=(const NvpPlistPort& plist)
{
    if (this == &plist)
    {
        return *this;
    }

    Delete();

    memcpy(this, &plist, sizeof(NvpPlistPort));
    this->is_freed = true;
    this->xml_out = XmlPtr();
    char* temp = nullptr;
    plist_dict_get_item_key(one_plist, &temp);
    key_string = temp;

    return *this;
}

NvpPlistPort::NvpPlistPort(plist_t plist, bool by_query, bool freed) :
    one_plist(plist), key_string(nullptr), one_type(PLIST_NONE),
    gen_by_query(by_query), is_freed(freed)
{
    if (one_plist) initVal();
}

void NvpPlistPort::initVal()
{
    one_type = plist_get_node_type(one_plist);

    char* temp = nullptr;
    plist_dict_get_item_key(one_plist, &temp);

    key_string = temp;

    switchType();
}

void NvpPlistPort::switchType()
{
    switch (one_type)
    {
    case PLIST_BOOLEAN:
    {
        uint8_t temp = 0;
        plist_get_bool_val(one_plist, &temp);
        one_value.val_bool = temp;
    }
    break;

    case PLIST_UINT:
    {
        uint64_t value = 0;
        plist_get_uint_val(one_plist, &value);
        one_value.val_uint = value;
    }
    break;

    case PLIST_REAL:
    {
        double real = 0;
        plist_get_real_val(one_plist, &real);
        one_value.val_real = real;
    }
    break;

    case PLIST_STRING:
    {
        uint64_t len = 0;
        auto temp = plist_get_string_ptr(one_plist, &len);
        one_value.val_string = temp;
    }
    break;

    case PLIST_ARRAY:
    {

    }
    break;

    case PLIST_DICT:
    {

    }
    break;

    case PLIST_DATE:
    {

    }
    break;

    case PLIST_DATA:
    {
        ValData data;
        data.data_ptr = plist_get_data_ptr(one_plist, &data.data_len);
        one_value.val_data = data;
    }
    break;

    case PLIST_KEY:
    {

    }
    break;

    case PLIST_UID:
    {

    }
    break;

    case PLIST_NONE:
    {

    }
    break;

    default:
        break;
    }
}

NvpPlistPort::XmlPtr NvpPlistPort::writeToXml()
{
    if (xml_out.xml_str)
    {
        plist_to_xml_free(xml_out.xml_str); //free previous xml;
        xml_out.xml_str = nullptr;
        xml_out.xml_len = 0;
    }

    plist_to_xml(one_plist, &xml_out.xml_str, &xml_out.xml_len);
    return xml_out;
}

NvpPlistPort::PlistType NvpPlistPort::getType() const
{
    return one_type;
}

NvpPlistPort NvpPlistPort::getParent() const
{
    auto parent = plist_get_parent(one_plist);
    return NvpPlistPort(parent, false, true);
}

NvpPlistPort NvpPlistPort::queryDictKey(const char* key_str) const
{
    assert(one_type == PLIST_DICT);
    auto item = plist_dict_get_item(one_plist, key_str);
    return NvpPlistPort(item, true, true);
}

NvpPlistPort NvpPlistPort::queryArray(uint32_t order) const
{
    assert(one_type == PLIST_ARRAY);
    auto item = plist_array_get_item(one_plist, order);
    return NvpPlistPort(item, true, true);
}

uint32_t NvpPlistPort::getDictSize() const
{
    assert(one_type == PLIST_DICT);
    return plist_dict_get_size(one_plist);
}

uint32_t NvpPlistPort::getArraySize() const
{
    assert(one_type == PLIST_ARRAY);
    return plist_array_get_size(one_plist);
}

uint32_t NvpPlistPort::getArrayIndex() const
{
    return plist_array_get_item_index(one_plist);
}

void NvpPlistPort::insertDictKey(NvpPlistPort& plist, const char* key_str)
{
    assert(plist.is_freed == false && one_type == PLIST_DICT);
    plist_dict_set_item(one_plist, key_str, plist.one_plist);
    plist.is_freed = true;
}

void NvpPlistPort::pushArrayItem(NvpPlistPort& plist)
{
    assert(plist.is_freed == false && one_type == PLIST_ARRAY);
    plist_array_append_item(one_plist, plist.one_plist);
    plist.is_freed = true;
}

const char* NvpPlistPort::getKeyString() const
{
    return key_string;
}

const char* NvpPlistPort::getValString() const
{
    assert(one_type == PLIST_STRING);
    return one_value.val_string;
}

bool NvpPlistPort::getValBool() const
{
    assert(one_type == PLIST_BOOLEAN);
    return one_value.val_bool;
}

double NvpPlistPort::getValReal() const
{
    assert(one_type == PLIST_REAL);
    return one_value.val_real;
}

uint64_t NvpPlistPort::getValUint() const
{
    assert(one_type == PLIST_UINT);
    return one_value.val_uint;
}

NvpPlistPort::ValData NvpPlistPort::getValData() const
{
    assert(one_type == PLIST_DATA);
    return one_value.val_data;
}

NvpPlistPort NvpPlistPort::newEmptyDict()
{
    auto empty = plist_new_dict();
    return NvpPlistPort(empty, false);
}

NvpPlistPort NvpPlistPort::newEmptyArray()
{
    auto empty = plist_new_array();
    return NvpPlistPort(empty, false);
}

NvpPlistPort NvpPlistPort::newString(const char* str)
{
    auto item_str = plist_new_string(str);
    return NvpPlistPort(item_str, false);
}

NvpPlistPort NvpPlistPort::newBool(bool bool_t)
{
    auto item_bool = plist_new_bool(bool_t);
    return NvpPlistPort(item_bool, false);
}

NvpPlistPort NvpPlistPort::newReal(double val)
{
    auto item_real = plist_new_real(val);
    return NvpPlistPort(item_real, false);
}

NvpPlistPort NvpPlistPort::newUint(uint64_t val)
{
    auto item_uint = plist_new_uint(val);
    return NvpPlistPort(item_uint, false);
}

NvpPlistPort NvpPlistPort::newData(const ValData& data)
{
    auto item_data = plist_new_data(data.data_ptr, data.data_len);
    return NvpPlistPort(item_data, false);
}

NvpPlistPort NvpPlistPort::getSubFirst(const NvpPlistPort& array)
{
    if (array.getType() == PLIST_ARRAY)
    {
        return array.queryArray(0);
    }
    else
    {
        return NvpPlistPort(nullptr);
    }
}

NvpPlistPort NvpPlistPort::getNext(const NvpPlistPort& array)
{
    auto index = array.getArrayIndex();
    auto parent = array.getParent();
    if (parent.getType() == PLIST_ARRAY)
    {
        return parent.queryArray(++index);
    }
    else
    {
        return NvpPlistPort(nullptr);
    }
}

NvpPlistPort NvpPlistPort::getSubLast(const NvpPlistPort& array)
{
    if (array.getType() == PLIST_ARRAY)
    {
        auto size = array.getArraySize();
        return array.queryArray(--size);
    }
    else
    {
        return NvpPlistPort(nullptr);
    }
}

NvpPlistPort NvpPlistPort::getNextReverse(const NvpPlistPort& array)
{
    auto index = array.getArrayIndex();
    auto parent = array.getParent();
    if (parent.getType() == PLIST_ARRAY)
    {
        return parent.queryArray(--index);
    }
    else
    {
        return NvpPlistPort(nullptr);
    }
}

void NvpPlistPort::Delete()
{
    if (!gen_by_query && !is_freed)
    {
        plist_free(one_plist);
    }

    if (xml_out.xml_str)
    {
        plist_to_xml_free(xml_out.xml_str);
    }

    if (key_string)
    {
        free(key_string);
    }
}

NvpPlistPort::~NvpPlistPort()
{
    Delete();
}
