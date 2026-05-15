// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Get root node (document node)
    pugi::xml_node root = doc;
    
    // Prepare names for child nodes
    std::string_view name1, name2;
    
    // Use first part of data for first name
    if (Size > 0) {
        size_t name1_len = std::min(Size, (size_t)100);
        name1 = std::string_view(reinterpret_cast<const char*>(Data), name1_len);
    }
    
    // Use second part of data for second name (if available)
    if (Size > 100) {
        size_t name2_len = std::min(Size - 100, (size_t)100);
        name2 = std::string_view(reinterpret_cast<const char*>(Data + 100), name2_len);
    }
    
    // 1. First append_child with name
    if (!name1.empty()) {
        // Ensure null termination for C string
        std::string name1_str(name1);
        root.append_child(name1_str.c_str());
    }
    
    // 2. Second append_child with node type (default is node_element)
    root.append_child(pugi::node_element);
    
    // 3. First child call
    if (!name1.empty()) {
        std::string name1_str(name1);
        root.child(name1_str.c_str());
    }
    
    // 4. First append_buffer call
    if (Size > 0) {
        // Use the entire input data as buffer
        root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    }
    
    // 5. Second child call
    if (!name1.empty()) {
        std::string name1_str(name1);
        root.child(name1_str.c_str());
    }
    
    // 6. Third child call
    if (!name2.empty()) {
        std::string name2_str(name2);
        root.child(name2_str.c_str());
    }
    
    // 7. remove_children
    root.remove_children();
    
    // 8. Fourth child call
    if (!name1.empty()) {
        std::string name1_str(name1);
        root.child(name1_str.c_str());
    }
    
    // 9. Second append_buffer call
    if (Size > 0) {
        // Use the entire input data as buffer again
        root.append_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    }
    
    // 10. Fifth child call
    if (!name1.empty()) {
        std::string name1_str(name1);
        root.child(name1_str.c_str());
    }
    
    return 0;
}