// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
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
#include <fstream>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Parse XML from input buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        return 0;
    }

    // Get root node to start traversal
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare name for child() function - ensure null termination
    std::string child_name;
    if (Size > 0) {
        size_t name_len = Size < 256 ? Size : 255;
        child_name.assign(reinterpret_cast<const char*>(Data), name_len);
        child_name.push_back('\0');
    } else {
        child_name = "dummy";
    }

    // Function sequence as specified
    pugi::xml_node child_node = root.child(child_name.c_str());
    pugi::xml_attribute first_attr = child_node.first_attribute();
    pugi::xml_node first_child = child_node.first_child();
    
    // Internal object calls
    (void)child_node.internal_object();
    (void)first_attr.internal_object();
    (void)first_child.internal_object();
    (void)child_node.internal_object();
    (void)child_node.internal_object();
    (void)first_attr.internal_object();
    (void)child_node.internal_object();
    (void)first_attr.internal_object();

    return 0;
}