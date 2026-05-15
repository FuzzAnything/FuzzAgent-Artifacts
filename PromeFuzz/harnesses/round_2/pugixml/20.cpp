// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // If parsing fails, we can still test with empty document
    if (!result) {
        // Create empty document for testing
        pugi::xml_document empty_doc;
        doc = std::move(empty_doc);
    }
    
    // Get the document node (root)
    pugi::xml_node root = doc;
    
    // Prepare a name for child() function - ensure null termination
    std::string child_name;
    if (Size > 0) {
        // Use first part of input as child name, but ensure it's null-terminated
        size_t name_len = Size > 100 ? 100 : Size; // Limit name length
        child_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        if (!child_name.empty()) {
            child_name[name_len - 1] = '\0';
        }
    } else {
        child_name = "test";
    }
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name.c_str());
    
    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = child_node.first_attribute();
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();
    
    // 4. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct1 = child_node.internal_object();
    
    // 5. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct1 = first_attr.internal_object();
    
    // 6. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct2 = first_child.internal_object();
    
    // 7. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct3 = root.internal_object();
    
    // 8. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct4 = doc.internal_object();
    
    // 9. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct2 = first_attr.internal_object();
    
    // 10. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct5 = child_node.internal_object();
    
    // 11. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct3 = first_attr.internal_object();
    
    // Use the pointers in some way to avoid optimization
    volatile uintptr_t dummy = 0;
    if (node_struct1) dummy += reinterpret_cast<uintptr_t>(node_struct1);
    if (attr_struct1) dummy += reinterpret_cast<uintptr_t>(attr_struct1);
    if (node_struct2) dummy += reinterpret_cast<uintptr_t>(node_struct2);
    if (node_struct3) dummy += reinterpret_cast<uintptr_t>(node_struct3);
    if (node_struct4) dummy += reinterpret_cast<uintptr_t>(node_struct4);
    if (attr_struct2) dummy += reinterpret_cast<uintptr_t>(attr_struct2);
    if (node_struct5) dummy += reinterpret_cast<uintptr_t>(node_struct5);
    if (attr_struct3) dummy += reinterpret_cast<uintptr_t>(attr_struct3);
    
    return 0;
}