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

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Initialize pugixml document and node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use first byte to determine node types (modulo 9 for valid node_type values)
    if (Size == 0) return 0;
    
    uint8_t node_type_selector = Data[0];
    pugi::xml_node_type type1 = static_cast<pugi::xml_node_type>((node_type_selector) % 9);
    pugi::xml_node_type type2 = static_cast<pugi::xml_node_type>((node_type_selector + 1) % 9);
    pugi::xml_node_type type3 = static_cast<pugi::xml_node_type>((node_type_selector + 2) % 9);
    pugi::xml_node_type type4 = static_cast<pugi::xml_node_type>((node_type_selector + 3) % 9);
    
    // 1. append_child
    pugi::xml_node child1 = root.append_child(type1);
    
    // 2. first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. first_child (again)
    pugi::xml_node first2 = root.first_child();
    
    // 4. insert_child_after
    pugi::xml_node child2 = root.insert_child_after(type2, child1);
    
    // 5. first_child
    pugi::xml_node first3 = root.first_child();
    
    // 6. insert_child_before
    pugi::xml_node child3 = root.insert_child_before(type3, first3);
    
    // 7. first_child
    pugi::xml_node first4 = root.first_child();
    
    // 8. prepend_child
    pugi::xml_node child4 = root.prepend_child(type4);
    
    // Cleanup happens automatically when objects go out of scope
    return 0;
}