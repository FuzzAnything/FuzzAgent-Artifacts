// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    // Create dummy objects to test the API functions
    pugi::xml_node node;
    pugi::xml_attribute attr;
    
    // Prepare string data from fuzzer input
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }
    
    // Create string_view from fuzzer data (safe even if not null-terminated)
    std::string_view str_view(reinterpret_cast<const char*>(Data), Size);
    
    // Test pugi::xml_node::set_name
    node.set_name(str_view);
    
    // Test pugi::xml_attribute::set_value (string_view_t overload)
    attr.set_value(str_view);
    
    // Test pugi::xml_attribute::set_value (bool overload)
    // Use first byte to determine boolean value
    bool bool_val = Data[0] & 1;
    attr.set_value(bool_val);
    
    // Test pugi::xml_node::set_value
    node.set_value(str_view);
    
    // Test pugi::xml_attribute::set_name (string_view_t overload)
    attr.set_name(str_view);
    
    // Test pugi::xml_attribute::set_name (const char_t* overload)
    // Create a null-terminated string for this overload
    // Use a small buffer to avoid large allocations
    const size_t buf_size = std::min(Size, static_cast<size_t>(256));
    char* null_term_str = new char[buf_size + 1];
    if (buf_size > 0) {
        memcpy(null_term_str, Data, buf_size);
    }
    null_term_str[buf_size] = '\0';
    
    attr.set_name(null_term_str);
    
    delete[] null_term_str;
    
    return 0;
}