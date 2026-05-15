// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
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

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Get current memory management functions
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();
    
    // Set custom memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use fuzz data to determine node type for append_child
    if (Size > 0) {
        uint8_t node_type_byte = Data[0] % 9; // 9 node types in xml_node_type enum
        pugi::xml_node_type type = static_cast<pugi::xml_node_type>(node_type_byte);
        
        // Append child with determined type
        pugi::xml_node child = root.append_child(type);
        
        // Get first child
        pugi::xml_node first_child = root.first_child();
        
        // Try to explore more states by using different node types
        if (Size > 1) {
            uint8_t second_type_byte = Data[1] % 9;
            pugi::xml_node_type second_type = static_cast<pugi::xml_node_type>(second_type_byte);
            pugi::xml_node second_child = root.append_child(second_type);
        }
    } else {
        // Default behavior with minimal input
        pugi::xml_node child = root.append_child();
        pugi::xml_node first_child = root.first_child();
    }
    
    // Reset to default memory management functions
    pugi::set_memory_management_functions(alloc_func, dealloc_func);
    
    return 0;
}