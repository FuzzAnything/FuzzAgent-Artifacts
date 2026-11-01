// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string_view>

// Custom memory management functions for fuzzing
// Must use malloc/free to match pugixml's internal expectations when overriding
static void* custom_allocate(size_t size) {
    if (size == 0) return nullptr;
    return std::malloc(size);
}

static void custom_deallocate(void* ptr) {
    if (ptr) {
        std::free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Step 1: Get current memory allocation function
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    
    // Step 2: Get current memory deallocation function
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();
    
    // Step 3: Set custom memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
    
    // Step 4: Create an XML document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc;
    
    // Step 5: Append a child node (element type)
    pugi::xml_node child = root.append_child(pugi::node_element);
    if (child) {
        // Try to append another child to test deeper structures
        pugi::xml_node grandchild = child.append_child(pugi::node_pcdata);
        (void)grandchild; // Suppress unused warning
    }
    
    // Step 6: Get the first child of the root
    pugi::xml_node first = root.first_child();
    (void)first; // Suppress unused warning
    
    // Additional exploration: try appending different node types
    pugi::xml_node comment_node = root.append_child(pugi::node_comment);
    (void)comment_node;
    
    pugi::xml_node cdata_node = root.append_child(pugi::node_cdata);
    (void)cdata_node;
    
    pugi::xml_node pi_node = root.append_child(pugi::node_pi);
    (void)pi_node;
    
    // Test with null-like scenarios (though append_child on valid node should work)
    pugi::xml_node null_node;
    pugi::xml_node null_child = null_node.append_child(pugi::node_element);
    (void)null_child;
    
    pugi::xml_node null_first = null_node.first_child();
    (void)null_first;
    
    // Reset memory management to default (optional, but good practice)
    pugi::set_memory_management_functions(alloc_func, dealloc_func);
    
    return 0;
}