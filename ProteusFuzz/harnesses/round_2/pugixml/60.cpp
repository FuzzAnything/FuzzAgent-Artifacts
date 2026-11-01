// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
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

// Helper to create a document and a node for testing
static pugi::xml_document* create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Try to parse the input as XML
    if (size > 0 && data[0] != 0) {
        // Create a temporary buffer with null terminator
        char* buf = new char[size + 1];
        if (!buf) {
            delete doc;
            return nullptr;
        }
        std::memcpy(buf, data, size);
        buf[size] = '\0';
        
        pugi::xml_parse_result result = doc->load_string(buf);
        delete[] buf;
        
        if (!result) {
            // If parsing fails, create a minimal valid document
            doc->reset();
            if (!doc->append_child("root")) {
                delete doc;
                return nullptr;
            }
        }
    } else {
        // Empty or null input, create minimal document
        doc->reset();
        if (!doc->append_child("root")) {
            delete doc;
            return nullptr;
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a test document
    pugi::xml_document* doc = create_test_document(Data, Size);
    if (!doc) return 0;
    
    try {
        // Get a node to work with
        pugi::xml_node node = doc->root();
        if (!node) {
            delete doc;
            return 0;
        }
        
        // Prepare test string from input
        // We create a null-terminated string from the input data
        std::string input_str(reinterpret_cast<const char*>(Data), Size);
        // Ensure null termination for C-string operations
        input_str += '\0';
        const char* c_str = input_str.c_str();
        size_t str_len = input_str.length() - 1; // Exclude the added null
        
        // Test set_value overloads
        // Overload 1: set_value(const char_t* rhs, size_t size)
        node.set_value(c_str, str_len);
        
        // Overload 2: set_value(const char_t* rhs)
        node.set_value(c_str);
        
        // Test set_name overloads
        // Overload 1: set_name(const char_t* rhs, size_t size)
        node.set_name(c_str, str_len);
        
        // Overload 2: set_name(const char_t* rhs)
        node.set_name(c_str);
        
        // Test xml_text::set overloads
        pugi::xml_text text = node.text();
        
        // Overload 1: set(const char_t* rhs, size_t size)
        text.set(c_str, str_len);
        
        // Overload 2: set(const char_t* rhs)
        text.set(c_str);
        
        // Additional edge cases
        // Test with empty string
        node.set_value("", 0);
        node.set_value("");
        node.set_name("", 0);
        node.set_name("");
        text.set("", 0);
        text.set("");
        
        // Test with single character - FIXED: Ensure null termination
        if (Size >= 1) {
            // Create a properly null-terminated buffer for single char
            char single_char_buf[2];
            single_char_buf[0] = Data[0];
            single_char_buf[1] = '\0';
            
            node.set_value(single_char_buf, 1);
            node.set_value(single_char_buf);
            node.set_name(single_char_buf, 1);
            node.set_name(single_char_buf);
            text.set(single_char_buf, 1);
            text.set(single_char_buf);
        }
        
        // Test with larger chunks if available
        if (Size >= 10) {
            size_t test_size = Size > 100 ? 100 : Size;
            node.set_value(c_str, test_size);
            node.set_name(c_str, test_size);
            text.set(c_str, test_size);
        }
        
    } catch (...) {
        // Handle any exceptions gracefully
    }
    
    delete doc;
    return 0;
}