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
    // Create a document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use fuzzer input to determine node types
    // We'll use the first few bytes to determine node types for operations
    // If input is too small, use defaults
    pugi::xml_node_type type1 = pugi::node_element;
    pugi::xml_node_type type2 = pugi::node_element;
    pugi::xml_node_type type3 = pugi::node_element;
    pugi::xml_node_type type4 = pugi::node_element;
    
    if (Size > 0) {
        type1 = static_cast<pugi::xml_node_type>((Data[0] % 9) + 1); // +1 to skip node_null
    }
    if (Size > 1) {
        type2 = static_cast<pugi::xml_node_type>((Data[1] % 9) + 1);
    }
    if (Size > 2) {
        type3 = static_cast<pugi::xml_node_type>((Data[2] % 9) + 1);
    }
    if (Size > 3) {
        type4 = static_cast<pugi::xml_node_type>((Data[3] % 9) + 1);
    }
    
    // 1. append_child
    pugi::xml_node child1 = root.append_child(type1);
    
    // 2. first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. first_child (again)
    pugi::xml_node first2 = root.first_child();
    
    // 4. insert_child_after
    pugi::xml_node inserted_after;
    if (first1) {
        inserted_after = root.insert_child_after(type2, first1);
    }
    
    // 5. first_child
    pugi::xml_node first3 = root.first_child();
    
    // 6. insert_child_before
    pugi::xml_node inserted_before;
    if (first3) {
        inserted_before = root.insert_child_before(type3, first3);
    }
    
    // 7. first_child
    pugi::xml_node first4 = root.first_child();
    
    // 8. prepend_child
    pugi::xml_node prepended = root.prepend_child(type4);
    
    // No explicit cleanup needed - doc will be destroyed automatically
    
    return 0;
}