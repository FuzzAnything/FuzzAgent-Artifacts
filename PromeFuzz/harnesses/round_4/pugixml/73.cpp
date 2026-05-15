// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static void test_xml_attribute_set_name(pugi::xml_attribute& attr, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Test string_view_t overload
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    attr.set_name(sv);
    
    // Test const char_t* overload - ensure null termination
    if (size > 0) {
        std::string str(sv);
        attr.set_name(str.c_str());
    }
}

static void test_xml_node_set_name(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    node.set_name(sv);
}

static void test_xml_node_set_value(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    node.set_value(sv);
}

static void test_xml_attribute_set_value(pugi::xml_attribute& attr, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Test string_view_t overload
    std::string_view sv(reinterpret_cast<const char*>(data), size);
    attr.set_value(sv);
    
    // Test bool overload - use first byte to determine boolean value
    bool bool_val = (data[0] & 1) != 0;
    attr.set_value(bool_val);
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Create a non-empty node (element type)
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Create a non-empty attribute
    pugi::xml_attribute attr = node.append_attribute("initial_attr");
    
    // Test all target functions
    test_xml_attribute_set_name(attr, Data, Size);
    test_xml_node_set_name(node, Data, Size);
    test_xml_node_set_value(node, Data, Size);
    test_xml_attribute_set_value(attr, Data, Size);
    
    // Test with different node types that support set_value
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    test_xml_node_set_value(comment_node, Data, Size);
    
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    test_xml_node_set_value(pi_node, Data, Size);
    
    pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
    test_xml_node_set_value(cdata_node, Data, Size);
    
    pugi::xml_node doctype_node = doc.append_child(pugi::node_doctype);
    test_xml_node_set_value(doctype_node, Data, Size);
    
    // Test with another attribute
    pugi::xml_attribute attr2 = node.append_attribute("another_attr");
    test_xml_attribute_set_name(attr2, Data, Size);
    test_xml_attribute_set_value(attr2, Data, Size);
    
    return 0;
}