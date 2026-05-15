// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Get current memory management functions
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();
    
    // Set memory management functions (using the same ones we just retrieved)
    pugi::set_memory_management_functions(alloc_func, dealloc_func);
    
    // Create an XML document and get a node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Use fuzz data to determine node type for append_child
    if (Size > 0) {
        // Use first byte modulo number of node types to determine node type
        pugi::xml_node_type node_type = static_cast<pugi::xml_node_type>(Data[0] % 9);
        
        // Append child with determined node type
        pugi::xml_node appended_child = node.append_child(node_type);
        
        // Get first child of the node
        pugi::xml_node first_child = node.first_child();
        
        // Optional: If we have more data, we can use it to create more diverse tests
        if (Size > 1) {
            // Try appending another child with different type
            pugi::xml_node_type second_type = static_cast<pugi::xml_node_type>((Data[1] % 8) + 1); // Skip node_null
            pugi::xml_node second_child = node.append_child(second_type);
            
            // Get first child again (should still be the same)
            pugi::xml_node first_child_again = node.first_child();
        }
    } else {
        // If no data, test with default node_element type
        pugi::xml_node appended_child = node.append_child();
        pugi::xml_node first_child = node.first_child();
    }
    
    // Cleanup happens automatically when objects go out of scope
    return 0;
}