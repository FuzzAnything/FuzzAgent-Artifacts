// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
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
#include <vector>
#include <algorithm>

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzing input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // If parsing fails, create a minimal document to work with
    if (!result) {
        static const char* minimal_xml = "<root><child attr=\"value\"/></root>";
        doc.load_string(minimal_xml);
    }
    
    // Create xpath_node_set from document nodes
    std::vector<pugi::xpath_node> nodes;
    
    // Collect all nodes and attributes from the document
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        // Traverse all nodes in document order
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            // Add node itself
            nodes.push_back(pugi::xpath_node(node));
            
            // Add all attributes of this node with their parent
            for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
                nodes.push_back(pugi::xpath_node(attr, node));
            }
            
            // Traverse children recursively
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                nodes.push_back(pugi::xpath_node(child));
                
                // Add attributes of child with their parent
                for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                    nodes.push_back(pugi::xpath_node(attr, child));
                }
            }
        }
    }
    
    // Determine set type based on fuzzing input
    pugi::xpath_node_set::type_t set_type = pugi::xpath_node_set::type_unsorted;
    if (Size > 0) {
        switch (Data[0] % 3) {
            case 0: set_type = pugi::xpath_node_set::type_unsorted; break;
            case 1: set_type = pugi::xpath_node_set::type_sorted; break;
            case 2: set_type = pugi::xpath_node_set::type_sorted_reverse; break;
        }
    }
    
    // Create xpath_node_set from collected nodes
    pugi::xpath_node_set node_set;
    if (!nodes.empty()) {
        node_set = pugi::xpath_node_set(&nodes[0], &nodes[0] + nodes.size(), set_type);
    }
    
    // Call API functions in specified order
    // 1. xpath_node_set::type
    pugi::xpath_node_set::type_t retrieved_type = node_set.type();
    (void)retrieved_type; // Prevent unused variable warning
    
    // 2. xpath_node_set::sort
    bool reverse_sort = false;
    if (Size > 1) {
        reverse_sort = (Data[1] % 2) == 1;
    }
    node_set.sort(reverse_sort);
    
    // 3. xpath_node_set::empty
    bool is_empty = node_set.empty();
    (void)is_empty; // Prevent unused variable warning
    
    // Get an xpath_node from the set (or create one if empty)
    pugi::xpath_node xnode;
    if (!node_set.empty()) {
        // Use fuzzing data to select which node to use
        size_t index = 0;
        if (Size > 2 && node_set.size() > 0) {
            index = Data[2] % node_set.size();
        }
        xnode = node_set[index];
    } else {
        // Create an xpath_node from the root if set is empty
        xnode = pugi::xpath_node(root_node);
    }
    
    // 4. xpath_node::attribute
    pugi::xml_attribute attr = xnode.attribute();
    (void)attr; // Prevent unused variable warning
    
    // 5. xpath_node::parent
    pugi::xml_node parent = xnode.parent();
    (void)parent; // Prevent unused variable warning
    
    // 6. xpath_node::node
    pugi::xml_node node = xnode.node();
    (void)node; // Prevent unused variable warning
    
    // 7. xml_node::root
    if (node) {
        pugi::xml_node doc_root = node.root();
        (void)doc_root; // Prevent unused variable warning
    }
    
    return 0;
}