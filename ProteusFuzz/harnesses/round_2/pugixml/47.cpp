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
#include <string>
#include <cstring>

static void safe_fuzz_driver(const uint8_t* data, size_t size) {
    if (size == 0) {
        return;
    }

    // Prepare environment: Create a null-terminated string from fuzz input
    // We ensure the string is null-terminated to satisfy load_string requirements
    std::string input_str(reinterpret_cast<const char*>(data), size);
    input_str.push_back('\0');

    pugi::xml_document doc;
    
    // 1. pugi::xml_document::load_string
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());
    if (result.status != pugi::status_ok) {
        // Parsing failed, but we can still try to traverse if some structure exists
        // or just return safely. We proceed to test traversal on potentially partial trees.
    }

    // Start traversal from root
    pugi::xml_node root = doc;

    // 2. pugi::xml_node::child
    // Use a safe name; since we don't know the content, try a common name or empty
    // The hint suggests using string_view_t, but the signature is const char_t*.
    // We will use a static string to avoid stack issues or empty string.
    const char* child_name = "test";
    pugi::xml_node node1 = root.child(child_name);

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
    (void)type1; // Suppress unused warning

    // 8. pugi::xml_node::value
    const pugi::char_t* val1 = node5.value();
    (void)val1; // Suppress unused warning

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

    // Cleanup is handled by RAII (destructors of xml_document and xml_node)
    // No explicit cleanup needed for pugi::xml_document or xml_node in this context
    // as they manage their own memory and xml_node is just a handle.
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    safe_fuzz_driver(Data, Size);
    return 0;
}