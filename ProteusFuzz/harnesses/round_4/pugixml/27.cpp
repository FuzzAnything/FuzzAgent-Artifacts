// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

static void* custom_allocate(size_t size) {
    return std::malloc(size);
}

static void custom_deallocate(void* ptr) {
    std::free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // 1. Prepare the environment
    // Retrieve current memory management functions
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();

    // Set custom memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Initialize XML document and node
    pugi::xml_document doc;
    pugi::xml_node root = doc;

    // 2. Invoke target functions in specified order with diverse inputs
    // We already called get_memory_allocation_function and get_memory_deallocation_function above.
    // We already called set_memory_management_functions above.

    // Invoke append_child with different node types to explore states
    // Use data from fuzzer to determine node type if possible, but ensure validity
    pugi::xml_node_type type = pugi::node_element;
    if (Size > 0) {
        int val = Data[0] % 9; // 0 to 8, matching enum values roughly
        type = static_cast<pugi::xml_node_type>(val);
    }

    pugi::xml_node child = root.append_child(type);

    // Try appending another child with a name if the first was an element
    if (child.type() == pugi::node_element && Size > 1) {
        // Create a string from fuzzer data for the name
        size_t name_len = (Data[1] % 10) + 1;
        if (Size > 1 + name_len) {
            std::string_view name_view(reinterpret_cast<const char*>(Data + 2), name_len);
            // Note: append_child with name is not directly available in the provided signature list,
            // but the hint mentions string_view_t. The provided signature is append_child(xml_node_type).
            // We stick to the provided signature for strict compliance.
            // However, to make the tree non-trivial, we can append more generic nodes.
            root.append_child(pugi::node_pcdata);
        }
    }

    // Invoke first_child
    pugi::xml_node first = root.first_child();

    // 3. Cleanup
    // xml_document destructor handles cleanup, but we can explicitly load nothing new.
    // Reset memory management to default if needed, though not strictly required for fuzzing stability
    // if custom functions are robust. Here we leave them as set for the duration of the test.
    
    return 0;
}