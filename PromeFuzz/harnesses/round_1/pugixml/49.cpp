// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Initialize pugixml document and node structure
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Create attributes to work with
    pugi::xml_attribute attr1 = node.append_attribute("attr1");
    pugi::xml_attribute attr2 = node.append_attribute("attr2");
    pugi::xml_attribute empty_attr;  // Empty attribute for null tests
    
    // Use fuzzer input to create strings for testing
    // Split input into parts for name and value testing
    size_t half_size = Size / 2;
    const char* data_str = reinterpret_cast<const char*>(Data);
    
    // Create string views from the input data
    std::string_view name_input(data_str, half_size);
    std::string_view value_input(data_str + half_size, Size - half_size);
    
    // Test 1: xml_attribute::value()
    {
        // Test on valid attribute
        const char* val1 = attr1.value();
        (void)val1;
        
        // Test on empty attribute
        const char* val2 = empty_attr.value();
        (void)val2;
    }
    
    // Test 2: xml_attribute::as_string()
    {
        // Test with default parameter
        const char* str1 = attr1.as_string();
        (void)str1;
        
        // Test with custom default value
        const char* str2 = attr2.as_string("default");
        (void)str2;
        
        // Test on empty attribute
        const char* str3 = empty_attr.as_string();
        (void)str3;
    }
    
    // Test 3: xml_attribute::name()
    {
        // Test on valid attribute
        const char* name1 = attr1.name();
        (void)name1;
        
        // Test on empty attribute
        const char* name2 = empty_attr.name();
        (void)name2;
    }
    
    // Test 4: xml_attribute::set_value()
    {
        // Test setting value with fuzzer input
        if (value_input.data() && value_input.size() > 0) {
            bool success1 = attr1.set_value(value_input.data());
            (void)success1;
        }
        
        // Test setting empty value
        bool success2 = attr2.set_value("");
        (void)success2;
        
        // Test on empty attribute (should fail)
        bool success3 = empty_attr.set_value("test");
        (void)success3;
    }
    
    // Test 5: xml_attribute::set_name()
    {
        // Test setting name with fuzzer input
        if (name_input.data() && name_input.size() > 0) {
            bool success1 = attr1.set_name(name_input.data());
            (void)success1;
        }
        
        // Test setting empty name
        bool success2 = attr2.set_name("");
        (void)success2;
        
        // Test on empty attribute (should fail)
        bool success3 = empty_attr.set_name("test");
        (void)success3;
    }
    
    // Test 6: xml_node::remove_attribute()
    {
        // Remove attribute by name
        bool removed1 = node.remove_attribute("attr1");
        (void)removed1;
        
        // Try to remove non-existent attribute
        bool removed2 = node.remove_attribute("non_existent");
        (void)removed2;
        
        // Remove with fuzzer input as name
        if (name_input.data() && name_input.size() > 0) {
            bool removed3 = node.remove_attribute(name_input.data());
            (void)removed3;
        }
    }
    
    // Clean up - document will be automatically destroyed
    
    return 0;
}