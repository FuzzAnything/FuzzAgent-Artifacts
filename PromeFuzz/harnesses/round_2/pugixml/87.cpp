// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
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
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document to work with
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get root node and first attribute for testing
    pugi::xml_node root = doc.root();
    pugi::xml_attribute attr = root.first_attribute();
    
    // Also create empty/default objects
    pugi::xml_attribute empty_attr;
    pugi::xml_node empty_node;
    
    // Test 1: xml_attribute::empty()
    bool empty1 = empty_attr.empty();
    bool empty2 = attr.empty();
    
    // Prepare string data for set_name/set_value functions
    // Use a safe portion of the input data
    size_t str_size = Size > 100 ? 100 : Size;
    std::vector<char> safe_buffer(str_size + 1);
    if (Size > 0) {
        size_t copy_size = Size > 100 ? 100 : Size;
        memcpy(safe_buffer.data(), Data, copy_size);
        safe_buffer[copy_size] = '\0';
    } else {
        safe_buffer[0] = '\0';
    }
    
    std::string_view str_view(reinterpret_cast<const char*>(Data), 
                             Size > 100 ? 100 : Size);
    
    // Test 2: xml_attribute::set_name() - both overloads
    if (Size > 0) {
        // Overload 1: with size parameter
        attr.set_name(safe_buffer.data(), str_size);
        empty_attr.set_name(safe_buffer.data(), str_size);
        
        // Overload 2: without size parameter (null-terminated)
        attr.set_name(safe_buffer.data());
        empty_attr.set_name(safe_buffer.data());
    } else {
        // Test with empty string
        attr.set_name("", 0);
        attr.set_name("");
    }
    
    // Test 3: xml_node::set_name()
    if (Size > 0) {
        root.set_name(safe_buffer.data(), str_size);
        empty_node.set_name(safe_buffer.data(), str_size);
    } else {
        root.set_name("", 0);
    }
    
    // Test 4: xml_attribute::set_value()
    if (Size > 0) {
        attr.set_value(safe_buffer.data(), str_size);
        empty_attr.set_value(safe_buffer.data(), str_size);
        
        // Also test with numeric values
        if (Size > sizeof(int)) {
            int int_value;
            memcpy(&int_value, Data, sizeof(int));
            attr.set_value(int_value);
        }
        
        // Test with boolean
        attr.set_value(true);
        attr.set_value(false);
        
        // Test with floating point
        if (Size > sizeof(float)) {
            float float_value;
            memcpy(&float_value, Data, sizeof(float));
            attr.set_value(float_value);
        }
        
        // Test with double
        if (Size > sizeof(double)) {
            double double_value;
            memcpy(&double_value, Data, sizeof(double));
            attr.set_value(double_value);
        }
    } else {
        attr.set_value("", 0);
    }
    
    // Test 5: xml_attribute::name()
    const char* name1 = attr.name();
    const char* name2 = empty_attr.name();
    
    // Additional testing: create new attributes and test them
    pugi::xml_attribute new_attr = root.append_attribute("testattr");
    if (new_attr) {
        bool new_empty = new_attr.empty();
        new_attr.set_name("newname");
        new_attr.set_value("newvalue");
        const char* new_name = new_attr.name();
    }
    
    // Test with different node types if available
    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    if (decl) {
        decl.set_name("xml", 3);
    }
    
    pugi::xml_node pi = doc.append_child(pugi::node_pi);
    if (pi) {
        pi.set_name("target", 6);
    }
    
    return 0;
}