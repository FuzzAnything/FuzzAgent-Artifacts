// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
// pugi::xml_node::hash_value at pugixml.cpp:6836:32 in pugixml.hpp
// pugi::xml_attribute::hash_value at pugixml.cpp:5379:37 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzing input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    if (!result) {
        // If parsing fails, try to create a minimal XML document for API testing
        doc.load_string("<root attr=\"value\"><child>text</child></root>");
    }
    
    // Get the root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // If no root, create one
        root = doc.append_child("root");
        root.append_attribute("attr") = "value";
        root.append_child("child").text() = "text";
    }
    
    // Prepare child name - ensure null termination
    std::string child_name;
    if (Size > 0) {
        // Use first 100 bytes or less as child name
        size_t name_len = std::min(Size, (size_t)100);
        child_name = std::string(reinterpret_cast<const char*>(Data), name_len);
    } else {
        child_name = "child";
    }
    
    // Call functions in specified order
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name.c_str());
    
    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    
    // 4. pugi::xml_node::hash_value (on child_node)
    size_t hash1 = child_node.hash_value();
    
    // 5. pugi::xml_attribute::hash_value (on first_attr)
    size_t hash2 = first_attr.hash_value();
    
    // 6. pugi::xml_node::hash_value (on first_child)
    size_t hash3 = first_child.hash_value();
    
    // 7. pugi::xml_node::hash_value (on child_node again)
    size_t hash4 = child_node.hash_value();
    
    // 8. pugi::xml_node::hash_value (on first_child again)
    size_t hash5 = first_child.hash_value();
    
    // 9. pugi::xml_attribute::hash_value (on first_attr again)
    size_t hash6 = first_attr.hash_value();
    
    // 10. pugi::xml_node::hash_value (on child_node again)
    size_t hash7 = child_node.hash_value();
    
    // 11. pugi::xml_attribute::hash_value (on first_attr again)
    size_t hash8 = first_attr.hash_value();
    
    // Use volatile to prevent optimization
    volatile size_t dummy = hash1 + hash2 + hash3 + hash4 + hash5 + hash6 + hash7 + hash8;
    (void)dummy;
    
    return 0;
}