// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input data to a string for parsing
    // Ensure null termination for safety, though parse_string handles it
    std::string xml_data(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_document doc;
    
    // Parse the XML document
    pugi::xml_parse_result result = doc.load_string(xml_data.c_str());
    
    // If parsing fails, we can still test with an empty document structure
    // but valid traversal on null nodes is also a good test case.
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    // 1. pugi::xml_node::child
    // We try to find a child. The name is derived from the input data if possible,
    // or a default string.
    const char* child_name = nullptr;
    if (Size >= 1) {
        // Use first byte as a simple name indicator or just a static name
        // To ensure valid string, we use a static buffer or just a literal.
        // Let's use a literal to avoid null-byte issues in the name string itself.
        child_name = "child";
    }
    
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. pugi::xml_node::first_attribute
    // Call on the child_node (or root if child is empty)
    pugi::xml_attribute attr = child_node.first_attribute();
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child_node = child_node.first_child();
    
    // 4. pugi::xml_node::hash_value
    size_t hash1 = child_node.hash_value();
    
    // 5. pugi::xml_attribute::hash_value
    size_t attr_hash1 = attr.hash_value();
    
    // 6. pugi::xml_node::hash_value
    size_t hash2 = first_child_node.hash_value();
    
    // 7. pugi::xml_node::hash_value
    size_t hash3 = root.hash_value();
    
    // 8. pugi::xml_node::hash_value
    size_t hash4 = child_node.hash_value();
    
    // 9. pugi::xml_attribute::hash_value
    size_t attr_hash2 = attr.hash_value();
    
    // 10. pugi::xml_node::hash_value
    size_t hash5 = first_child_node.hash_value();
    
    // 11. pugi::xml_attribute::hash_value
    size_t attr_hash3 = attr.hash_value();
    
    // Suppress unused variable warnings
    (void)result;
    (void)hash1;
    (void)attr_hash1;
    (void)hash2;
    (void)hash3;
    (void)hash4;
    (void)attr_hash2;
    (void)hash5;
    (void)attr_hash3;
    
    return 0;
}