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

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;

    // pugixml uses char_t which is typically char unless PUGIXML_WCHAR_MODE is defined.
    // We use pugi::char_t to ensure compatibility.
    pugi::char_t* input_buffer = new pugi::char_t[Size + 1];
    if (!input_buffer) {
        return 0;
    }

    // Copy data and ensure null termination
    std::memcpy(input_buffer, Data, Size);
    input_buffer[Size] = '\0';

    // 1. pugi::xml_document::load_string
    pugi::xml_parse_result result = doc.load_string(input_buffer);
    delete[] input_buffer;

    // Even on parse error, we can try to traverse the document structure
    pugi::xml_node root = doc;

    // 2. pugi::xml_node::child
    pugi::xml_node child_node = root.child("root");

    // 3. pugi::xml_node::first_child
    pugi::xml_node first = child_node.first_child();

    // 4. pugi::xml_node::next_sibling
    pugi::xml_node next1 = first.next_sibling();

    // 5. pugi::xml_node::next_sibling
    pugi::xml_node next2 = next1.next_sibling();

    // 6. pugi::xml_node::last_child
    pugi::xml_node last = child_node.last_child();

    // 7. pugi::xml_node::type
    pugi::xml_node_type type1 = last.type();

    // 8. pugi::xml_node::value
    const pugi::char_t* val1 = last.value();
    (void)val1; // Suppress unused warning

    // 9. pugi::xml_node::type
    pugi::xml_node_type type2 = next2.type();

    // 10. pugi::xml_node::value
    const pugi::char_t* val2 = next2.value();
    (void)val2; // Suppress unused warning

    // 11. pugi::xml_node::last_child
    pugi::xml_node last2 = next2.last_child();

    // 12. pugi::xml_node::first_child
    pugi::xml_node first2 = last2.first_child();

    // 13. pugi::xml_node::first_child
    pugi::xml_node first3 = first2.first_child();

    // Cleanup is handled by RAII (pugi::xml_document destructor)
    // No explicit cleanup needed for xml_node as they are lightweight handles.

    return 0;
}