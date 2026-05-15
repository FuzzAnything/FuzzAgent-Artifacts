// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
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
#include <fstream>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // Test pugi::xml_node::empty()
    {
        // Create empty node (null)
        pugi::xml_node empty_node;
        bool is_empty = empty_node.empty();
        
        // Create a document and get a non-empty node
        pugi::xml_document doc;
        pugi::xml_node node = doc.append_child("test_node");
        bool is_constructed_empty = node.empty();
    }
    
    // Test pugi::xml_text functions
    {
        // Create empty xml_text object
        pugi::xml_text empty_text;
        bool is_text_empty = empty_text.empty();
        
        // Create xml_document and add a node with text
        pugi::xml_document doc;
        pugi::xml_node node = doc.append_child("test_node");
        
        // Get text object from node
        pugi::xml_text text = node.text();
        
        // Test xml_text::empty() on both empty and non-empty text
        bool text_empty_before = text.empty();
        
        // Use fuzzer data to generate values for set() functions
        if (Size >= sizeof(double) + sizeof(unsigned int) + 1) {
            // Extract values from fuzzer input
            bool bool_val = Data[0] & 0x01;
            
            double double_val;
            std::memcpy(&double_val, Data + 1, sizeof(double));
            
            unsigned int uint_val;
            std::memcpy(&uint_val, Data + 1 + sizeof(double), sizeof(unsigned int));
            
            // Test all three overloads of xml_text::set()
            bool set_bool_result = text.set(bool_val);
            bool set_double_result = text.set(double_val);
            bool set_uint_result = text.set(uint_val);
            
            // Test xml_text::as_bool()
            bool as_bool_default = text.as_bool();
            bool as_bool_true = text.as_bool(true);
            bool as_bool_false = text.as_bool(false);
            
            // Test xml_text::empty() after setting values
            bool text_empty_after = text.empty();
        }
        
        // Test with string_view parameter if available in the API
        if (Size > 0) {
            std::string_view str_view(reinterpret_cast<const char*>(Data), 
                                      Size > 100 ? 100 : Size);
            // Create another text object for string_view test
            pugi::xml_node another_node = doc.append_child("another_node");
            pugi::xml_text another_text = another_node.text();
        }
    }
    
    // Test with file operations if needed
    if (Size > 0) {
        std::ofstream out_file("./dummy_file", std::ios::binary);
        if (out_file) {
            out_file.write(reinterpret_cast<const char*>(Data), 
                          Size > 1024 ? 1024 : Size);
            out_file.close();
            
            // Load XML from file and test functions
            pugi::xml_document doc_from_file;
            pugi::xml_parse_result result = doc_from_file.load_file("./dummy_file");
            
            if (result) {
                pugi::xml_node root = doc_from_file.root();
                bool root_empty = root.empty();
                
                // Test text functions on first child
                for (pugi::xml_node child : root.children()) {
                    pugi::xml_text child_text = child.text();
                    bool child_text_empty = child_text.empty();
                    bool child_text_as_bool = child_text.as_bool();
                    
                    // Try setting some values
                    child_text.set(true);
                    child_text.set(3.14);
                    child_text.set(42u);
                    
                    break; // Just test first child
                }
            }
        }
    }
    
    return 0;
}