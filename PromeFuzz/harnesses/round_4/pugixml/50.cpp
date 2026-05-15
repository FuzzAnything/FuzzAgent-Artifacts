// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Create a writable buffer for load_buffer_inplace
    char* buffer = new char[Size];
    if (buffer == nullptr) return 0;
    std::memcpy(buffer, Data, Size);
    
    // Step 1: Load document from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Step 2: Append child to root
    pugi::xml_node child1 = root.append_child("child1");
    
    // Step 3: Append copy of an attribute (need to create one first)
    pugi::xml_attribute attr1 = child1.append_attribute("attr1");
    root.append_copy(attr1);
    
    // Step 4: Get first child
    pugi::xml_node first_child = root.first_child();
    
    // Step 5: Get first attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    
    // Step 6: Append another child
    pugi::xml_node child2 = root.append_child("child2");
    
    // Step 7: Append copy of first attribute if it exists
    if (first_attr) {
        root.append_copy(first_attr);
    }
    
    // Step 8: Get first attribute again
    pugi::xml_attribute first_attr2 = root.first_attribute();
    
    // Cleanup
    delete[] buffer;
    return 0;
}