// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // 1. Prepare the environment
    // Retrieve current memory management functions first
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();

    // Set memory management functions (using the retrieved ones to ensure stability, 
    // or we could pass custom ones if needed, but using existing ones is safer for fuzzing)
    pugi::set_memory_management_functions(alloc_func, dealloc_func);

    // Create a document to work with
    pugi::xml_document doc;

    // Parse the fuzz input as XML
    // We use the data directly. If Size is 0, parsing will fail gracefully.
    pugi::xml_parse_result result = doc.load_string(reinterpret_cast<const char*>(Data), Size);

    // If parsing failed, we can still test the node functions on the empty document
    // or the root node, which handles errors gracefully.

    // 2. Invoke target functions in the specified order
    // Note: The prompt asks for specific order: get_alloc -> get_dealloc -> set_mem -> append_child -> first_child
    // We already did the first three above. Now we do append_child and first_child.

    // Get the root node
    pugi::xml_node root = doc;

    // Fuzz append_child with different node types based on input data
    // We use the first byte of data to determine the node type to explore different states
    pugi::xml_node_type type = pugi::node_element;
    if (Size > 0) {
        int type_idx = Data[0] % 9; // 0 to 8 (node_null to node_doctype)
        type = static_cast<pugi::xml_node_type>(type_idx);
    }

    // Invoke append_child
    // This adds a child of the specified type to the root
    pugi::xml_node new_child = root.append_child(type);

    // If we successfully added an element, we might want to add text or attributes based on more input
    // But the primary requirement is just to call append_child.
    
    // Invoke first_child
    // This retrieves the first child of the root node
    pugi::xml_node first = root.first_child();

    // Additional exploration: Try to append to the new child if it's valid
    if (!new_child.empty()) {
        // Try appending another child to the newly created node
        pugi::xml_node grandchild = new_child.append_child(pugi::node_pcdata);
        // Try getting first child of the new node
        pugi::xml_node gc_first = new_child.first_child();
    }

    // 3. Cleanup
    // pugixml xml_document automatically cleans up when it goes out of scope.
    // No explicit deallocation needed for the document itself.
    // The memory management functions set earlier are restored to default behavior 
    // when the program exits, or we can reset them if we were in a long-running process.
    // For a fuzzer, letting the process exit is sufficient.

    return 0;
}