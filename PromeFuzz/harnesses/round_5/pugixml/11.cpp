// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
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
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    const char* filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    // If parsing failed, create a minimal valid XML document to work with
    if (!result) {
        const char* minimal_xml = "<root><child1/><child2/></root>";
        result = doc.load_string(minimal_xml);
        if (!result) {
            return 0;
        }
    }

    // Get the root node
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare null-terminated strings for child() calls
    std::string child_name1 = "child1";
    std::string child_name2 = "child2";
    
    // Ensure strings are null-terminated
    if (Size > 0) {
        // Use fuzzer data to create node names if possible
        size_t len1 = Size / 2;
        size_t len2 = Size - len1;
        
        if (len1 > 0) {
            child_name1.assign(reinterpret_cast<const char*>(Data), len1);
            // Ensure null termination
            if (child_name1.back() != '\0') {
                child_name1 += '\0';
            }
        }
        
        if (len2 > 0) {
            child_name2.assign(reinterpret_cast<const char*>(Data + len1), len2);
            // Ensure null termination
            if (child_name2.back() != '\0') {
                child_name2 += '\0';
            }
        }
    }

    // Call the functions in the specified order
    // 1. pugi::xml_node::child
    pugi::xml_node child1 = root.child(child_name1.c_str());
    
    // 2. pugi::xml_node::child
    pugi::xml_node child2 = root.child(child_name2.c_str());
    
    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    
    // 4. pugi::xml_node::last_child
    pugi::xml_node last_child = root.last_child();
    
    // Create some nodes to move if they don't exist
    pugi::xml_node move_node1 = doc.append_child("move1");
    pugi::xml_node move_node2 = doc.append_child("move2");
    pugi::xml_node move_node3 = doc.append_child("move3");
    pugi::xml_node move_node4 = doc.append_child("move4");
    
    // 5. pugi::xml_node::prepend_move
    if (move_node1 && root) {
        root.prepend_move(move_node1);
    }
    
    // 6. pugi::xml_node::prepend_move
    if (move_node2 && root) {
        root.prepend_move(move_node2);
    }
    
    // 7. pugi::xml_node::append_move
    if (move_node3 && root) {
        root.append_move(move_node3);
    }
    
    // 8. pugi::xml_node::append_move
    if (move_node4 && root) {
        root.append_move(move_node4);
    }
    
    // Get reference nodes for insert operations
    pugi::xml_node ref_node1 = root.first_child();
    pugi::xml_node ref_node2 = root.last_child();
    
    // Create more nodes for insertion moves
    pugi::xml_node insert_node1 = doc.append_child("insert1");
    pugi::xml_node insert_node2 = doc.append_child("insert2");
    pugi::xml_node insert_node3 = doc.append_child("insert3");
    pugi::xml_node insert_node4 = doc.append_child("insert4");
    
    // 9. pugi::xml_node::insert_move_before
    if (insert_node1 && ref_node1 && root) {
        root.insert_move_before(insert_node1, ref_node1);
    }
    
    // 10. pugi::xml_node::insert_move_before
    if (insert_node2 && ref_node2 && root) {
        root.insert_move_before(insert_node2, ref_node2);
    }
    
    // 11. pugi::xml_node::insert_move_after
    if (insert_node3 && ref_node1 && root) {
        root.insert_move_after(insert_node3, ref_node1);
    }
    
    // 12. pugi::xml_node::insert_move_after
    if (insert_node4 && ref_node2 && root) {
        root.insert_move_after(insert_node4, ref_node2);
    }
    
    // Create one more node for final append_move
    pugi::xml_node final_node = doc.append_child("final");
    
    // 13. pugi::xml_node::append_move
    if (final_node && root) {
        root.append_move(final_node);
    }

    return 0;
}