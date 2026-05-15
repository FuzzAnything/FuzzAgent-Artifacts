// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    // Create null-terminated string from fuzzer input for safer usage
    char* cstr = nullptr;
    if (Size > 0) {
        cstr = new char[Size + 1];
        std::memcpy(cstr, Data, Size);
        cstr[Size] = '\0';
    } else {
        cstr = new char[1];
        cstr[0] = '\0';
    }
    
    std::string_view sv(reinterpret_cast<const char*>(Data), Size);
    
    // Test with empty objects
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_attr;
    
    // Test empty node set_name
    bool result1 = empty_node.set_name(cstr, Size);
    bool result2 = empty_node.set_name(sv.data(), sv.size());
    
    // Test empty attribute functions
    const char* name1 = empty_attr.name();
    bool is_empty = empty_attr.empty();
    bool result3 = empty_attr.set_value(cstr, Size);
    bool result4 = empty_attr.set_name(cstr, Size);
    bool result5 = empty_attr.set_name(cstr);  // Second overload
    
    // Create document and non-empty objects for more thorough testing
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child();
    pugi::xml_attribute attr = node.append_attribute("initial_name");
    
    // Test non-empty node set_name
    bool result6 = node.set_name(cstr, Size);
    bool result7 = node.set_name(sv.data(), sv.size());
    
    // Test non-empty attribute functions
    const char* name2 = attr.name();
    bool is_empty2 = attr.empty();
    bool result8 = attr.set_value(cstr, Size);
    bool result9 = attr.set_name(cstr, Size);
    bool result10 = attr.set_name(cstr);  // Second overload
    
    // Test with various string lengths and positions
    if (Size > 0) {
        // Test with substring of the input
        size_t half_size = Size / 2;
        bool result11 = node.set_name(cstr, half_size);
        bool result12 = attr.set_value(cstr + half_size, Size - half_size);
        
        // Test with single character
        bool result13 = attr.set_name(cstr, 1);
        
        // Test with empty string
        bool result14 = attr.set_name("", 0);
        bool result15 = attr.set_value("", 0);
    }
    
    // Cleanup
    delete[] cstr;
    
    // Use volatile variables to prevent optimization
    volatile bool v1 = result1;
    volatile bool v2 = result2;
    volatile const char* v3 = name1;
    volatile bool v4 = is_empty;
    volatile bool v5 = result3;
    volatile bool v6 = result4;
    volatile bool v7 = result5;
    volatile bool v8 = result6;
    volatile bool v9 = result7;
    volatile const char* v10 = name2;
    volatile bool v11 = is_empty2;
    volatile bool v12 = result8;
    volatile bool v13 = result9;
    volatile bool v14 = result10;
    
    return 0;
}