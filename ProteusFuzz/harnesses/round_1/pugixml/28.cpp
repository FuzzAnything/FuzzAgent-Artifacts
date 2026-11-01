// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>

static void* custom_allocate(size_t size) {
    if (size == 0) return nullptr;
    return ::operator new(size);
}

static void custom_deallocate(void* ptr) {
    if (ptr) {
        ::operator delete(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    // Invoke get_memory_allocation_function
    pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
    
    // Invoke get_memory_deallocation_function
    pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();

    // Invoke set_memory_management_functions with custom functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Initialize xml_node (default constructor creates an empty node)
    pugi::xml_node node;

    // 2. Invoke append_child with diverse types to explore states
    // We iterate through possible node types if data suggests, or just try a few
    pugi::xml_node_type types[] = {
        pugi::node_element,
        pugi::node_pcdata,
        pugi::node_cdata,
        pugi::node_comment,
        pugi::node_pi,
        pugi::node_declaration,
        pugi::node_doctype
    };
    
    size_t num_types = sizeof(types) / sizeof(types[0]);
    size_t type_idx = Size % num_types;
    
    // Append a child node
    // Note: append_child on an empty node (node_null) will return an empty node
    // because there is no parent to attach to. This is expected behavior.
    pugi::xml_node child = node.append_child(types[type_idx]);

    // 3. Invoke first_child
    pugi::xml_node first = node.first_child();

    // 4. Cleanup
    // xml_node does not have a reset() method. 
    // Since 'node' was default constructed and we only appended to it (which failed 
    // to attach because node is null), no dynamic memory was allocated by the library 
    // for this specific node structure. 
    // If we had loaded a document, we would use document::reset(), but here we just 
    // let the object go out of scope.
    
    (void)alloc_func;
    (void)dealloc_func;
    (void)child;
    (void)first;

    return 0;
}