// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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
    if (size == 0) return nullptr;
    return std::malloc(size);
}

static void custom_deallocate(void* ptr) {
    if (ptr) {
        std::free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Step 0: Setup memory management functions as per the last item in the sequence
    // Although the task lists it last, it must be called before parsing to affect the document.
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Prepare the environment: Create an XML document from the fuzz input
    pugi::xml_document doc;
    
    // Convert input to string for parsing. Ensure null termination.
    char* buffer = static_cast<char*>(std::malloc(Size + 1));
    if (!buffer) return 0;
    
    std::memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    // Parse the document
    pugi::xml_parse_result result = doc.load_string(buffer);
    std::free(buffer);

    // If parsing failed, we still proceed to test the API on the potentially partial structure
    // or an empty document, which is valid for fuzzing.

    pugi::xml_node root = doc.first_child();

    // 1. pugi::xml_node::first_child
    pugi::xml_node node1 = root.first_child();

    // 2. pugi::xml_node::next_sibling
    pugi::xml_node node2 = node1.next_sibling();

    // 3. pugi::xml_node::value
    const pugi::char_t* val = node2.value();
    (void)val; // Suppress unused variable warning

    // 4. pugi::xml_node::set_value
    // We try to set a value. This might fail if node2 is an element, which is expected.
    node2.set_value("fuzzed_text_content");

    // 5. pugi::xml_node::first_child
    pugi::xml_node node3 = node2.first_child();

    // 6. pugi::xml_node::next_sibling
    pugi::xml_node node4 = node3.next_sibling();

    // 7. pugi::xml_node::next_sibling
    pugi::xml_node node5 = node4.next_sibling();

    // 8. pugi::xml_node::parent
    pugi::xml_node parent = node5.parent();

    // 9. pugi::xml_node::remove_child
    // We attempt to remove a child. We try to remove 'node3' from 'parent' if valid.
    // In pugixml, an empty node has an internal null pointer, so we check if the node is valid.
    // The correct way to check validity in pugixml is to check if the internal pointer is null.
    // However, xml_node does not expose a direct is_null() method in older versions or specific builds.
    // Instead, we can check if the node is empty by comparing to a default constructed node or checking internal state if accessible.
    // The standard idiom is to check if the node is valid by ensuring it's not null. 
    // Since xml_node wraps a pointer, we can't directly check it without internal access.
    // But we can rely on the fact that operations on null nodes are safe in pugixml.
    // To be explicit and safe, we can check if parent is not null by checking if it's not a default constructed node.
    // Actually, pugixml nodes can be checked for validity by checking if they are not null.
    // The standard way is: if (parent) { ... }
    // But since we need to be explicit about the logic requested:
    // We will attempt the removal. pugixml handles null nodes gracefully.
    // To satisfy the logic of "if valid", we can check if the node is not null.
    // In pugixml, xml_node has an implicit conversion to bool or we can use is_null() if available.
    // The error indicated is_null() is missing. The correct check in standard pugixml is:
    // if (parent && node3) { ... }
    // Or checking internal pointer if we had access, but we don't.
    // Let's use the boolean conversion which checks for null.
    if (parent && node3) {
        parent.remove_child(node3);
    }

    // 10. pugi::set_memory_management_functions
    // Call again to ensure the API is invoked as requested in the sequence.
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    return 0;
}