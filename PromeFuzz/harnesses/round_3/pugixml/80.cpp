// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for potential file-based operations
    writeDummyFile(Data, Size);

    try {
        // Create XML document and parse from buffer
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size);
        
        // Get root node (or empty node if parsing failed)
        pugi::xml_node root = doc.root();
        
        // Test 1: parent()
        {
            pugi::xml_node test_node = root;
            for (int i = 0; i < 3 && test_node; ++i) {
                test_node = test_node.parent();
            }
        }
        
        // Test 2: prepend_child()
        {
            if (root) {
                // Create a string with proper lifetime for the string_view
                std::string name_str;
                if (Size > 0) {
                    size_t name_len = Size < 100 ? Size : 100;
                    name_str.assign(reinterpret_cast<const char*>(Data), name_len);
                }
                std::string_view name_view = name_str;
                
                pugi::xml_node new_child = root.prepend_child(name_view);
                (void)new_child;
            }
        }
        
        // Test 3: append_child()
        {
            if (root) {
                // Create a string with proper lifetime for the string_view
                std::string name_str;
                if (Size > 1) {
                    size_t offset = Size / 2;
                    size_t name_len = Size - offset;
                    if (name_len > 100) name_len = 100;
                    name_str.assign(reinterpret_cast<const char*>(Data + offset), name_len);
                }
                std::string_view name_view = name_str;
                
                pugi::xml_node new_child = root.append_child(name_view);
                (void)new_child;
            }
        }
        
        // Test 4: last_child()
        {
            pugi::xml_node last = root.last_child();
            (void)last;
            
            // Also test on a child node if exists
            if (root.first_child()) {
                pugi::xml_node child_last = root.first_child().last_child();
                (void)child_last;
            }
        }
        
        // Test 5: child()
        {
            if (root) {
                // Create a string with proper lifetime for the string_view
                std::string name_str;
                if (Size > 0) {
                    size_t name_len = Size < 50 ? Size : 50;
                    name_str.assign(reinterpret_cast<const char*>(Data), name_len);
                }
                std::string_view name_view = name_str;
                
                pugi::xml_node found = root.child(name_view);
                (void)found;
                
                // Also test with empty name
                pugi::xml_node empty_found = root.child("");
                (void)empty_found;
            }
        }
        
        // Test 6: insert_move_after()
        {
            if (root) {
                // Create some child nodes to work with
                pugi::xml_node child1 = root.append_child("child1");
                pugi::xml_node child2 = root.append_child("child2");
                
                if (child1 && child2) {
                    // Try moving child2 after child1
                    pugi::xml_node result = root.insert_move_after(child2, child1);
                    (void)result;
                    
                    // Try moving to empty node
                    pugi::xml_node empty_node;
                    pugi::xml_node result2 = root.insert_move_after(child1, empty_node);
                    (void)result2;
                }
            }
        }
        
        // Additional tests with empty nodes
        {
            pugi::xml_node empty_node;
            
            // Test all functions on empty node
            pugi::xml_node parent = empty_node.parent();
            (void)parent;
            
            pugi::xml_node last = empty_node.last_child();
            (void)last;
            
            std::string_view empty_name = "";
            pugi::xml_node child = empty_node.child(empty_name);
            (void)child;
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crash
    }
    
    return 0;
}