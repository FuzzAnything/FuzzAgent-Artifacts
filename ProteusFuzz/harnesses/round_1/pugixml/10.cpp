// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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

static void* custom_allocate(size_t size) {
    return std::malloc(size);
}

static void custom_deallocate(void* ptr) {
    std::free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment: Set memory management functions first
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    pugi::xml_document doc;
    
    // Load XML from fuzz input
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    // We proceed regardless of parse result to test robustness on empty/invalid structures

    pugi::xml_node root = doc;

    // Target API Sequence:
    // 1. pugi::xml_node::first_child
    pugi::xml_node node1 = root.first_child();

    // 2. pugi::xml_node::next_sibling
    pugi::xml_node node2 = node1.next_sibling();

    // 3. pugi::xml_node::value
    const pugi::char_t* val = node2.value();
    (void)val; // Suppress unused warning

    // 4. pugi::xml_node::set_value
    // Attempt to set value. Returns false for elements, true for text/comment/etc.
    std::string dummy_str("fuzz_value");
    node2.set_value(dummy_str.c_str());

    // 5. pugi::xml_node::first_child
    pugi::xml_node node3 = node2.first_child();

    // 6. pugi::xml_node::next_sibling
    pugi::xml_node node4 = node3.next_sibling();

    // 7. pugi::xml_node::next_sibling (again)
    pugi::xml_node node5 = node4.next_sibling();

    // 8. pugi::xml_node::parent
    pugi::xml_node node6 = node5.parent();

    // 9. pugi::xml_node::remove_child
    // Check if nodes are valid before attempting removal.
    // In pugixml, an empty node (invalid) is checked by comparing with pugi::xml_node() or checking type.
    // However, the safest check for a valid node pointer is often checking if it's not null.
    // Since is_null() is not available in older versions or specific builds, we check if the node is not empty.
    // A node is empty if it is default constructed. We can check if the internal pointer is null or use type checks.
    // The standard way in pugixml without is_null() is to check if the node is not equal to a default constructed one,
    // but a simpler heuristic for fuzzing is to just call it if the parent is not the root of an empty doc, 
    // or simply rely on the function's internal null checks. 
    // However, to be safe and avoid undefined behavior if the node is truly invalid:
    if (!node6.empty() && !node5.empty()) {
        node6.remove_child(node5);
    }

    // 10. pugi::set_memory_management_functions
    // Re-set as per the sequence requirement
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}