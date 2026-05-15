// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Ensure null-terminated string for load_string
    char* null_terminated_data = new char[Size + 1];
    if (null_terminated_data == nullptr) {
        return 0;
    }
    std::memcpy(null_terminated_data, Data, Size);
    null_terminated_data[Size] = '\0';

    // Create XML document and load from string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(null_terminated_data);
    
    // Clean up the temporary buffer
    delete[] null_terminated_data;
    
    // If parsing failed, we can still continue with empty document
    // but we should handle potential exceptions
    try {
        // Get child with name "root" (using fixed name for fuzzing)
        pugi::xml_node child_node = doc.child("root");
        
        // Get first child of that node
        pugi::xml_node first_child = child_node.first_child();
        
        // Get next sibling twice
        pugi::xml_node next_sibling1 = first_child.next_sibling();
        pugi::xml_node next_sibling2 = next_sibling1.next_sibling();
        
        // Get last child of the second sibling
        pugi::xml_node last_child = next_sibling2.last_child();
        
        // Get type and value of last child
        pugi::xml_node_type type1 = last_child.type();
        const pugi::char_t* value1 = last_child.value();
        
        // Get type and value again (same node)
        pugi::xml_node_type type2 = last_child.type();
        const pugi::char_t* value2 = last_child.value();
        
        // Get last child of last child
        pugi::xml_node last_child2 = last_child.last_child();
        
        // Get first child of that node
        pugi::xml_node first_child2 = last_child2.first_child();
        
        // Get first child again (same node)
        pugi::xml_node first_child3 = first_child2.first_child();
        
        // Use the values to avoid dead code elimination
        if (value1 != nullptr && value2 != nullptr) {
            // Do nothing, just reference the values
            volatile const pugi::char_t* v1 = value1;
            volatile const pugi::char_t* v2 = value2;
            (void)v1;
            (void)v2;
        }
        
        // Use the types to avoid dead code elimination
        volatile pugi::xml_node_type t1 = type1;
        volatile pugi::xml_node_type t2 = type2;
        (void)t1;
        (void)t2;
        
    } catch (...) {
        // Catch any exceptions to prevent crashes from propagating
    }
    
    return 0;
}