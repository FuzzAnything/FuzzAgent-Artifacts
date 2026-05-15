// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
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
#include <fstream>
#include <string>
#include <string_view>

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Parse XML from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, we can still test with empty document
    if (!result) {
        // Create a minimal document for testing
        doc.append_child("root");
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Test child() function - need null-terminated strings
    std::string child_name1;
    std::string child_name2;
    
    if (Size > 0) {
        // Use first byte modulo 26 to generate a letter for child name
        char c1 = 'a' + (Data[0] % 26);
        child_name1 = std::string(1, c1);
        
        if (Size > 1) {
            char c2 = 'a' + (Data[1] % 26);
            child_name2 = std::string(1, c2);
        } else {
            child_name2 = "b";
        }
    } else {
        child_name1 = "a";
        child_name2 = "b";
    }
    
    // Ensure null termination
    child_name1 += '\0';
    child_name2 += '\0';
    
    // Call child() twice
    pugi::xml_node child1 = root.child(child_name1.c_str());
    pugi::xml_node child2 = root.child(child_name2.c_str());
    
    // Call first_child() and last_child()
    pugi::xml_node first_child = root.first_child();
    pugi::xml_node last_child = root.last_child();
    
    // Create some nodes for moving operations if they don't exist
    if (!child1) {
        child1 = root.append_child(child_name1.c_str());
    }
    if (!child2) {
        child2 = root.append_child(child_name2.c_str());
    }
    
    // Create additional nodes for more complex move operations
    pugi::xml_node node_a = root.append_child("node_a");
    pugi::xml_node node_b = root.append_child("node_b");
    pugi::xml_node node_c = root.append_child("node_c");
    pugi::xml_node node_d = root.append_child("node_d");
    
    // Test prepend_move() twice
    try {
        root.prepend_move(child1);
        root.prepend_move(child2);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test append_move() twice
    try {
        root.append_move(node_a);
        root.append_move(node_b);
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test insert_move_before() twice
    try {
        if (first_child && node_c) {
            root.insert_move_before(node_c, first_child);
        }
        if (last_child && node_d) {
            root.insert_move_before(node_d, last_child);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test insert_move_after() twice
    try {
        if (first_child && node_a) {
            root.insert_move_after(node_a, first_child);
        }
        if (last_child && node_b) {
            root.insert_move_after(node_b, last_child);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test append_move() one more time
    try {
        if (node_c) {
            root.append_move(node_c);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Clean up - remove dummy file
    std::remove("./dummy_file");
    
    return 0;
}