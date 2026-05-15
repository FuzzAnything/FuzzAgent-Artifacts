// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Ensure null-terminated string for load_string
    if (Size == 0) {
        return 0;
    }
    
    // Create buffer with null terminator
    char* buffer = new char[Size + 1];
    if (!buffer) {
        return 0;
    }
    
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';
    
    // Create XML document
    pugi::xml_document doc;
    
    // 1. Call load_string
    pugi::xml_parse_result result = doc.load_string(buffer);
    
    // Clean up buffer
    delete[] buffer;
    
    // If parsing failed, we can still proceed with empty document
    // to test other functions on empty/invalid nodes
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    
    // Prepare some names for child lookups
    // We'll use a few fixed names and also try to extract names from the document
    const char* names[] = {"root", "child", "node", "element", "item", "data", "value", "text", "name", "attr"};
    size_t name_count = sizeof(names) / sizeof(names[0]);
    
    // 2-9. Call child() 8 times
    pugi::xml_node current = root;
    for (int i = 0; i < 8; i++) {
        if (current) {
            // Try to get a child with a name from our list
            const char* name_to_use = names[i % name_count];
            current = current.child(name_to_use);
            
            // Also try with empty string
            if (!current) {
                current = current.child("");
            }
        } else {
            // If current is empty, child() will return empty node
            current = current.child("");
        }
    }
    
    // 10. Call text()
    if (current) {
        pugi::xml_text txt = current.text();
        // Can optionally use the text object
        (void)txt;
    }
    
    // 11. Call child() again
    pugi::xml_node next_node = current;
    if (current) {
        // Try a different name
        const char* name_to_use = names[2 % name_count];
        next_node = current.child(name_to_use);
    } else {
        next_node = current.child("");
    }
    
    // 12. Call child_value() without parameters
    if (next_node) {
        const char* value1 = next_node.child_value();
        (void)value1;
    }
    
    // 13. Call child_value() with name parameter
    if (next_node) {
        const char* name_to_use = names[3 % name_count];
        const char* value2 = next_node.child_value(name_to_use);
        (void)value2;
        
        // Also try with empty string
        const char* value3 = next_node.child_value("");
        (void)value3;
    }
    
    // Try to explore more states by calling functions on different nodes
    // Try to traverse the document tree
    if (root) {
        // Try to get children with various names
        for (size_t i = 0; i < name_count && i < 5; i++) {
            pugi::xml_node child = root.child(names[i]);
            if (child) {
                // Call child_value on this child
                const char* cv1 = child.child_value();
                (void)cv1;
                
                // Call child_value with name
                const char* cv2 = child.child_value(names[(i + 1) % name_count]);
                (void)cv2;
                
                // Call text()
                pugi::xml_text child_text = child.text();
                (void)child_text;
                
                // Try to go deeper
                pugi::xml_node grandchild = child.child(names[(i + 2) % name_count]);
                if (grandchild) {
                    const char* gcv = grandchild.child_value();
                    (void)gcv;
                }
            }
        }
    }
    
    return 0;
}