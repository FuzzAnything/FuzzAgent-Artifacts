// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
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
#include <string>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // Create XML document and nodes/attributes for testing
    pugi::xml_document doc;
    
    // Create a node and attribute to work with
    pugi::xml_node node = doc.append_child("test_node");
    pugi::xml_attribute attr = node.append_attribute("test_attr");
    
    // Create empty nodes/attributes for testing failure cases
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_attr;
    
    // Prepare string data from fuzzer input
    std::string_view str_view(reinterpret_cast<const char*>(Data), Size);
    
    // Test pugi::xml_node::set_name
    // Test with valid node
    node.set_name(str_view);
    // Test with empty node (should fail)
    empty_node.set_name(str_view);
    
    // Test pugi::xml_attribute::set_value overloads
    // Test string_view overload with valid attribute
    attr.set_value(str_view);
    // Test string_view overload with empty attribute (should fail)
    empty_attr.set_value(str_view);
    
    // Test bool overload - use first byte to determine boolean value
    bool bool_val = Size > 0 ? (Data[0] & 1) : false;
    attr.set_value(bool_val);
    empty_attr.set_value(bool_val);
    
    // Test pugi::xml_node::set_value
    // Create a PCDATA node for valid set_value operation
    pugi::xml_node pcdata_node = node.append_child(pugi::node_pcdata);
    pcdata_node.set_value(str_view);
    
    // Test with element node (should fail for unsupported type)
    node.set_value(str_view);
    // Test with empty node (should fail)
    empty_node.set_value(str_view);
    
    // Test pugi::xml_attribute::set_name overloads
    // Test string_view overload
    attr.set_name(str_view);
    empty_attr.set_name(str_view);
    
    // Test const char_t* overload
    // Ensure null termination for const char* version
    std::string null_term_str(reinterpret_cast<const char*>(Data), Size);
    if (!null_term_str.empty()) {
        attr.set_name(null_term_str.c_str());
        empty_attr.set_name(null_term_str.c_str());
    }
    
    // Additional tests with different node types
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    comment_node.set_value(str_view);
    
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    pi_node.set_name(str_view);
    pi_node.set_value(str_view);
    
    pugi::xml_node doctype_node = doc.append_child(pugi::node_doctype);
    doctype_node.set_value(str_view);
    
    // Test with multiple attributes
    for (int i = 0; i < 3 && i * 10 < static_cast<int>(Size); ++i) {
        size_t start = i * 10;
        size_t length = std::min(static_cast<size_t>(10), Size - start);
        std::string_view partial_view(reinterpret_cast<const char*>(Data + start), length);
        
        pugi::xml_attribute new_attr = node.append_attribute("attr");
        new_attr.set_name(partial_view);
        new_attr.set_value(partial_view);
    }
    
    // Test edge cases with empty strings
    std::string_view empty_view("", 0);
    attr.set_name(empty_view);
    attr.set_value(empty_view);
    node.set_name(empty_view);
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}