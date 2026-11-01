// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string_view>

static void* custom_allocate(size_t size) {
    return std::malloc(size);
}

static void custom_deallocate(void* ptr) {
    std::free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: set custom memory management functions
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Create a document
    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for load_string
    // We create a temporary buffer to hold the data plus a null terminator
    std::vector<char> buffer(Size + 1);
    if (Size > 0) {
        std::memcpy(buffer.data(), Data, Size);
        buffer[Size] = '\0';
    }

    pugi::xml_parse_result result = doc.load_string(buffer.data());

    // If parsing failed, try to construct a minimal valid tree to test traversal logic
    if (!result) {
        // Create a minimal structure manually if parsing fails completely
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }

    pugi::xml_node node = doc.first_child();
    
    // 1. pugi::xml_node::first_child
    pugi::xml_node first = node.first_child();
    
    // 2. pugi::xml_node::next_sibling
    pugi::xml_node next1 = first.next_sibling();
    
    // 3. pugi::xml_node::value
    const pugi::char_t* val = first.value();
    (void)val; // Use value to suppress unused warning
    
    // 4. pugi::xml_node::set_value
    // We need a valid node type (e.g., text, comment) to set value, but elements return false.
    // We attempt to set value on 'first' regardless; it might fail if it's an element, which is expected behavior.
    // We use a safe string derived from input or a literal.
    const char* safe_val = "fuzzed_value";
    first.set_value(safe_val);
    
    // 5. pugi::xml_node::first_child (again)
    pugi::xml_node first_again = node.first_child();
    
    // 6. pugi::xml_node::next_sibling (again)
    pugi::xml_node next2 = first_again.next_sibling();
    
    // 7. pugi::xml_node::next_sibling (again)
    pugi::xml_node next3 = next2.next_sibling();
    
    // 8. pugi::xml_node::parent
    pugi::xml_node parent_node = first_again.parent();
    
    // 9. pugi::xml_node::remove_child
    // Attempt to remove a child from the parent_node. 
    // We try to remove 'next3' if it exists and is a child of parent_node.
    // In pugixml, a node is valid if it evaluates to true in a boolean context.
    
    if (parent_node && next3) {
        parent_node.remove_child(next3);
    } else if (parent_node && next2) {
        parent_node.remove_child(next2);
    } else if (parent_node && next1) {
        parent_node.remove_child(next1);
    }

    // 10. pugi::set_memory_management_functions (again, as per sequence)
    // Re-setting to same functions or different ones is valid.
    pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}