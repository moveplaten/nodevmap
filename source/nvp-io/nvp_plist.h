#pragma once

#include <cstring>
#include <cassert>
#include <string>

#include "../third-party/libplist/include/plist/plist.h"

//wrap some functions from libplist;
class NvpPlistPort
{
public:
    typedef plist_type PlistType;

    NvpPlistPort(const char* xml_in);

    NvpPlistPort(const NvpPlistPort& plist);
    NvpPlistPort& operator=(const NvpPlistPort& plist) = delete;

    struct XmlPtr
    {
        XmlPtr() : xml_str(nullptr), xml_len(0) {}
        XmlPtr(char* str, uint32_t len) : xml_str(str), xml_len(len) {}

        union
        {
            char* xml_str;
            uint8_t(*p_xml_str)[1000][20];
        };
        uint32_t xml_len;
    };

    XmlPtr writeToXml();

    PlistType getType() const;

    NvpPlistPort queryDictKey(const char* key_str) const;

    uint32_t getDictSize() const;

    void insertDictKey(NvpPlistPort& plist, const char* key_str);

    const char* getKeyString() const;

    const char* getValString() const;

    bool getValBool() const;

    double getValReal() const;

    uint64_t getValUint() const;

    struct ValData
    {
        ValData() : data_ptr(nullptr), data_len(0) {}
        ValData(const char* ptr, uint64_t len) : data_ptr(ptr), data_len(len) {}

        union
        {
            const char* data_ptr;
            uint8_t(*p_data_ptr)[1000][20];
        };
        uint64_t data_len;
    };

    ValData getValData() const;

    ////////////////////////////////////////////////////////////////////////////

    static NvpPlistPort newEmptyDict();

    static NvpPlistPort newString(const char* str);

    static NvpPlistPort newBool(bool bool_t);

    static NvpPlistPort newReal(double val);

    static NvpPlistPort newUint(uint64_t val);

    static NvpPlistPort newData(const ValData& data);

    ////////////////////////////////////////////////////////////////////////////

    ~NvpPlistPort();

private:
    NvpPlistPort(plist_t plist, bool by_query);

    void initVal();
    void switchType();

    union Value
    {
        Value() : val_data() {}

        const char* val_string;
        bool val_bool;
        double val_real;
        uint64_t val_uint;
        ValData val_data;
    };

    Value one_value;

    char* key_string;
    XmlPtr xml_out;

    bool gen_by_query;
    bool is_freed;

    PlistType one_type;
    plist_t one_plist;
};
