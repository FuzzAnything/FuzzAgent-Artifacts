// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Test with default-constructed empty objects
    {
        pugi::xml_text empty_text;
        pugi::xml_node empty_node;
        
        // Test empty() methods on empty objects
        bool text_empty = empty_text.empty();
        bool node_empty = empty_node.empty();
        
        // Test as_bool() with default and explicit parameter
        bool as_bool_default = empty_text.as_bool();
        bool as_bool_explicit = empty_text.as_bool(true);
        
        // Test get() on empty text
        const pugi::char_t* text_get = empty_text.get();
        
        // Test set() methods on empty text (should return false)
        bool set_bool_result = empty_text.set(true);
        
        // Create string_view from fuzzer data (handle empty case)
        std::string_view sv(reinterpret_cast<const char*>(Data), Size);
        bool set_string_result = empty_text.set(sv);
    }
    
    // Test with actual XML document
    {
        pugi::xml_document doc;
        
        // Create a node with some content
        pugi::xml_node node = doc.append_child("test_node");
        pugi::xml_text text = node.text();
        
        // Test empty() methods on non-empty objects
        bool text_empty = text.empty();
        bool node_empty = node.empty();
        
        // Test get() on text (initially empty)
        const pugi::char_t* text_get = text.get();
        
        // Test as_bool() with various scenarios
        bool as_bool_default = text.as_bool();
        bool as_bool_explicit = text.as_bool(true);
        
        // Test set() methods with fuzzer data
        
        // Use first byte for boolean value
        bool bool_val = false;
        if (Size > 0) {
            bool_val = (Data[0] & 1) != 0;
        }
        bool set_bool_result = text.set(bool_val);
        
        // Test as_bool() after setting boolean value
        bool as_bool_after_set = text.as_bool();
        
        // Use fuzzer data for string value
        std::string_view sv(reinterpret_cast<const char*>(Data), Size);
        bool set_string_result = text.set(sv);
        
        // Test get() after setting string
        const pugi::char_t* text_get_after_set = text.get();
        
        // Test as_bool() after setting string
        bool as_bool_after_string = text.as_bool();
        bool as_bool_after_string_default = text.as_bool(false);
        
        // Create another text object from different node
        pugi::xml_node node2 = doc.append_child("another_node");
        pugi::xml_text text2 = node2.append_child(pugi::node_pcdata).text();
        
        // Test set with different values
        if (Size > 1) {
            bool bool_val2 = (Data[1] & 1) != 0;
            text2.set(bool_val2);
            
            // Use remaining data for string (skip first 2 bytes if available)
            size_t offset = Size > 2 ? 2 : 0;
            std::string_view sv2(reinterpret_cast<const char*>(Data + offset), Size - offset);
            text2.set(sv2);
        }
        
        // Test empty() on the second text
        bool text2_empty = text2.empty();
        
        // Test get() on second text
        const pugi::char_t* text2_get = text2.get();
        
        // Test as_bool() on second text
        bool text2_as_bool = text2.as_bool();
    }
    
    // Additional test with document loaded from string
    if (Size > 0) {
        pugi::xml_document doc;
        
        // Try to parse the fuzzer data as XML
        pugi::xml_parse_result result = doc.load_buffer(Data, Size);
        
        if (result) {
            // Traverse all nodes and test the functions
            pugi::xml_node root = doc.document_element();
            if (root) {
                for (pugi::xml_node node = root; node; node = node.next_sibling()) {
                    // Test node empty()
                    bool node_empty = node.empty();
                    
                    pugi::xml_text text = node.text();
                    
                    // Test text empty()
                    bool text_empty = text.empty();
                    
                    // Test get()
                    const pugi::char_t* text_get = text.get();
                    
                    // Test as_bool()
                    bool as_bool = text.as_bool();
                    bool as_bool_with_default = text.as_bool(true);
                    
                    // Test set() with fuzzer data (use first byte for bool)
                    if (Size > 0) {
                        bool bool_val = (Data[0] & 1) != 0;
                        text.set(bool_val);
                        
                        // Use the data as string
                        std::string_view sv(reinterpret_cast<const char*>(Data), Size);
                        text.set(sv);
                    }
                }
            }
        }
    }
    
    return 0;
}