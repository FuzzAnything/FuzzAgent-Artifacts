// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // Even if parsing fails, we can still test with empty document
        // Continue execution with empty document
    }
    
    // Get root node (document element)
    pugi::xml_node root = doc.document_element();
    
    // Prepare child name from fuzzer data (ensure null-terminated)
    char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        std::memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        child_name[0] = '\0';
    }
    
    // Call functions in specified order
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name);
    
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
    // Use a different node - try to get parent if available
    pugi::xml_node parent_node = child_node.parent();
    pugi::xml_node_struct* node_struct4 = parent_node.internal_object();
    
    // 9. pugi::xml_attribute::internal_object
    // Try to get next attribute if available
    pugi::xml_attribute next_attr = first_attr.next_attribute();
    pugi::xml_attribute_struct* attr_struct2 = next_attr.internal_object();
    
    // 10. pugi::xml_node::internal_object
    // Try to get next sibling if available
    pugi::xml_node next_sibling = first_child.next_sibling();
    pugi::xml_node_struct* node_struct5 = next_sibling.internal_object();
    
    // 11. pugi::xml_attribute::internal_object
    // Try to get previous attribute if available
    pugi::xml_attribute prev_attr = first_attr.previous_attribute();
    pugi::xml_attribute_struct* attr_struct3 = prev_attr.internal_object();
    
    // The internal_object() calls return pointers that we don't use directly
    // but the fuzzer exercises the code paths
    (void)node_struct1;
    (void)attr_struct1;
    (void)node_struct2;
    (void)node_struct3;
    (void)node_struct4;
    (void)attr_struct2;
    (void)node_struct5;
    (void)attr_struct3;
    
    return 0;
}