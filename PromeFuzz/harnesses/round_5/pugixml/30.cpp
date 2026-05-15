// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static std::string_view extract_string(const uint8_t* data, size_t size, size_t& pos) {
    if (pos >= size) {
        return std::string_view();
    }
    const char* str_start = reinterpret_cast<const char*>(data + pos);
    size_t remaining = size - pos;
    size_t len = 0;
    while (len < remaining && str_start[len] != '\0') {
        ++len;
    }
    pos += len + 1;
    return std::string_view(str_start, len);
}

static long extract_long(const uint8_t* data, size_t size, size_t& pos) {
    if (pos + sizeof(long) > size) {
        return 0;
    }
    long value;
    std::memcpy(&value, data + pos, sizeof(long));
    pos += sizeof(long);
    return value;
}

static unsigned long extract_ulong(const uint8_t* data, size_t size, size_t& pos) {
    if (pos + sizeof(unsigned long) > size) {
        return 0;
    }
    unsigned long value;
    std::memcpy(&value, data + pos, sizeof(unsigned long));
    pos += sizeof(unsigned long);
    return value;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* Data, size_t Size) {
    size_t pos = 0;
    
    std::string_view name1 = extract_string(Data, Size, pos);
    std::string_view name2 = extract_string(Data, Size, pos);
    std::string_view name3 = extract_string(Data, Size, pos);
    unsigned long ul_val1 = extract_ulong(Data, Size, pos);
    long l_val1 = extract_long(Data, Size, pos);
    std::string_view name4 = extract_string(Data, Size, pos);
    std::string_view name5 = extract_string(Data, Size, pos);
    unsigned long ul_val2 = extract_ulong(Data, Size, pos);
    long l_val2 = extract_long(Data, Size, pos);
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    pugi::xml_node child1 = root.child(name1.data());
    
    pugi::xml_node child2 = root.append_child(name2.data());
    
    pugi::xml_node child3 = root.append_child(name3.data());
    
    pugi::xml_text text1 = child3.text();
    text1.set(ul_val1);
    text1.set(l_val1);
    
    pugi::xml_node child4 = root.append_child(name4.data());
    
    pugi::xml_node child5 = root.append_child(name5.data());
    
    pugi::xml_text text2 = child5.text();
    text2.set(ul_val2);
    text2.set(l_val2);
    
    return 0;
}