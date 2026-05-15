// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create buffer for in-place parsing
    char* buffer = new char[Size];
    std::memcpy(buffer, Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size, pugi::parse_default, pugi::encoding_auto);

    // Only proceed if parsing succeeded
    if (result.status == pugi::status_ok) {
        // Get the root node (document element)
        pugi::xml_node root = doc.document_element();
        
        // If no root element exists, create one
        if (!root) {
            root = doc.append_child("root");
        }

        // 1. First append_child on root
        pugi::xml_node child1 = root.append_child("child1");
        
        // 2. First append_copy on root (copy an attribute if exists)
        pugi::xml_attribute root_attr = root.first_attribute();
        if (root_attr) {
            root.append_copy(root_attr);
        }
        
        // 3. First child of root
        pugi::xml_node first_child = root.first_child();
        
        // 4. First attribute of root
        pugi::xml_attribute first_attr = root.first_attribute();
        
        // 5. Second append_child (on first_child if it exists, otherwise on root)
        pugi::xml_node target_node = first_child ? first_child : root;
        pugi::xml_node child2 = target_node.append_child("child2");
        
        // 6. Second append_copy (copy first_attr if it exists)
        if (first_attr) {
            target_node.append_copy(first_attr);
        }
        
        // 7. First attribute of target_node
        pugi::xml_attribute target_attr = target_node.first_attribute();
        
        // Clean up the buffer
        delete[] buffer;
    } else {
        // Clean up buffer even if parsing failed
        delete[] buffer;
    }

    return 0;
}