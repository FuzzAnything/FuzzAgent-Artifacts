// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    // Split input into multiple parts for different operations
    if (Size < 4) return 0;
    
    size_t part1_size = Size / 4;
    size_t part2_size = Size / 4;
    size_t part3_size = Size / 4;
    size_t part4_size = Size - 3 * (Size / 4);
    
    const char* data1 = reinterpret_cast<const char*>(Data);
    const char* data2 = data1 + part1_size;
    const char* data3 = data2 + part2_size;
    const char* data4 = data3 + part3_size;
    
    // Create XML document and nodes for testing
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    pugi::xml_attribute attr = node.append_attribute("initial");
    
    // Test 1: xml_node::set_name
    {
        std::string_view name_str(data1, part1_size);
        bool result = node.set_name(name_str.data(), name_str.size());
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test 2: xml_attribute::name
    {
        const char* attr_name = attr.name();
        (void)attr_name; // Use to avoid unused variable warning
    }
    
    // Test 3: xml_attribute::empty
    {
        bool is_empty = attr.empty();
        (void)is_empty; // Use to avoid unused variable warning
        
        // Test with default-constructed attribute
        pugi::xml_attribute empty_attr;
        bool empty_result = empty_attr.empty();
        (void)empty_result;
    }
    
    // Test 4: xml_attribute::set_value
    {
        std::string_view value_str(data2, part2_size);
        bool result = attr.set_value(value_str.data(), value_str.size());
        (void)result;
    }
    
    // Test 5: xml_attribute::set_name - both overloads
    {
        std::string_view name_str1(data3, part3_size);
        bool result1 = attr.set_name(name_str1.data(), name_str1.size());
        (void)result1;
        
        // Create null-terminated string for second overload
        char* null_term_str = new char[part4_size + 1];
        if (null_term_str) {
            memcpy(null_term_str, data4, part4_size);
            null_term_str[part4_size] = '\0';
            
            bool result2 = attr.set_name(null_term_str);
            (void)result2;
            
            delete[] null_term_str;
        }
    }
    
    // Additional tests with different states
    {
        // Test with empty node
        pugi::xml_node empty_node;
        std::string_view empty_str("test", 4);
        bool empty_node_result = empty_node.set_name(empty_str.data(), empty_str.size());
        (void)empty_node_result;
        
        // Test with another attribute
        pugi::xml_attribute attr2 = node.append_attribute("second");
        std::string_view name_str(data1, std::min(part1_size, size_t(10)));
        bool attr2_result = attr2.set_name(name_str.data(), name_str.size());
        (void)attr2_result;
        
        // Test value setting on empty attribute
        pugi::xml_attribute empty_attr2;
        std::string_view value_str(data2, std::min(part2_size, size_t(10)));
        bool empty_attr_result = empty_attr2.set_value(value_str.data(), value_str.size());
        (void)empty_attr_result;
    }
    
    return 0;
}