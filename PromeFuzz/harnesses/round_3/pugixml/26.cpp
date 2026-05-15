// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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

static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Get current memory management functions
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();

    // Set custom memory management functions based on fuzzer input
    if (Data[0] % 2 == 0) {
        pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    } else {
        pugi::set_memory_management_functions(alloc_func, dealloc_func);
    }

    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);

    // Append child with node type determined by fuzzer input
    pugi::xml_node_type node_type = static_cast<pugi::xml_node_type>((Data[0] % 9) + 1);
    pugi::xml_node child = root.append_child(node_type);

    // Get first child
    pugi::xml_node first_child = root.first_child();

    // Clean up - objects will be automatically destroyed when they go out of scope
    // The custom memory management functions will handle deallocation if set

    return 0;
}