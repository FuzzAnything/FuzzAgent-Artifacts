// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse XML from the input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, we can still test with empty document
    if (!result) {
        // Create empty document for fuzzing
        doc.reset();
    }

    // Get the root node (or document node if no root element)
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc;
    }

    // 1. pugi::xml_node::child
    // Create a null-terminated string from input data for child name
    static char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        std::memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        child_name[0] = '\0';
    }
    
    pugi::xml_node child_node = root.child(child_name);

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();

    // 4. pugi::xml_node::internal_object (first call)
    pugi::xml_node_struct* node_struct1 = root.internal_object();

    // 5. pugi::xml_attribute::internal_object (first call)
    pugi::xml_attribute_struct* attr_struct1 = nullptr;
    if (first_attr) {
        attr_struct1 = first_attr.internal_object();
    }

    // 6. pugi::xml_node::internal_object (second call)
    pugi::xml_node_struct* node_struct2 = nullptr;
    if (child_node) {
        node_struct2 = child_node.internal_object();
    } else {
        node_struct2 = root.internal_object();
    }

    // 7. pugi::xml_node::internal_object (third call)
    pugi::xml_node_struct* node_struct3 = nullptr;
    if (first_child) {
        node_struct3 = first_child.internal_object();
    } else {
        node_struct3 = root.internal_object();
    }

    // 8. pugi::xml_node::internal_object (fourth call)
    pugi::xml_node_struct* node_struct4 = root.internal_object();

    // 9. pugi::xml_attribute::internal_object (second call)
    pugi::xml_attribute_struct* attr_struct2 = nullptr;
    if (first_attr) {
        attr_struct2 = first_attr.internal_object();
    }

    // 10. pugi::xml_node::internal_object (fifth call)
    pugi::xml_node_struct* node_struct5 = nullptr;
    if (child_node) {
        node_struct5 = child_node.internal_object();
    }

    // 11. pugi::xml_attribute::internal_object (third call)
    pugi::xml_attribute_struct* attr_struct3 = nullptr;
    if (first_attr) {
        attr_struct3 = first_attr.internal_object();
    }

    // Use the internal pointers to prevent optimization
    // Use separate volatile variables for different pointer types
    volatile pugi::xml_node_struct* dummy_node = node_struct1;
    volatile pugi::xml_attribute_struct* dummy_attr = attr_struct1;
    
    dummy_node = node_struct2;
    dummy_node = node_struct3;
    dummy_node = node_struct4;
    dummy_attr = attr_struct2;
    dummy_node = node_struct5;
    dummy_attr = attr_struct3;

    // Additional dummy assignments to ensure all variables are used
    (void)dummy_node;
    (void)dummy_attr;

    return 0;
}