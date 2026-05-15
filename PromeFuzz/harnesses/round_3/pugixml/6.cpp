// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document from fuzz data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Create xpath_node_set from document nodes
    pugi::xpath_node_set node_set = doc.select_nodes("//*");
    
    // 1. Call xpath_node_set::type
    pugi::xpath_node_set::type_t set_type = node_set.type();
    (void)set_type; // Avoid unused variable warning
    
    // 2. Call xpath_node_set::sort
    // Use fuzz data to determine sort direction
    bool reverse_sort = false;
    if (Size > 0) {
        reverse_sort = (Data[0] & 0x01) != 0;
    }
    node_set.sort(reverse_sort);
    
    // 3. Call xpath_node_set::empty
    bool is_empty = node_set.empty();
    (void)is_empty; // Avoid unused variable warning
    
    // Create xpath_node objects
    pugi::xpath_node xnode;
    if (!node_set.empty()) {
        // Use first node from set if available
        xnode = node_set[0];
    } else {
        // Otherwise create from document root
        xnode = pugi::xpath_node(doc.root());
    }
    
    // 4. Call xpath_node::attribute
    pugi::xml_attribute attr = xnode.attribute();
    (void)attr; // Avoid unused variable warning
    
    // 5. Call xpath_node::parent
    pugi::xml_node parent_node = xnode.parent();
    (void)parent_node; // Avoid unused variable warning
    
    // 6. Call xpath_node::node
    pugi::xml_node node = xnode.node();
    (void)node; // Avoid unused variable warning
    
    // 7. Call xml_node::root
    if (node) {
        pugi::xml_node root_node = node.root();
        (void)root_node; // Avoid unused variable warning
    }
    
    // Additional exploration: create xpath_node from attribute if available
    if (!attr.empty()) {
        // Get the parent node of the attribute using the node from xpath_node
        pugi::xml_node attr_parent = xnode.node();
        if (attr_parent) {
            pugi::xpath_node xnode_from_attr(attr, attr_parent);
            pugi::xml_node node_from_attr = xnode_from_attr.node();
            pugi::xml_node parent_from_attr = xnode_from_attr.parent();
            (void)node_from_attr;
            (void)parent_from_attr;
        }
    }
    
    return 0;
}