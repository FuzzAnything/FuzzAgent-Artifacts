// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <string>
#include <fstream>

static void traverse_and_test(pugi::xml_node node) {
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
        (void)child.offset_debug();
        (void)child.parent();
        (void)child.last_child();
        (void)child.next_sibling();
        (void)child.previous_sibling();
        (void)child.first_child();
        
        // Recursively traverse children
        traverse_and_test(child);
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Write fuzz data to a file for XML parsing
    static const char* dummy_file = "./dummy_file";
    std::ofstream out(dummy_file, std::ios::binary);
    out.write(reinterpret_cast<const char*>(Data), Size);
    out.close();

    // Parse XML from the file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(dummy_file);
    
    // If parsing succeeded, test the API functions on various nodes
    if (result) {
        // Test on document root
        pugi::xml_node root = doc.root();
        (void)root.offset_debug();
        (void)root.parent();
        (void)root.last_child();
        (void)root.next_sibling();
        (void)root.previous_sibling();
        (void)root.first_child();
        
        // Test on all nodes in the document using traversal
        traverse_and_test(root);
        
        // Test on empty nodes
        pugi::xml_node empty_node;
        (void)empty_node.offset_debug();
        (void)empty_node.parent();
        (void)empty_node.last_child();
        (void)empty_node.next_sibling();
        (void)empty_node.previous_sibling();
        (void)empty_node.first_child();
        
        // Test on specific node types if they exist
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            (void)first_child.offset_debug();
            (void)first_child.parent();
            (void)first_child.last_child();
            (void)first_child.next_sibling();
            (void)first_child.previous_sibling();
            (void)first_child.first_child();
            
            // Test sibling traversal
            pugi::xml_node next_sib = first_child.next_sibling();
            if (next_sib) {
                (void)next_sib.offset_debug();
                (void)next_sib.parent();
                (void)next_sib.last_child();
                (void)next_sib.next_sibling();
                (void)next_sib.previous_sibling();
                (void)next_sib.first_child();
            }
            
            pugi::xml_node prev_sib = first_child.previous_sibling();
            if (prev_sib) {
                (void)prev_sib.offset_debug();
                (void)prev_sib.parent();
                (void)prev_sib.last_child();
                (void)prev_sib.next_sibling();
                (void)prev_sib.previous_sibling();
                (void)prev_sib.first_child();
            }
            
            // Test parent-child relationships
            pugi::xml_node parent = first_child.parent();
            if (parent) {
                (void)parent.offset_debug();
                (void)parent.parent();
                (void)parent.last_child();
                (void)parent.next_sibling();
                (void)parent.previous_sibling();
                (void)parent.first_child();
            }
            
            // Test last child
            pugi::xml_node last_child = parent.last_child();
            if (last_child) {
                (void)last_child.offset_debug();
                (void)last_child.parent();
                (void)last_child.last_child();
                (void)last_child.next_sibling();
                (void)last_child.previous_sibling();
                (void)last_child.first_child();
            }
        }
    }
    
    // Also test with string parsing for different code paths
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_string(xml_string.c_str());
    
    if (result2) {
        pugi::xml_node root2 = doc2.root();
        (void)root2.offset_debug();
        (void)root2.parent();
        (void)root2.last_child();
        (void)root2.next_sibling();
        (void)root2.previous_sibling();
        (void)root2.first_child();
    }
    
    return 0;
}