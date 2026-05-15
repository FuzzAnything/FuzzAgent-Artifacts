// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static std::string get_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    if (size == 0) return "";
    
    size_t len = std::min(size, max_len);
    const char* char_data = reinterpret_cast<const char*>(data);
    
    // Ensure null termination
    std::string result(char_data, len);
    if (len > 0 && result[len-1] != '\0') {
        result += '\0';
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, create a minimal valid XML document to work with
    if (!result) {
        static const char* dummy_xml = "<root attr=\"value\"><child>text</child></root>";
        result = doc.load_string(dummy_xml);
        if (!result) {
            return 0;
        }
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare a name for child lookup - ensure null termination
    std::string child_name = get_null_terminated_string(Data, Size);
    
    // Call functions in specified order
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name.c_str());
    
    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    
    // 4. pugi::xml_node::hash_value (on first_child)
    size_t hash1 = first_child.hash_value();
    
    // 5. pugi::xml_attribute::hash_value (on first_attr)
    size_t hash2 = first_attr.hash_value();
    
    // 6. pugi::xml_node::hash_value (on child_node)
    size_t hash3 = child_node.hash_value();
    
    // 7. pugi::xml_node::hash_value (on root)
    size_t hash4 = root.hash_value();
    
    // 8. pugi::xml_node::hash_value (on first_child again)
    size_t hash5 = first_child.hash_value();
    
    // 9. pugi::xml_attribute::hash_value (on first_attr again)
    size_t hash6 = first_attr.hash_value();
    
    // 10. pugi::xml_node::hash_value (on child_node again)
    size_t hash7 = child_node.hash_value();
    
    // 11. pugi::xml_attribute::hash_value (on first_attr again)
    size_t hash8 = first_attr.hash_value();
    
    // Use the hash values to avoid optimization
    volatile size_t dummy = hash1 + hash2 + hash3 + hash4 + hash5 + hash6 + hash7 + hash8;
    (void)dummy;
    
    return 0;
}