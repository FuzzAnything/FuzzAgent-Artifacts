// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null-termination for load_string
    std::string input(reinterpret_cast<const char*>(Data), Size);
    if (input.back() != '\0') {
        input += '\0';
    }

    pugi::xml_document doc;
    doc.load_string(input.c_str());

    // Start traversal from root
    pugi::xml_node root = doc.child("");

    // Sequence: child -> first_child -> next_sibling -> next_sibling -> last_child -> type -> value -> type -> value -> last_child -> first_child -> first_child
    pugi::xml_node node1 = root.child("");
    pugi::xml_node node2 = node1.first_child();
    pugi::xml_node node3 = node2.next_sibling();
    pugi::xml_node node4 = node3.next_sibling();
    pugi::xml_node node5 = node4.last_child();
    
    // Check type and value
    pugi::xml_node_type type1 = node5.type();
    const pugi::char_t* value1 = node5.value();
    (void)type1;
    (void)value1;

    pugi::xml_node_type type2 = node5.type();
    const pugi::char_t* value2 = node5.value();
    (void)type2;
    (void)value2;

    pugi::xml_node node6 = node5.last_child();
    pugi::xml_node node7 = node6.first_child();
    pugi::xml_node node8 = node7.first_child();

    (void)node8;

    return 0;
}