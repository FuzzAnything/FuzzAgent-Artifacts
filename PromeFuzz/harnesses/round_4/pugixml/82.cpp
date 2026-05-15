// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create two XML documents to work with
    pugi::xml_document doc1;
    pugi::xml_document doc2;
    
    // Create root nodes for both documents
    pugi::xml_node root1 = doc1.append_child("root1");
    pugi::xml_node root2 = doc2.append_child("root2");
    
    // Create some initial child nodes to work with
    pugi::xml_node child1 = root1.append_child("child1");
    pugi::xml_node child2 = root1.append_child("child2");
    pugi::xml_node child3 = root1.append_child("child3");
    pugi::xml_node child4 = root2.append_child("child4");
    
    // Create a null node for testing
    pugi::xml_node null_node;
    
    // Array of nodes to choose from for operations
    pugi::xml_node nodes[] = {root1, child1, child2, child3, root2, child4, null_node};
    const size_t node_count = sizeof(nodes) / sizeof(nodes[0]);
    
    // Use first byte to determine which function to test
    uint8_t func_selector = Data[0];
    
    // Use subsequent bytes for string data if needed
    size_t data_offset = 1;
    std::string_view node_name;
    if (data_offset < Size) {
        size_t name_len = std::min<size_t>(Size - data_offset, Data[data_offset]);
        data_offset++;
        if (name_len > 0 && data_offset + name_len <= Size) {
            node_name = std::string_view(reinterpret_cast<const char*>(Data + data_offset), name_len);
        }
    }
    
    // Select nodes based on input bytes
    size_t target_idx = (Size > 1) ? (Data[1] % node_count) : 0;
    size_t ref_idx = (Size > 2) ? (Data[2] % node_count) : 0;
    size_t move_idx = (Size > 3) ? (Data[3] % node_count) : 0;
    
    pugi::xml_node target_node = nodes[target_idx];
    pugi::xml_node ref_node = nodes[ref_idx];
    pugi::xml_node move_node = nodes[move_idx];
    
    // Test different functions based on selector
    switch (func_selector % 6) {
        case 0:
            // Test insert_child_before
            target_node.insert_child_before(node_name, ref_node);
            break;
        case 1:
            // Test prepend_move
            target_node.prepend_move(move_node);
            break;
        case 2:
            // Test append_child
            target_node.append_child(node_name);
            break;
        case 3:
            // Test insert_move_before
            target_node.insert_move_before(move_node, ref_node);
            break;
        case 4:
            // Test prepend_child
            target_node.prepend_child(node_name);
            break;
        case 5:
            // Test insert_child_after
            target_node.insert_child_after(node_name, ref_node);
            break;
    }
    
    // Additional random operations to explore more states
    if (Size > 4) {
        uint8_t extra_op = Data[4];
        
        // Create more nodes with random names
        if (extra_op % 3 == 0) {
            char temp_name[32];
            size_t copy_len = std::min<size_t>(31, Size - 5);
            if (copy_len > 0) {
                memcpy(temp_name, Data + 5, copy_len);
                temp_name[copy_len] = '\0';
                std::string_view extra_name(temp_name, copy_len);
                
                // Try operations on different nodes
                pugi::xml_node extra_node = nodes[extra_op % node_count];
                extra_node.append_child(extra_name);
                
                if (extra_op % 5 == 0) {
                    pugi::xml_node another_node = nodes[(extra_op + 1) % node_count];
                    extra_node.insert_child_before(extra_name, another_node);
                }
            }
        }
    }
    
    return 0;
}