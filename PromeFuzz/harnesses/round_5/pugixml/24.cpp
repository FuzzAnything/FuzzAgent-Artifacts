// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document to work with
    pugi::xml_document doc;
    
    // Try to parse the input as XML, but if it fails, create a minimal document
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, create a simple document to ensure we have nodes to work with
    if (!result) {
        doc.append_child("root");
    }
    
    // Get the root node (or create one if none exists)
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Ensure we have at least one child node for the child() call
    pugi::xml_node child_node = root.append_child("child");
    
    // Prepare string data for function calls
    // We'll use parts of the input data for strings, ensuring null termination
    std::string child_name;
    std::string attr_name1, attr_name2, attr_name3, attr_name4, attr_name5;
    std::string string_value1, string_value2, string_value3;
    
    // Extract strings from input data, ensuring we don't go out of bounds
    if (Size > 0) {
        // For child name - use first portion of data
        size_t child_name_len = Size > 100 ? 100 : Size;
        child_name = std::string(reinterpret_cast<const char*>(Data), child_name_len);
        
        // For attribute names - use different portions of the data
        size_t offset = 0;
        size_t chunk_size = Size / 10;
        
        if (chunk_size > 0) {
            // Attribute name 1
            size_t start1 = offset % Size;
            size_t len1 = chunk_size > 50 ? 50 : chunk_size;
            attr_name1 = std::string(reinterpret_cast<const char*>(Data + start1), 
                                    len1 > Size - start1 ? Size - start1 : len1);
            offset += chunk_size;
            
            // Attribute name 2
            size_t start2 = offset % Size;
            size_t len2 = chunk_size > 50 ? 50 : chunk_size;
            attr_name2 = std::string(reinterpret_cast<const char*>(Data + start2), 
                                    len2 > Size - start2 ? Size - start2 : len2);
            offset += chunk_size;
            
            // Attribute name 3
            size_t start3 = offset % Size;
            size_t len3 = chunk_size > 50 ? 50 : chunk_size;
            attr_name3 = std::string(reinterpret_cast<const char*>(Data + start3), 
                                    len3 > Size - start3 ? Size - start3 : len3);
            offset += chunk_size;
            
            // Attribute name 4
            size_t start4 = offset % Size;
            size_t len4 = chunk_size > 50 ? 50 : chunk_size;
            attr_name4 = std::string(reinterpret_cast<const char*>(Data + start4), 
                                    len4 > Size - start4 ? Size - start4 : len4);
            offset += chunk_size;
            
            // Attribute name 5
            size_t start5 = offset % Size;
            size_t len5 = chunk_size > 50 ? 50 : chunk_size;
            attr_name5 = std::string(reinterpret_cast<const char*>(Data + start5), 
                                    len5 > Size - start5 ? Size - start5 : len5);
            offset += chunk_size;
            
            // String values
            size_t start_val1 = offset % Size;
            size_t len_val1 = chunk_size > 100 ? 100 : chunk_size;
            string_value1 = std::string(reinterpret_cast<const char*>(Data + start_val1), 
                                       len_val1 > Size - start_val1 ? Size - start_val1 : len_val1);
            offset += chunk_size;
            
            size_t start_val2 = offset % Size;
            size_t len_val2 = chunk_size > 100 ? 100 : chunk_size;
            string_value2 = std::string(reinterpret_cast<const char*>(Data + start_val2), 
                                       len_val2 > Size - start_val2 ? Size - start_val2 : len_val2);
            offset += chunk_size;
            
            size_t start_val3 = offset % Size;
            size_t len_val3 = chunk_size > 100 ? 100 : chunk_size;
            string_value3 = std::string(reinterpret_cast<const char*>(Data + start_val3), 
                                       len_val3 > Size - start_val3 ? Size - start_val3 : len_val3);
        }
    } else {
        // Default strings if input is empty
        child_name = "test_child";
        attr_name1 = "attr1";
        attr_name2 = "attr2";
        attr_name3 = "attr3";
        attr_name4 = "attr4";
        attr_name5 = "attr5";
        string_value1 = "value1";
        string_value2 = "value2";
        string_value3 = "value3";
    }
    
    // Ensure strings are null-terminated by using std::string
    const char* child_name_cstr = child_name.c_str();
    const char* attr_name1_cstr = attr_name1.c_str();
    const char* attr_name2_cstr = attr_name2.c_str();
    const char* attr_name3_cstr = attr_name3.c_str();
    const char* attr_name4_cstr = attr_name4.c_str();
    const char* attr_name5_cstr = attr_name5.c_str();
    const char* string_value1_cstr = string_value1.c_str();
    const char* string_value2_cstr = string_value2.c_str();
    const char* string_value3_cstr = string_value3.c_str();
    
    // Generate numeric values from input data
    unsigned int uint_val = Size > 0 ? static_cast<unsigned int>(Data[0]) : 42;
    int int_val = Size > 1 ? static_cast<int>(Data[1]) : -42;
    
    // 1. Call pugi::xml_node::child
    pugi::xml_node found_child = root.child(child_name_cstr);
    
    // 2. Call pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = root.append_attribute(attr_name1_cstr);
    
    // 3. Call pugi::xml_attribute::set_value (all 3 overloads)
    if (attr1) {
        attr1.set_value(string_value1_cstr);
        attr1.set_value(uint_val);
        attr1.set_value(int_val);
    }
    
    // 4. Call pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = root.append_attribute(attr_name2_cstr);
    
    // 5. Call pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = root.append_attribute(attr_name3_cstr);
    
    // 6. Call pugi::xml_attribute::set_value (all 3 overloads)
    if (attr2) {
        attr2.set_value(string_value2_cstr);
        attr2.set_value(uint_val + 1);
        attr2.set_value(int_val - 1);
    }
    
    // 7. Call pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = root.append_attribute(attr_name4_cstr);
    
    // 8. Call pugi::xml_node::append_attribute
    pugi::xml_attribute attr5 = root.append_attribute(attr_name5_cstr);
    
    // 9. Call pugi::xml_attribute::set_value (all 3 overloads)
    if (attr4) {
        attr4.set_value(string_value3_cstr);
        attr4.set_value(uint_val * 2);
        attr4.set_value(int_val * 2);
    }
    
    // Try to explore different program states by using the found child node
    if (found_child) {
        // Add some attributes to the found child as well
        pugi::xml_attribute extra_attr = found_child.append_attribute("extra");
        if (extra_attr) {
            extra_attr.set_value("extra_value");
        }
    }
    
    // Cleanup happens automatically when doc goes out of scope
    return 0;
}