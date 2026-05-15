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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment, pugi::encoding_auto);
    
    if (!result) {
        return 0;
    }

    // Get the document element (root node)
    pugi::xml_node root = doc.document_element();
    
    // Prepare a null-terminated string for child() function
    // Use a small portion of input as the child name
    size_t name_len = Size > 100 ? 100 : Size;
    char* child_name = new char[name_len + 1];
    memcpy(child_name, Data, name_len);
    child_name[name_len] = '\0';
    
    // Create string_view from the child name
    std::string_view child_name_sv(child_name, name_len);
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. pugi::xml_node::first_attribute (on child_node)
    pugi::xml_attribute first_attr = child_node.first_attribute();
    
    // 3. pugi::xml_node::first_child (on child_node)
    pugi::xml_node first_child = child_node.first_child();
    
    // 4. pugi::xml_node::hash_value (on child_node)
    size_t hash1 = child_node.hash_value();
    
    // 5. pugi::xml_attribute::hash_value (on first_attr)
    size_t hash2 = first_attr.hash_value();
    
    // 6. pugi::xml_node::hash_value (on first_child)
    size_t hash3 = first_child.hash_value();
    
    // 7. pugi::xml_node::hash_value (on child_node again)
    size_t hash4 = child_node.hash_value();
    
    // 8. pugi::xml_node::hash_value (on root)
    size_t hash5 = root.hash_value();
    
    // 9. pugi::xml_attribute::hash_value (on first_attr again)
    size_t hash6 = first_attr.hash_value();
    
    // 10. pugi::xml_node::hash_value (on first_child again)
    size_t hash7 = first_child.hash_value();
    
    // 11. pugi::xml_attribute::hash_value (on first_attr again)
    size_t hash8 = first_attr.hash_value();
    
    // Use the hash values to prevent optimization
    static volatile size_t dummy = 0;
    dummy = hash1 + hash2 + hash3 + hash4 + hash5 + hash6 + hash7 + hash8;
    
    delete[] child_name;
    return 0;
}