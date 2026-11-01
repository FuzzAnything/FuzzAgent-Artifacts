// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
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
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xml_document doc;
    
    // Convert input data to a string for parsing, ensuring null termination
    std::string input(reinterpret_cast<const char*>(Data), Size);
    
    // Parse the XML document
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    if (!result) {
        // Even if parsing fails, we can still test some API behaviors on the empty document
        // but for meaningful tests, we try to proceed with what we have
    }

    // Get root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root, try to create one or use document itself if it has children
        root = doc;
    }

    // 1. pugi::xml_node::child
    // Use a name derived from input or a default if input is too short
    std::string child_name = (Size >= 4) ? std::string(reinterpret_cast<const char*>(Data), 4) : "test";
    // Ensure null termination for child_name
    child_name.resize(4); // Limit to 4 chars to avoid issues, though child takes char_t*
    pugi::xml_node child_node = root.child(child_name.c_str());

    // 2. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 3. pugi::xml_text::get
    const pugi::char_t* text_content = text_proxy.get();
    (void)text_content; // Suppress unused variable warning

    // 4. pugi::xml_text::set (first time)
    // Create a string_view from input data for setting text
    std::string_view set_data_view(reinterpret_cast<const char*>(Data), Size);
    // Ensure we don't set a string with embedded nulls if the library expects C-strings, 
    // but pugixml's set(string_view_t) handles string_view correctly.
    // We'll use a safe substring to avoid potential issues if input is huge or malformed
    size_t safe_size = (Size > 100) ? 100 : Size;
    std::string_view safe_view(reinterpret_cast<const char*>(Data), safe_size);
    text_proxy.set(safe_view);

    // 5. pugi::xml_text::data
    pugi::xml_node data_node = text_proxy.data();
    (void)data_node; // Suppress unused variable warning

    // 6. pugi::xml_text::set (second time)
    // Use a different part of the input or a fixed string
    std::string_view set_data_view2 = "modified";
    text_proxy.set(set_data_view2);

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child = root.last_child();
    (void)last_child; // Suppress unused variable warning

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}