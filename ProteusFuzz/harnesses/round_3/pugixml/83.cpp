// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
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
#include <cstdlib>
#include <cstddef>
#include <string_view>

static void fuzz_xml_node_offset_debug(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a null-terminated string to ensure safe parsing
    std::string str(reinterpret_cast<const char*>(data), size);
    // Ensure null termination
    if (size > 0 && str[size - 1] != '\0') {
        str.push_back('\0');
    }
    
    // Try parsing the input data
    if (!doc.load_string(str.c_str())) {
        return;
    }
    
    // Iterate through all nodes and call offset_debug
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        ptrdiff_t offset = node.offset_debug();
        (void)offset; // Suppress unused variable warning
    }
}

static void fuzz_xml_node_set_value(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Prepare a null-terminated string from input data
    std::string str(reinterpret_cast<const char*>(data), size);
    if (size > 0 && str[size - 1] != '\0') {
        str.push_back('\0');
    }
    
    // Call set_value with null-terminated string
    node.set_value(str.c_str());
    
    // Call set_value with size parameter (use original size before null termination if needed)
    node.set_value(str.c_str(), str.size());
}

static void fuzz_xml_text_set(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Prepare a null-terminated string from input data
    std::string str(reinterpret_cast<const char*>(data), size);
    if (size > 0 && str[size - 1] != '\0') {
        str.push_back('\0');
    }
    
    // Call set with null-terminated string
    text.set(str.c_str());
}

static void fuzz_xml_node_set_name(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Prepare a null-terminated string from input data
    std::string str(reinterpret_cast<const char*>(data), size);
    if (size > 0 && str[size - 1] != '\0') {
        str.push_back('\0');
    }
    
    // Call set_name with null-terminated string
    node.set_name(str.c_str());
    
    // Call set_name with size parameter
    node.set_name(str.c_str(), str.size());
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    
    // Fuzz offset_debug
    fuzz_xml_node_offset_debug(doc, Data, Size);
    
    // Fuzz set_value on document root
    fuzz_xml_node_set_value(doc, Data, Size);
    
    // Fuzz set_name on document root
    fuzz_xml_node_set_name(doc, Data, Size);
    
    // Try to get text node and fuzz set
    // Use text() method on the document node itself
    pugi::xml_text text = doc.text();
    if (text) {
        fuzz_xml_text_set(text, Data, Size);
    }
    
    // Create a new node and fuzz its methods
    pugi::xml_node new_node = doc.append_child("test");
    fuzz_xml_node_set_value(new_node, Data, Size);
    fuzz_xml_node_set_name(new_node, Data, Size);
    
    // Get text node of new_node and fuzz set
    pugi::xml_text new_text = new_node.text();
    if (new_text) {
        fuzz_xml_text_set(new_text, Data, Size);
    }
    
    return 0;
}