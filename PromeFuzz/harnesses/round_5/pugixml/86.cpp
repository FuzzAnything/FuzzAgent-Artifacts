// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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
#include <stdint.h>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Use the input data to create a string for node names
    std::string_view node_name(reinterpret_cast<const char*>(Data), Size);
    
    // Create XML documents and nodes to work with
    pugi::xml_document doc1;
    pugi::xml_document doc2;
    
    // Create root nodes
    pugi::xml_node root1 = doc1.append_child("root1");
    pugi::xml_node root2 = doc2.append_child("root2");
    
    // Create some child nodes for testing
    pugi::xml_node child1 = root1.append_child("child1");
    pugi::xml_node child2 = root1.append_child("child2");
    pugi::xml_node child3 = root1.append_child("child3");
    pugi::xml_node child4 = root2.append_child("child4");
    
    // Test insert_child_before
    if (!node_name.empty()) {
        pugi::xml_node new_node = root1.insert_child_before(node_name, child2);
        // Try with empty string_view
        root1.insert_child_before(std::string_view(), child3);
    }
    
    // Test prepend_move
    root1.prepend_move(child3);  // Move within same document
    root1.prepend_move(child4);  // Try moving from different document (should fail)
    
    // Test prepend_child
    if (!node_name.empty()) {
        pugi::xml_node prepended = root1.prepend_child(node_name);
        // Try with empty string_view
        root1.prepend_child(std::string_view());
    }
    
    // Test insert_move_before
    root1.insert_move_before(child1, child2);  // Move within same document
    root1.insert_move_before(child4, child2);  // Try moving from different document (should fail)
    root1.insert_move_before(child2, child2);  // Try moving same node before itself (should fail)
    
    // Test insert_child_after
    if (!node_name.empty()) {
        pugi::xml_node inserted_after = root1.insert_child_after(node_name, child1);
        // Try with empty string_view
        root1.insert_child_after(std::string_view(), child2);
    }
    
    // Test append_child
    if (!node_name.empty()) {
        pugi::xml_node appended = root1.append_child(node_name);
        // Try with empty string_view
        root1.append_child(std::string_view());
    }
    
    // Create more complex scenarios with nested nodes
    pugi::xml_node nested = root1.append_child("nested");
    pugi::xml_node nested_child1 = nested.append_child("nested_child1");
    pugi::xml_node nested_child2 = nested.append_child("nested_child2");
    
    // Test operations on nested nodes
    if (!node_name.empty()) {
        nested.insert_child_before(node_name, nested_child2);
        nested.prepend_child(node_name);
        nested.insert_child_after(node_name, nested_child1);
        nested.append_child(node_name);
    }
    
    // Test moving nested nodes
    nested.prepend_move(nested_child2);
    nested.insert_move_before(nested_child1, nested_child2);
    
    // Test with null/empty nodes
    pugi::xml_node null_node;
    root1.insert_child_before("test", null_node);
    root1.prepend_move(null_node);
    root1.insert_move_before(null_node, child1);
    root1.insert_move_before(child1, null_node);
    root1.insert_child_after("test", null_node);
    
    return 0;
}