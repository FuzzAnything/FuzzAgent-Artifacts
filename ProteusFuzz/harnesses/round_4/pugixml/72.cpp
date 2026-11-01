// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <pugixml.hpp>

static void test_child_value(pugi::xml_node node, const uint8_t* data, size_t size) {
    // Test child_value() - get text of first child
    const pugi::char_t* val1 = node.child_value();
    if (val1) {
        (void)std::strlen(val1);
    }

    // Test child_value(name) - get text of specific child
    if (size > 0) {
        // Create a safe name from input data, ensuring null termination
        std::string name_str(reinterpret_cast<const char*>(data), size);
        // Remove nulls to ensure we have a valid string for the name
        name_str.erase(std::remove(name_str.begin(), name_str.end(), '\0'), name_str.end());
        if (!name_str.empty()) {
            const pugi::char_t* val2 = node.child_value(name_str.c_str());
            if (val2) {
                (void)std::strlen(val2);
            }
        }
    }
}

static void test_as_wide(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Ensure null termination for as_wide
    std::string str(reinterpret_cast<const char*>(data), size);
    str.push_back('\0');
    
    // Test as_wide conversion
    std::wstring wide_str = pugi::as_wide(str.c_str());
    (void)wide_str.length();
}

static void test_xml_text_get(pugi::xml_text text_node) {
    const pugi::char_t* val = text_node.get();
    if (val) {
        (void)std::strlen(val);
    }
}

static void test_node_value(pugi::xml_node node) {
    const pugi::char_t* val = node.value();
    if (val) {
        (void)std::strlen(val);
    }
}

static void test_node_name(pugi::xml_node node) {
    const pugi::char_t* name = node.name();
    if (name) {
        (void)std::strlen(name);
    }
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Parse XML from input data
    // We must ensure the input is null-terminated for load_string
    // Create a copy with null termination to avoid buffer overflow
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    input_str.push_back('\0');

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());
    
    // Even if parsing fails, we can still test with empty nodes
    pugi::xml_node root = doc.root();
    
    // Test all target functions
    test_child_value(root, Data, Size);
    test_as_wide(Data, Size);
    
    // Test xml_text::get with root's text node
    pugi::xml_text text_node = root.text();
    test_xml_text_get(text_node);
    
    // Test node value and name
    test_node_value(root);
    test_node_name(root);
    
    // Also test with child nodes if they exist
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        test_child_value(child, Data, Size);
        test_xml_text_get(child.text());
        test_node_value(child);
        test_node_name(child);
        
        // Test with attributes as well (attributes are not nodes, so we don't cast them)
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            // We can't convert attribute to node, so we just iterate to exercise the code path
            // Attributes don't have name() or value() in the same way nodes do for this specific API set
            // but we can check if the attribute exists
            (void)attr.name();
            (void)attr.value();
        }
    }
    
    // Test with empty nodes
    pugi::xml_node empty_node;
    test_child_value(empty_node, Data, Size);
    test_xml_text_get(pugi::xml_text());
    test_node_value(empty_node);
    test_node_name(empty_node);
    
    return 0;
}