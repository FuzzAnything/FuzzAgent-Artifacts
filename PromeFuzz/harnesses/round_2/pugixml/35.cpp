// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
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

static void fuzz_xml_operations(const uint8_t* data, size_t size) {
    // Create a buffer with null terminator for safe string operations
    if (size == 0) return;
    
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    if (!result) return;
    
    // Get the root node
    pugi::xml_node root = doc.document_element();
    if (!root) return;
    
    // Create null-terminated strings for node/attribute names
    // Use first few bytes for names, ensuring null termination
    const size_t name_len = size > 100 ? 100 : size;
    std::vector<char> name_buf(name_len + 1);
    if (size > 0) {
        size_t copy_len = name_len < size ? name_len : size;
        std::memcpy(name_buf.data(), data, copy_len);
        name_buf[copy_len] = '\0';
    } else {
        name_buf[0] = '\0';
    }
    
    // 1. First xml_node::child call
    pugi::xml_node child1 = root.child(name_buf.data());
    
    // 2. First xml_node::attribute call
    pugi::xml_attribute attr1 = root.attribute(name_buf.data());
    
    // 3. Second xml_node::child call (on first child)
    pugi::xml_node child2 = child1.child(name_buf.data());
    
    // 4. Second xml_node::attribute call (on second child)
    pugi::xml_attribute attr2 = child2.attribute(name_buf.data());
    
    // 5. First xml_attribute::next_attribute call
    pugi::xml_attribute next_attr1 = attr2.next_attribute();
    
    // 6. Second xml_attribute::next_attribute call
    pugi::xml_attribute next_attr2 = next_attr1.next_attribute();
    
    // 7. First xml_attribute::previous_attribute call
    pugi::xml_attribute prev_attr1 = next_attr2.previous_attribute();
    
    // 8. Second xml_attribute::previous_attribute call
    pugi::xml_attribute prev_attr2 = prev_attr1.previous_attribute();
    
    // 9. Third xml_attribute::next_attribute call
    pugi::xml_attribute next_attr3 = prev_attr2.next_attribute();
    
    // 10. Third xml_attribute::previous_attribute call
    pugi::xml_attribute prev_attr3 = next_attr3.previous_attribute();
    
    // The objects are automatically cleaned up when they go out of scope
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* data, size_t size) {
    fuzz_xml_operations(data, size);
    return 0;
}