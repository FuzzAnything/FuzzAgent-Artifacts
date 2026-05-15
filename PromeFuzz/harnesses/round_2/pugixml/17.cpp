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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Initialize document
    pugi::xml_document doc;
    
    // Create a root node to work with
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use fuzzer data to determine node types
    // We'll use modulo to get valid enum values (0-8)
    pugi::xml_node_type type1 = pugi::xml_node_type(Data[0] % 9);
    pugi::xml_node_type type2 = pugi::xml_node_type(Data[1] % 9);
    pugi::xml_node_type type3 = pugi::xml_node_type(Data[2] % 9);
    pugi::xml_node_type type4 = pugi::xml_node_type(Data[3] % 9);
    
    // 1. append_child
    pugi::xml_node child1 = root.append_child(type1);
    
    // 2. first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. first_child (again)
    pugi::xml_node first2 = root.first_child();
    
    // 4. insert_child_after
    // We need a valid node to insert after - use the child we just created
    pugi::xml_node inserted_after;
    if (first1) {
        inserted_after = root.insert_child_after(type2, first1);
    }
    
    // 5. first_child
    pugi::xml_node first3 = root.first_child();
    
    // 6. insert_child_before
    // We need a valid node to insert before - use the first child
    pugi::xml_node inserted_before;
    if (first3) {
        inserted_before = root.insert_child_before(type3, first3);
    }
    
    // 7. first_child
    pugi::xml_node first4 = root.first_child();
    
    // 8. prepend_child
    pugi::xml_node prepended = root.prepend_child(type4);
    
    return 0;
}