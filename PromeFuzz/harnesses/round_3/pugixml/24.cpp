// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    if (!result) {
        return 0;
    }
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
    // Prepare a null-terminated string for child() call
    // Use a small buffer to avoid overflows
    char child_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        memcpy(child_name, Data, name_len);
        child_name[name_len] = '\0';
    } else {
        child_name[0] = '\0';
    }
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = child_node.first_attribute();
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = child_node.first_child();
    
    // 4. pugi::xml_node::hash_value
    size_t hash1 = child_node.hash_value();
    
    // 5. pugi::xml_attribute::hash_value
    size_t hash2 = first_attr.hash_value();
    
    // 6. pugi::xml_node::hash_value
    size_t hash3 = child_node.hash_value();
    
    // 7. pugi::xml_node::hash_value
    size_t hash4 = first_child.hash_value();
    
    // 8. pugi::xml_node::hash_value
    size_t hash5 = root.hash_value();
    
    // 9. pugi::xml_attribute::hash_value
    size_t hash6 = first_attr.hash_value();
    
    // 10. pugi::xml_node::hash_value
    size_t hash7 = child_node.hash_value();
    
    // 11. pugi::xml_attribute::hash_value
    size_t hash8 = first_attr.hash_value();
    
    // Use the hash values to avoid optimization
    volatile size_t dummy = hash1 + hash2 + hash3 + hash4 + hash5 + hash6 + hash7 + hash8;
    (void)dummy;
    
    return 0;
}