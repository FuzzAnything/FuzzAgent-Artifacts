// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for load_string
    // We create a buffer with a null terminator to prevent out-of-bounds reads
    // if the fuzzer provides data without a terminator.
    // However, load_string expects a zero-terminated string. 
    // We will pass the data directly but ensure we don't read past Size if it's not null-terminated.
    // A safer approach for fuzzing is to ensure null termination.
    // Since we cannot modify the input buffer, we check for null terminator or create a safe copy if needed.
    // For simplicity and performance in fuzzing, we assume the fuzzer might provide non-null-terminated data.
    // We will use a temporary buffer to ensure safety.
    
    // Create a safe copy with null termination
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';

    // 1. pugi::xml_document::load_string
    pugi::xml_parse_result result = doc.load_string(safe_buffer.data());
    // We don't strictly need to check result for fuzzing, but it's good practice.
    // If parsing fails, doc is in a valid but empty state, which is safe to traverse.

    pugi::xml_node root = doc;

    // 2. pugi::xml_node::child
    // We use a static string for the child name to avoid issues with fuzzer data as a name
    // unless we want to fuzz the name too. The prompt says "invoke ... child", implying we call it.
    // To explore more states, we can try different names, but a fixed one is safer for stability.
    // Let's use a simple name.
    pugi::xml_node node1 = root.child("child");

    // 3. pugi::xml_node::first_child
    pugi::xml_node node2 = node1.first_child();

    // 4. pugi::xml_node::next_sibling
    pugi::xml_node node3 = node2.next_sibling();

    // 5. pugi::xml_node::next_sibling
    pugi::xml_node node4 = node3.next_sibling();

    // 6. pugi::xml_node::last_child
    pugi::xml_node node5 = node4.last_child();

    // 7. pugi::xml_node::type
    pugi::xml_node_type type1 = node5.type();
    (void)type1; // Suppress unused variable warning

    // 8. pugi::xml_node::value
    const pugi::char_t* val1 = node5.value();
    (void)val1;

    // 9. pugi::xml_node::type
    pugi::xml_node_type type2 = node5.type();
    (void)type2;

    // 10. pugi::xml_node::value
    const pugi::char_t* val2 = node5.value();
    (void)val2;

    // 11. pugi::xml_node::last_child
    pugi::xml_node node6 = node5.last_child();

    // 12. pugi::xml_node::first_child
    pugi::xml_node node7 = node6.first_child();

    // 13. pugi::xml_node::first_child
    pugi::xml_node node8 = node7.first_child();

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}