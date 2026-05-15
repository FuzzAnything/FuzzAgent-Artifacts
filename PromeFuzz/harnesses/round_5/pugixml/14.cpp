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

static pugi::xml_node_type get_node_type_from_byte(uint8_t byte) {
    const pugi::xml_node_type types[] = {
        pugi::node_null,
        pugi::node_document,
        pugi::node_element,
        pugi::node_pcdata,
        pugi::node_cdata,
        pugi::node_comment,
        pugi::node_pi,
        pugi::node_declaration,
        pugi::node_doctype
    };
    return types[byte % (sizeof(types) / sizeof(types[0]))];
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // 1. append_child
    pugi::xml_node child1 = root.append_child(get_node_type_from_byte(Data[0 % Size]));
    
    // 2. first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. first_child
    pugi::xml_node first2 = root.first_child();
    
    // 4. insert_child_after
    pugi::xml_node child2 = root.insert_child_after(
        get_node_type_from_byte(Data[1 % Size]), 
        child1
    );
    
    // 5. first_child
    pugi::xml_node first3 = root.first_child();
    
    // 6. insert_child_before
    pugi::xml_node child3 = root.insert_child_before(
        get_node_type_from_byte(Data[2 % Size]), 
        child1
    );
    
    // 7. first_child
    pugi::xml_node first4 = root.first_child();
    
    // 8. prepend_child
    pugi::xml_node child4 = root.prepend_child(get_node_type_from_byte(Data[3 % Size]));
    
    return 0;
}