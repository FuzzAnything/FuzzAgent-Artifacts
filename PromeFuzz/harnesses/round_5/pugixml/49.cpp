// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Create a mutable buffer for in-place parsing
    char* buffer = new char[Size];
    if (!buffer) return 0;
    std::memcpy(buffer, Data, Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size);
    
    // Use document element as base node, fallback to document itself
    pugi::xml_node base_node = doc.document_element();
    if (!base_node) base_node = doc;
    
    // 1. First append_child
    pugi::xml_node child1 = base_node.append_child("child1");
    
    // 2. First append_copy - need an attribute to copy
    // Create a dummy attribute first
    pugi::xml_attribute dummy_attr = child1.append_attribute("dummy");
    dummy_attr.set_value("value");
    pugi::xml_attribute copied_attr1 = base_node.append_copy(dummy_attr);
    
    // 3. first_child
    pugi::xml_node first_child = base_node.first_child();
    
    // 4. first_attribute
    pugi::xml_attribute first_attr = base_node.first_attribute();
    
    // 5. Second append_child
    pugi::xml_node child2 = base_node.append_child("child2");
    
    // 6. Second append_copy - use first_attr if available
    if (first_attr) {
        pugi::xml_attribute copied_attr2 = base_node.append_copy(first_attr);
    } else {
        // Use dummy_attr if no attributes exist
        pugi::xml_attribute copied_attr2 = base_node.append_copy(dummy_attr);
    }
    
    // 7. Second first_attribute
    pugi::xml_attribute first_attr2 = base_node.first_attribute();
    
    // Cleanup
    delete[] buffer;
    return 0;
}