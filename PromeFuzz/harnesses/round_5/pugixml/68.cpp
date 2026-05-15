// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <cstdint>
#include <cstring>

static void test_xml_node_set_name(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a null-terminated string from fuzzer input
    std::string_view str_view(reinterpret_cast<const char*>(data), size);
    node.set_name(str_view.data(), str_view.size());
}

static void test_xml_attribute_set_value(pugi::xml_attribute& attr, const uint8_t* data, size_t size) {
    if (size < sizeof(long) + sizeof(unsigned long)) return;
    
    // Extract long and unsigned long values from fuzzer input
    long long_val;
    unsigned long ulong_val;
    
    std::memcpy(&long_val, data, sizeof(long));
    std::memcpy(&ulong_val, data + sizeof(long), sizeof(unsigned long));
    
    attr.set_value(long_val);
    attr.set_value(ulong_val);
}

static void test_xml_node_set_value(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a string view from fuzzer input
    std::string_view str_view(reinterpret_cast<const char*>(data), size);
    node.set_value(str_view.data(), str_view.size());
}

static void test_xml_attribute_set_name(pugi::xml_attribute& attr, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a null-terminated string from fuzzer input
    std::string str(reinterpret_cast<const char*>(data), size);
    
    // Test both overloads
    attr.set_name(str.c_str(), str.size());
    attr.set_name(str.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
    }
    
    // Create XML document
    pugi::xml_document doc;
    
    // Create various node types to test with
    pugi::xml_node elem_node = doc.append_child(pugi::node_element);
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    pugi::xml_node decl_node = doc.append_child(pugi::node_declaration);
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
    pugi::xml_node doctype_node = doc.append_child(pugi::node_doctype);
    
    // Create attributes
    pugi::xml_attribute attr1 = elem_node.append_attribute("attr1");
    pugi::xml_attribute attr2 = elem_node.append_attribute("attr2");
    
    // Test xml_node::set_name on different node types
    test_xml_node_set_name(elem_node, Data, Size);
    test_xml_node_set_name(pi_node, Data, Size);
    test_xml_node_set_name(decl_node, Data, Size);
    
    // Test xml_attribute::set_value with numeric values
    test_xml_attribute_set_value(attr1, Data, Size);
    test_xml_attribute_set_value(attr2, Data, Size);
    
    // Test xml_node::set_value on compatible node types
    test_xml_node_set_value(comment_node, Data, Size);
    test_xml_node_set_value(pi_node, Data, Size);
    test_xml_node_set_value(cdata_node, Data, Size);
    test_xml_node_set_value(doctype_node, Data, Size);
    
    // Test xml_attribute::set_name
    test_xml_attribute_set_name(attr1, Data, Size);
    test_xml_attribute_set_name(attr2, Data, Size);
    
    // Also test on empty attribute
    pugi::xml_attribute empty_attr;
    test_xml_attribute_set_name(empty_attr, Data, Size);
    
    // Test on empty node
    pugi::xml_node empty_node;
    test_xml_node_set_name(empty_node, Data, Size);
    test_xml_node_set_value(empty_node, Data, Size);
    
    return 0;
}