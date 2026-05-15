// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Create XML document from fuzz data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (!result) {
        // If parsing fails, create a minimal valid document for testing
        const char* default_xml = "<root><child1/><child2/><child3/></root>";
        doc.load_string(default_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) return 0;
    
    // Test 1: insert_move_after
    // Create some nodes to work with
    pugi::xml_node node1 = root.append_child("node1");
    pugi::xml_node node2 = root.append_child("node2");
    pugi::xml_node node3 = root.append_child("node3");
    
    if (node1 && node2 && node3) {
        // Try moving node1 after node2
        root.insert_move_after(node1, node2);
        
        // Try moving node3 after node1 (which is now after node2)
        root.insert_move_after(node3, node1);
    }
    
    // Test 2: children with name filtering
    // Create named children
    root.append_child("named_child");
    root.append_child("named_child");
    root.append_child("other_child");
    
    // Use fuzz data to create name string (limited to 100 chars for safety)
    std::string child_name;
    if (Size > 0) {
        size_t name_len = Size % 100;
        child_name = std::string(reinterpret_cast<const char*>(Data), std::min(name_len, Size));
    } else {
        child_name = "named_child";
    }
    
    // Iterate children with the name
    for (pugi::xml_node child : root.children(child_name.c_str())) {
        // Access the child node to trigger any potential issues
        (void)child.type();
    }
    
    // Test 3: parent
    if (node1) {
        pugi::xml_node parent = node1.parent();
        (void)parent;
    }
    
    // Test 4: next_sibling
    if (node2) {
        pugi::xml_node next = node2.next_sibling();
        (void)next;
    }
    
    // Test 5: previous_sibling
    if (node3) {
        pugi::xml_node prev = node3.previous_sibling();
        (void)prev;
    }
    
    // Test 6: last_child
    pugi::xml_node last = root.last_child();
    (void)last;
    
    // Additional testing: create a deeper tree structure
    pugi::xml_node deep_node = root.append_child("deep");
    for (int i = 0; i < 5 && i < (int)Size; i++) {
        pugi::xml_node child = deep_node.append_child("level");
        if (i % 2 == 0 && child) {
            child.append_attribute("attr") = "value";
        }
    }
    
    // Test traversal on deep structure
    pugi::xml_node current = deep_node;
    while (current) {
        pugi::xml_node next = current.next_sibling();
        pugi::xml_node prev = current.previous_sibling();
        pugi::xml_node last_child = current.last_child();
        (void)next;
        (void)prev;
        (void)last_child;
        
        // Test children iteration on each node
        for (pugi::xml_node child : current.children()) {
            (void)child.parent();
        }
        
        current = last_child;
    }
    
    // Test moving nodes within the deep structure
    if (deep_node.first_child() && deep_node.last_child()) {
        deep_node.insert_move_after(deep_node.first_child(), deep_node.last_child());
    }
    
    return 0;
}