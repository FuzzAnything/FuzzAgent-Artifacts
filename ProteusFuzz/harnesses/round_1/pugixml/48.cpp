// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
    
    // Ensure null-termination for load_string
    // pugixml::char_t is typically char or wchar_t depending on PUGIXML_WCHAR_MODE.
    // We cast the input to pugi::char_t* and ensure null termination.
    pugi::char_t* buffer = new pugi::char_t[Size + 1];
    std::memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer);
    delete[] buffer;

    if (result.status == pugi::status_ok) {
        pugi::xml_node root = doc.child("root");
        if (root.empty()) {
            // If no root named "root", try to get any root or first child of document
            root = doc.first_child();
        }

        pugi::xml_node node1 = root.child("child");
        pugi::xml_node node2 = node1.first_child();
        pugi::xml_node node3 = node2.next_sibling();
        pugi::xml_node node4 = node3.next_sibling();
        pugi::xml_node node5 = node4.last_child();
        
        pugi::xml_node_type type1 = node5.type();
        const pugi::char_t* val1 = node5.value();
        
        pugi::xml_node_type type2 = node5.type();
        const pugi::char_t* val2 = node5.value();
        
        pugi::xml_node node6 = node5.last_child();
        pugi::xml_node node7 = node6.first_child();
        pugi::xml_node node8 = node7.first_child();
        
        // Suppress unused variable warnings if any
        (void)type1;
        (void)val1;
        (void)type2;
        (void)val2;
        (void)node8;
    }

    return 0;
}