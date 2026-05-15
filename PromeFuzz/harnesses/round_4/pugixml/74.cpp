// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Create XML document and nodes for testing
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    pugi::xml_attribute attr = node.append_attribute("initial_attr");
    
    // Test with empty attribute
    pugi::xml_attribute empty_attr;
    
    // Prepare string data from fuzzer input
    if (Size == 0) {
        return 0;
    }
    
    // Create a null-terminated string from fuzzer data
    char* str_data = new char[Size + 1];
    memcpy(str_data, Data, Size);
    str_data[Size] = '\0';
    
    // Test xml_attribute::set_name overloads
    // Overload 1: with size parameter
    bool result1 = attr.set_name(str_data, Size);
    
    // Overload 2: without size parameter (requires null-terminated string)
    bool result2 = attr.set_name(str_data);
    
    // Test xml_attribute::empty
    bool empty1 = attr.empty();
    bool empty2 = empty_attr.empty();
    
    // Test xml_attribute::name
    const pugi::char_t* name1 = attr.name();
    const pugi::char_t* name2 = empty_attr.name();
    
    // Test xml_attribute::set_value
    bool value_result = attr.set_value(str_data, Size);
    
    // Test xml_node::set_name
    bool node_result = node.set_name(str_data, Size);
    
    // Clean up
    delete[] str_data;
    
    // Use the results to avoid optimization
    (void)result1;
    (void)result2;
    (void)empty1;
    (void)empty2;
    (void)name1;
    (void)name2;
    (void)value_result;
    (void)node_result;
    
    return 0;
}