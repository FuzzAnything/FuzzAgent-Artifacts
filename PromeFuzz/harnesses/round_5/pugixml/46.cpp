// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Ensure null-terminated string for load_string
    char* xml_string = new char[Size + 1];
    if (!xml_string) return 0;
    std::memcpy(xml_string, Data, Size);
    xml_string[Size] = '\0';

    // Create document and load XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_string);
    
    // Clean up the temporary string
    delete[] xml_string;
    
    // Only proceed if parsing was successful
    if (result.status == pugi::status_ok) {
        // Get root node and call child() with a fixed name
        pugi::xml_node root = doc.root();
        static const char* child_name = "test";
        pugi::xml_node child_node = root.child(child_name);
        
        // Call first_child on the child node
        pugi::xml_node first_child = child_node.first_child();
        
        // Call next_sibling twice
        pugi::xml_node next_sib1 = first_child.next_sibling();
        pugi::xml_node next_sib2 = next_sib1.next_sibling();
        
        // Call last_child on the child node
        pugi::xml_node last_child = child_node.last_child();
        
        // Call type and value on last_child
        pugi::xml_node_type type1 = last_child.type();
        const char* value1 = last_child.value();
        
        // Call type and value on first_child
        pugi::xml_node_type type2 = first_child.type();
        const char* value2 = first_child.value();
        
        // Call last_child on first_child
        pugi::xml_node last_child2 = first_child.last_child();
        
        // Call first_child on last_child2
        pugi::xml_node first_child2 = last_child2.first_child();
        
        // Call first_child on first_child2
        pugi::xml_node first_child3 = first_child2.first_child();
    }
    
    return 0;
}