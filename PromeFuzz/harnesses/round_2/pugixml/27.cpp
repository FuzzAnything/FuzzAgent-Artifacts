// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
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
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated XML string from fuzzer input
    std::vector<char> xml_buffer(Data, Data + Size);
    xml_buffer.push_back('\0');

    // Create document and parse XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_buffer.data());
    
    // If parsing failed, we can still continue with empty document
    // but child operations will just return empty nodes

    // Create null-terminated node name strings for child calls
    // We'll use simple single-character names derived from the input
    std::vector<std::string> node_names;
    for (int i = 0; i < 9; ++i) {  // Need 9 names for 9 child calls
        char name[2] = {static_cast<char>('a' + (i % 26)), '\0'};
        node_names.push_back(name);
    }

    // Execute the sequence of API calls
    try {
        // First 8 child() calls
        pugi::xml_node current_node = doc.child(node_names[0].c_str());
        for (int i = 1; i < 8; ++i) {
            current_node = current_node.child(node_names[i].c_str());
        }

        // text() call
        pugi::xml_text text_obj = current_node.text();
        
        // Additional child() call
        current_node = current_node.child(node_names[8].c_str());
        
        // First child_value() overload (no arguments)
        const char* child_val1 = current_node.child_value();
        
        // Second child_value() overload (with name argument)
        const char* child_val2 = current_node.child_value("test");

        // Use the results to avoid optimization
        if (child_val1 != nullptr && child_val2 != nullptr) {
            volatile char dummy1 = *child_val1;
            volatile char dummy2 = *child_val2;
            (void)dummy1;
            (void)dummy2;
        }
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }

    return 0;
}