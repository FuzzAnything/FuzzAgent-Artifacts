// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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

static void test_node_functions(pugi::xml_node node) {
    // Test all the target API functions on the given node
    pugi::xml_node next = node.next_sibling();
    pugi::xml_node prev = node.previous_sibling();
    pugi::xml_node first = node.first_child();
    pugi::xml_node last = node.last_child();
    pugi::xml_node parent = node.parent();
    
    // Test children() with a name filter
    if (node.name() != nullptr) {
        std::string_view name_view(node.name());
        auto children_range = node.children(name_view.data());
        // Iterate through the range to exercise the API
        for (auto child : children_range) {
            // Just iterate, no need to do anything else
            (void)child;
        }
    }
    
    // Test children() without a name filter (all children)
    auto all_children_range = node.children();
    for (auto child : all_children_range) {
        (void)child;
    }
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Parse the XML document from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // If parsing succeeded, test the API functions
    if (result) {
        // Test on the document root
        pugi::xml_node root = doc.root();
        test_node_functions(root);
        
        // Test on all nodes in the document
        pugi::xml_node current = doc.first_child();
        while (current) {
            test_node_functions(current);
            
            // Also test on some derived nodes
            pugi::xml_node next_sib = current.next_sibling();
            if (next_sib) {
                test_node_functions(next_sib);
            }
            
            pugi::xml_node prev_sib = current.previous_sibling();
            if (prev_sib) {
                test_node_functions(prev_sib);
            }
            
            pugi::xml_node first_child = current.first_child();
            if (first_child) {
                test_node_functions(first_child);
            }
            
            pugi::xml_node last_child = current.last_child();
            if (last_child) {
                test_node_functions(last_child);
            }
            
            pugi::xml_node parent = current.parent();
            if (parent) {
                test_node_functions(parent);
            }
            
            current = current.next_sibling();
        }
        
        // Test on empty nodes
        pugi::xml_node empty_node;
        test_node_functions(empty_node);
        
        // Test on a node created from null pointer
        pugi::xml_node null_node(nullptr);
        test_node_functions(null_node);
    }
    
    // Clean up the dummy file
    remove("./dummy_file");
    
    return 0;
}