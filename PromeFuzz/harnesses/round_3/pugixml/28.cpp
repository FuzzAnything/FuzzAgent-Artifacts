// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Create null-terminated string for XML content
    std::string xml_content;
    if (Size > 0) {
        xml_content.assign(reinterpret_cast<const char*>(Data), Size);
    }
    
    // Ensure null termination
    xml_content.push_back('\0');
    
    // Create XML document and parse the string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    // If parsing failed, we can still continue with empty document
    // Get the document element (root node)
    pugi::xml_node root = doc.document_element();
    
    // Call child() 8 times in sequence
    pugi::xml_node current = root;
    
    // Use some common XML element names for child lookups
    const char* child_names[] = {"node", "child", "item", "element", "tag", "root", "parent", "data"};
    
    for (int i = 0; i < 8; ++i) {
        if (current) {
            current = current.child(child_names[i % 8]);
        } else {
            // If current is empty, break out of the loop
            break;
        }
    }
    
    // Call text() on the current node
    if (current) {
        pugi::xml_text text = current.text();
        // We can optionally do something with the text
        (void)text;
    }
    
    // Call child() again
    pugi::xml_node next_child;
    if (current) {
        next_child = current.child("value");
    }
    
    // Call child_value() overload without name
    if (next_child) {
        const char* value1 = next_child.child_value();
        (void)value1;
    }
    
    // Call child_value() overload with name
    if (next_child) {
        const char* value2 = next_child.child_value("content");
        (void)value2;
    }
    
    // Also try child_value() on the current node
    if (current) {
        const char* value3 = current.child_value();
        (void)value3;
        
        const char* value4 = current.child_value("text");
        (void)value4;
    }
    
    return 0;
}