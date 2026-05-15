// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create XML document and parse input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    if (!result) {
        return 0;
    }

    // Get root node and first child
    pugi::xml_node root = doc.root();
    pugi::xml_node first_child = root.first_child();
    
    if (first_child.empty()) {
        return 0;
    }

    // Create hint attribute for the overloaded function
    pugi::xml_attribute hint;
    
    // First sequence of 8 attribute calls
    // Using both overloads: string_view and const char*
    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0) {
            // Use string_view overload with hint
            std::string_view attr_name = "attr";
            first_child.attribute(attr_name, hint);
        } else {
            // Use const char* overload
            first_child.attribute("attr");
        }
    }

    // Get an attribute to set name
    pugi::xml_attribute attr = first_child.first_attribute();
    if (!attr.empty()) {
        // Use set_name with string_view
        std::string_view new_name = "new_name";
        attr.set_name(new_name);
    }

    // Second sequence of 4 attribute calls
    // Using both overloads: string_view and const char*
    for (int i = 0; i < 4; i++) {
        if (i % 2 == 0) {
            // Use string_view overload with hint
            std::string_view attr_name2 = "attr2";
            first_child.attribute(attr_name2, hint);
        } else {
            // Use const char* overload
            first_child.attribute("attr2");
        }
    }

    return 0;
}