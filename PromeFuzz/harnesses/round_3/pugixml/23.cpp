// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <string_view>

static std::string make_null_terminated(std::string_view sv) {
    return std::string(sv.data(), sv.length());
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to create strings for node/attribute names and values
    // We'll split the input into chunks for different purposes
    if (Size < 10) return 0;  // Need minimum data
    
    // Create a document with a root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some child nodes to work with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    
    // Extract strings from fuzzer input
    size_t offset = 0;
    
    // String for child name (first 1/4 of input or up to null byte)
    size_t child_name_len = Size / 4;
    if (child_name_len == 0) child_name_len = 1;
    std::string child_name_str(reinterpret_cast<const char*>(Data), 
                               std::min(child_name_len, Size));
    offset += child_name_len;
    
    // Strings for attribute names and values (using remaining input)
    std::string attr_name1, attr_name2, attr_name3, attr_name4, attr_name5;
    std::string attr_value_str;
    
    // Split remaining data into 6 parts for attribute names and string value
    size_t part_size = (Size - offset) / 6;
    if (part_size == 0) part_size = 1;
    
    if (offset < Size) {
        size_t len = std::min(part_size, Size - offset);
        attr_name1 = std::string(reinterpret_cast<const char*>(Data + offset), len);
        offset += len;
    }
    
    if (offset < Size) {
        size_t len = std::min(part_size, Size - offset);
        attr_value_str = std::string(reinterpret_cast<const char*>(Data + offset), len);
        offset += len;
    }
    
    if (offset < Size) {
        size_t len = std::min(part_size, Size - offset);
        attr_name2 = std::string(reinterpret_cast<const char*>(Data + offset), len);
        offset += len;
    }
    
    if (offset < Size) {
        size_t len = std::min(part_size, Size - offset);
        attr_name3 = std::string(reinterpret_cast<const char*>(Data + offset), len);
        offset += len;
    }
    
    if (offset < Size) {
        size_t len = std::min(part_size, Size - offset);
        attr_name4 = std::string(reinterpret_cast<const char*>(Data + offset), len);
        offset += len;
    }
    
    if (offset < Size) {
        size_t len = std::min(part_size, Size - offset);
        attr_name5 = std::string(reinterpret_cast<const char*>(Data + offset), len);
        offset += len;
    }
    
    // Ensure strings are null-terminated
    std::string child_name = make_null_terminated(child_name_str);
    std::string attr1 = make_null_terminated(attr_name1);
    std::string attr2 = make_null_terminated(attr_name2);
    std::string attr3 = make_null_terminated(attr_name3);
    std::string attr4 = make_null_terminated(attr_name4);
    std::string attr5 = make_null_terminated(attr_name5);
    std::string value_str = make_null_terminated(attr_value_str);
    
    // Generate integer values from fuzzer data
    int int_val = 0;
    unsigned int uint_val = 0;
    
    if (Size >= sizeof(int) + sizeof(unsigned int)) {
        // Use first 4 bytes for int, next 4 for unsigned int
        std::memcpy(&int_val, Data, sizeof(int));
        std::memcpy(&uint_val, Data + sizeof(int), sizeof(unsigned int));
    } else if (Size >= sizeof(int)) {
        std::memcpy(&int_val, Data, sizeof(int));
        uint_val = static_cast<unsigned int>(int_val);
    } else if (Size > 0) {
        int_val = static_cast<int>(Data[0]);
        uint_val = static_cast<unsigned int>(Data[0]);
    }
    
    try {
        // 1. pugi::xml_node::child
        pugi::xml_node found_child = root.child(child_name.c_str());
        
        // 2. pugi::xml_node::append_attribute
        pugi::xml_attribute attr1_obj = child1.append_attribute(attr1.c_str());
        
        // 3. pugi::xml_attribute::set_value (string overload)
        if (attr1_obj) {
            attr1_obj.set_value(value_str.c_str());
        }
        
        // 4. pugi::xml_node::append_attribute
        pugi::xml_attribute attr2_obj = child1.append_attribute(attr2.c_str());
        
        // 5. pugi::xml_node::append_attribute
        pugi::xml_attribute attr3_obj = child1.append_attribute(attr3.c_str());
        
        // 6. pugi::xml_attribute::set_value (int overload)
        if (attr3_obj) {
            attr3_obj.set_value(int_val);
        }
        
        // 7. pugi::xml_node::append_attribute
        pugi::xml_attribute attr4_obj = child2.append_attribute(attr4.c_str());
        
        // 8. pugi::xml_node::append_attribute
        pugi::xml_attribute attr5_obj = child2.append_attribute(attr5.c_str());
        
        // 9. pugi::xml_attribute::set_value (unsigned int overload)
        if (attr5_obj) {
            attr5_obj.set_value(uint_val);
        }
        
        // Also test the other overloads on different attributes
        if (attr2_obj) {
            attr2_obj.set_value(int_val);  // Test int overload again
        }
        
        if (attr4_obj) {
            attr4_obj.set_value(value_str.c_str());  // Test string overload again
            attr4_obj.set_value(uint_val);  // Test unsigned int overload again
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}