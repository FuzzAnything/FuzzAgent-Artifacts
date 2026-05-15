// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Create a document and parse XML from fuzzing input
    pugi::xml_document doc;
    
    // Ensure we have a null-terminated buffer for parsing
    if (Size == 0) {
        return 0;
    }
    
    // Copy data to a null-terminated buffer
    char* buffer = new char[Size + 1];
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';
    
    // Try to parse the XML
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare string parameters for function calls
    std::string_view sv_param;
    const char* cstr_param = "";
    
    if (Size > 0) {
        // Use first byte to determine string content
        uint8_t first_byte = Data[0];
        if (first_byte % 3 == 0) {
            sv_param = "test";
            cstr_param = "test";
        } else if (first_byte % 3 == 1) {
            sv_param = "node";
            cstr_param = "node";
        } else {
            sv_param = "";
            cstr_param = "";
        }
    }
    
    // Call functions in specified order
    // First two next_sibling calls (all 3 overloads each)
    for (int i = 0; i < 2; i++) {
        root.next_sibling(sv_param);
        root.next_sibling();
        root.next_sibling(cstr_param);
    }
    
    // Two previous_sibling calls (all 3 overloads each)
    for (int i = 0; i < 2; i++) {
        root.previous_sibling(cstr_param);
        root.previous_sibling();
        root.previous_sibling(sv_param);
    }
    
    // Six child calls (single overload)
    for (int i = 0; i < 6; i++) {
        root.child(cstr_param);
    }
    
    // Four next_sibling calls (all 3 overloads each)
    for (int i = 0; i < 4; i++) {
        root.next_sibling(sv_param);
        root.next_sibling();
        root.next_sibling(cstr_param);
    }
    
    // Four previous_sibling calls (all 3 overloads each)
    for (int i = 0; i < 4; i++) {
        root.previous_sibling(cstr_param);
        root.previous_sibling();
        root.previous_sibling(sv_param);
    }
    
    // Cleanup
    delete[] buffer;
    
    return 0;
}