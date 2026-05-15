// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a document with a node and attribute to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    pugi::xml_attribute attr = node.append_attribute("attr");

    // Use the fuzz data to create string views
    std::string_view str_view(reinterpret_cast<const char*>(Data), Size);
    
    // Test pugi::xml_node::set_name
    if (Size > 0) {
        node.set_name(str_view.data(), str_view.size());
    }

    // Test pugi::xml_attribute::set_value overloads
    if (Size >= sizeof(unsigned long)) {
        unsigned long ul_val;
        memcpy(&ul_val, Data, sizeof(unsigned long));
        attr.set_value(ul_val);
    }
    
    if (Size >= sizeof(long)) {
        long l_val;
        memcpy(&l_val, Data, sizeof(long));
        attr.set_value(l_val);
    }

    // Test pugi::xml_attribute::set_name overloads
    if (Size > 0) {
        // Overload with size parameter
        attr.set_name(str_view.data(), str_view.size());
        
        // Overload without size parameter - ensure null termination
        static char null_terminated[4096];
        size_t copy_size = Size < 4095 ? Size : 4095;
        memcpy(null_terminated, Data, copy_size);
        null_terminated[copy_size] = '\0';
        attr.set_name(null_terminated);
    }

    // Test pugi::xml_node::set_value
    // Create a node type that supports values
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    if (Size > 0) {
        comment_node.set_value(str_view.data(), str_view.size());
    }

    return 0;
}