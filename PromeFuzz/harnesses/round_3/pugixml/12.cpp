// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        return 0;
    }

    // Get root node
    pugi::xml_node root = doc.root();
    if (root.empty()) {
        return 0;
    }

    // Prepare string views for child name lookups
    // Use a small subset of data to create null-terminated strings
    const char* child_name1 = "child1";
    const char* child_name2 = "child2";
    
    // If we have enough data, use actual bytes for names
    static char name_buf1[256];
    static char name_buf2[256];
    
    if (Size >= 2) {
        size_t len1 = std::min(Size/2, sizeof(name_buf1)-1);
        memcpy(name_buf1, Data, len1);
        name_buf1[len1] = '\0';
        
        size_t len2 = std::min(Size - len1, sizeof(name_buf2)-1);
        memcpy(name_buf2, Data + len1, len2);
        name_buf2[len2] = '\0';
        
        child_name1 = name_buf1;
        child_name2 = name_buf2;
    }

    // Call functions in specified order
    pugi::xml_node child1 = root.child(child_name1);
    pugi::xml_node child2 = root.child(child_name2);
    pugi::xml_node first_child = root.first_child();
    pugi::xml_node last_child = root.last_child();
    
    // For move operations, we need valid nodes to move
    // Try to use children if available
    pugi::xml_node node_to_move1 = first_child;
    pugi::xml_node node_to_move2 = last_child;
    
    // If we don't have valid nodes, create some
    pugi::xml_node dummy1, dummy2;
    if (node_to_move1.empty()) {
        dummy1 = root.append_child("dummy1");
        node_to_move1 = dummy1;
    }
    if (node_to_move2.empty() || node_to_move2 == node_to_move1) {
        dummy2 = root.append_child("dummy2");
        node_to_move2 = dummy2;
    }
    
    // Perform move operations
    if (!node_to_move1.empty()) {
        root.prepend_move(node_to_move1);
    }
    if (!node_to_move2.empty()) {
        root.prepend_move(node_to_move2);
    }
    
    if (!node_to_move1.empty()) {
        root.append_move(node_to_move1);
    }
    if (!node_to_move2.empty()) {
        root.append_move(node_to_move2);
    }
    
    // For insert_move_before/after, we need reference nodes
    pugi::xml_node ref_node1 = root.first_child();
    pugi::xml_node ref_node2 = root.last_child();
    
    if (!node_to_move1.empty() && !ref_node1.empty() && node_to_move1 != ref_node1) {
        root.insert_move_before(node_to_move1, ref_node1);
    }
    if (!node_to_move2.empty() && !ref_node1.empty() && node_to_move2 != ref_node1) {
        root.insert_move_before(node_to_move2, ref_node1);
    }
    
    if (!node_to_move1.empty() && !ref_node2.empty() && node_to_move1 != ref_node2) {
        root.insert_move_after(node_to_move1, ref_node2);
    }
    if (!node_to_move2.empty() && !ref_node2.empty() && node_to_move2 != ref_node2) {
        root.insert_move_after(node_to_move2, ref_node2);
    }
    
    // Final append_move
    if (!node_to_move1.empty()) {
        root.append_move(node_to_move1);
    }

    return 0;
}