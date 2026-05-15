// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    // Create empty objects for testing
    pugi::xml_text empty_text;
    pugi::xml_node empty_node;
    
    // Test empty() functions on empty objects
    bool text_empty = empty_text.empty();
    bool node_empty = empty_node.empty();
    
    // Create a document and parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // Test xml_node::empty() on non-empty node
        bool root_empty = root.empty();
        
        // Get text from the root node
        pugi::xml_text root_text = root.text();
        
        // Test xml_text::empty() on potentially non-empty text
        bool root_text_empty = root_text.empty();
        
        // Test xml_text::get() on the text
        const pugi::char_t* text_content = root_text.get();
        
        // Test xml_text::as_bool() with default and explicit values
        bool as_bool_default = root_text.as_bool();
        bool as_bool_true = root_text.as_bool(true);
        bool as_bool_false = root_text.as_bool(false);
        
        // Test xml_text::set() overloads
        // Create a string_view from fuzzer data (wrapping around if needed)
        size_t str_size = Size > 0 ? Size : 1;
        std::string_view sv(reinterpret_cast<const char*>(Data), str_size);
        
        // Convert first byte to bool for testing
        bool bool_val = Size > 0 ? (Data[0] & 1) : false;
        
        // Try to set text content
        bool set_bool_result = root_text.set(bool_val);
        bool set_string_result = root_text.set(sv);
        
        // Create a child node and test on it
        pugi::xml_node child = root.append_child("test");
        pugi::xml_text child_text = child.text();
        
        // Test all functions on the child text
        bool child_text_empty = child_text.empty();
        const pugi::char_t* child_text_content = child_text.get();
        bool child_as_bool = child_text.as_bool();
        
        // Test set on child text
        bool child_set_bool = child_text.set(!bool_val);
        bool child_set_string = child_text.set(sv);
    }
    
    // Test on document itself (which is a node)
    bool doc_empty = doc.empty();
    
    // Test on a text object created from a node that doesn't exist
    pugi::xml_node non_existent = pugi::xml_node();
    pugi::xml_text non_existent_text = non_existent.text();
    
    bool non_existent_text_empty = non_existent_text.empty();
    const pugi::char_t* non_existent_text_content = non_existent_text.get();
    bool non_existent_as_bool = non_existent_text.as_bool();
    
    // Try to set on non-existent text (should fail)
    bool non_existent_set_bool = non_existent_text.set(true);
    bool non_existent_set_string = non_existent_text.set(std::string_view("test"));
    
    // Create a new document and test setting text on various nodes
    pugi::xml_document doc2;
    pugi::xml_node node1 = doc2.append_child("node1");
    pugi::xml_node node2 = doc2.append_child("node2");
    
    pugi::xml_text text1 = node1.text();
    pugi::xml_text text2 = node2.text();
    
    // Test setting different values
    text1.set(true);
    text2.set(false);
    
    // Test get after setting
    const pugi::char_t* text1_content = text1.get();
    const pugi::char_t* text2_content = text2.get();
    
    // Test as_bool after setting
    bool text1_as_bool = text1.as_bool();
    bool text2_as_bool = text2.as_bool();
    
    // Test empty after setting
    bool text1_empty_after = text1.empty();
    bool text2_empty_after = text2.empty();
    
    // Test setting with string_view using fuzzer data
    if (Size > 0) {
        // Use different parts of the input for different string views
        size_t half_size = Size / 2;
        std::string_view sv1(reinterpret_cast<const char*>(Data), half_size);
        std::string_view sv2(reinterpret_cast<const char*>(Data + half_size), Size - half_size);
        
        text1.set(sv1);
        text2.set(sv2);
        
        // Test get after string set
        const pugi::char_t* text1_str_content = text1.get();
        const pugi::char_t* text2_str_content = text2.get();
        
        // Test as_bool after string set
        bool text1_str_as_bool = text1.as_bool();
        bool text2_str_as_bool = text2.as_bool();
    }
    
    return 0;
}