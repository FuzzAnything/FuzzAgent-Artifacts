// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6305:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use first byte to determine node types for operations
    uint8_t type_selector = Data[0];
    
    // Define node types based on input byte
    pugi::xml_node_type types[4];
    for (int i = 0; i < 4; i++) {
        types[i] = static_cast<pugi::xml_node_type>((type_selector >> (i * 2)) & 0x03);
        // Ensure valid node type (0-8)
        types[i] = static_cast<pugi::xml_node_type>(types[i] % 9);
    }

    // 1. append_child
    pugi::xml_node appended = root.append_child(types[0]);
    
    // 2. first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. first_child (again)
    pugi::xml_node first2 = root.first_child();
    
    // 4. insert_child_after - insert after the first child if it exists
    if (first2) {
        pugi::xml_node inserted_after = root.insert_child_after(types[1], first2);
    }
    
    // 5. first_child
    pugi::xml_node first3 = root.first_child();
    
    // 6. insert_child_before - insert before the first child if it exists
    if (first3) {
        pugi::xml_node inserted_before = root.insert_child_before(types[2], first3);
    }
    
    // 7. first_child
    pugi::xml_node first4 = root.first_child();
    
    // 8. prepend_child
    pugi::xml_node prepended = root.prepend_child(types[3]);

    return 0;
}