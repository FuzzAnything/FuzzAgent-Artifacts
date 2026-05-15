// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
    
    // Create XML document
    pugi::xml_document doc;
    
    // 1. load_buffer_inplace
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size);
    
    // Get root node (or document node if no root element)
    pugi::xml_node root = doc;
    if (doc.first_child()) {
        root = doc.first_child();
    }
    
    // 2. append_child
    pugi::xml_node child1 = root.append_child("child1");
    
    // 3. append_copy - need an attribute to copy first
    // Create an attribute on child1 to copy
    pugi::xml_attribute attr1 = child1.append_attribute("attr1");
    attr1.set_value("value1");
    root.append_copy(attr1);
    
    // 4. first_child
    pugi::xml_node first_child = root.first_child();
    
    // 5. first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    
    // 6. append_child (second time)
    pugi::xml_node child2 = root.append_child("child2");
    
    // 7. append_copy (second time) - need another attribute to copy
    // Create another attribute on child2
    pugi::xml_attribute attr2 = child2.append_attribute("attr2");
    attr2.set_value("value2");
    root.append_copy(attr2);
    
    // 8. first_attribute (second time)
    pugi::xml_attribute first_attr2 = root.first_attribute();
    
    // Clean up
    delete[] buffer;
    
    return 0;
}