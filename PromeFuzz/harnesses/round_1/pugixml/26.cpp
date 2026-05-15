// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for parsing
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (!result) {
        return 0;
    }

    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare name for child() function - ensure null termination
    std::string child_name;
    if (Size > 0) {
        size_t name_len = Size > 100 ? 100 : Size; // Limit name length
        child_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        if (child_name.back() != '\0') {
            child_name.push_back('\0');
        }
    } else {
        child_name = "default\0";
    }

    // Call functions in specified order
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name.c_str());
    
    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    
    // 4. pugi::xml_node::hash_value
    size_t hash1 = root.hash_value();
    
    // 5. pugi::xml_attribute::hash_value
    size_t hash2 = first_attr.hash_value();
    
    // 6. pugi::xml_node::hash_value
    size_t hash3 = child_node.hash_value();
    
    // 7. pugi::xml_node::hash_value
    size_t hash4 = first_child.hash_value();
    
    // 8. pugi::xml_node::hash_value
    size_t hash5 = root.hash_value(); // Call again on root
    
    // 9. pugi::xml_attribute::hash_value
    size_t hash6 = first_attr.hash_value(); // Call again on first_attr
    
    // 10. pugi::xml_node::hash_value
    size_t hash7 = child_node.hash_value(); // Call again on child_node
    
    // 11. pugi::xml_attribute::hash_value
    size_t hash8 = first_attr.hash_value(); // Call again on first_attr

    // Use the hash values to avoid optimization
    volatile size_t dummy = hash1 + hash2 + hash3 + hash4 + hash5 + hash6 + hash7 + hash8;
    (void)dummy;

    return 0;
}