// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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

// Custom memory allocation functions
static void* custom_allocate(size_t size) {
    if (size == 0) return nullptr;
    void* ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Step 1: Prepare environment and set custom memory management
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Create a document and parse the fuzzed input
    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for parsing
    std::string input(reinterpret_cast<const char*>(Data), Size);
    input.push_back('\0');

    pugi::xml_parse_result result = doc.load_string(input.c_str());
    (void)result;

    // Get the root node
    pugi::xml_node root = doc.first_child();
    
    // Check if root is null (empty document)
    if (root.empty()) {
        // If no root, create a dummy structure to test navigation
        root = doc.append_child("root");
    }

    // Ensure we have some children to traverse
    if (root.first_child().empty()) {
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }

    // Target API Sequence:
    // 1. pugi::xml_node::first_child
    pugi::xml_node node = root.first_child();
    if (node.empty()) return 0;

    // 2. pugi::xml_node::next_sibling
    pugi::xml_node sibling1 = node.next_sibling();
    (void)sibling1;

    // 3. pugi::xml_node::value
    const pugi::char_t* val = node.value();
    (void)val;

    // 4. pugi::xml_node::set_value
    // Use pugi::char_t for the string literal
    pugi::char_t new_value[] = "fuzzed_value";
    bool set_ok = node.set_value(new_value);
    (void)set_ok;

    // 5. pugi::xml_node::first_child (again)
    pugi::xml_node child_of_node = node.first_child();
    (void)child_of_node;

    // 6. pugi::xml_node::next_sibling (again)
    pugi::xml_node sibling2 = node.next_sibling();
    (void)sibling2;

    // 7. pugi::xml_node::next_sibling (again)
    pugi::xml_node sibling3 = node.next_sibling();
    (void)sibling3;

    // 8. pugi::xml_node::parent
    pugi::xml_node parent = node.parent();
    if (!parent.empty()) {
        // 9. pugi::xml_node::remove_child
        // Attempt to remove the node we are currently traversing
        bool removed = parent.remove_child(node);
        (void)removed;
    }

    // 10. pugi::set_memory_management_functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Cleanup is handled automatically by pugi::xml_document destructor
    return 0;
}