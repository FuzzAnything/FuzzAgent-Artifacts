// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Get current memory management functions
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();
    
    // Set custom memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Use fuzzer input to determine node type
    if (Size > 0) {
        pugi::xml_node_type node_type = static_cast<pugi::xml_node_type>(Data[0] % 9);
        pugi::xml_node child = root.append_child(node_type);
        
        // Get first child
        pugi::xml_node first_child = root.first_child();
        
        // Additional operations to explore more states
        if (Size > 1) {
            // Try another append with different type
            pugi::xml_node_type another_type = static_cast<pugi::xml_node_type>((Data[1] % 8) + 1);
            root.append_child(another_type);
            
            // Get first child again
            pugi::xml_node updated_first_child = root.first_child();
        }
    }
    
    // Restore original memory management functions
    pugi::set_memory_management_functions(alloc_func, dealloc_func);
    
    return 0;
}