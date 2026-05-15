// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Use a small buffer to avoid large allocations
    const size_t max_len = 256;
    size_t len = Size > max_len ? max_len : Size;
    
    // Create null-terminated strings from fuzz data
    std::string input_str(reinterpret_cast<const char*>(Data), len);
    if (input_str.empty()) {
        input_str = "default";
    }
    
    // Ensure null termination
    std::string null_term_str = input_str + '\0';
    const char* cstr = null_term_str.c_str();
    
    // Create XML document
    pugi::xml_document doc;
    
    // Create root node
    pugi::xml_node root = doc.append_child("root");
    
    // Create a child node to work with
    pugi::xml_node child_node = root.append_child("child");
    
    // 1. Call pugi::xml_node::child
    pugi::xml_node found_child = root.child(cstr);
    (void)found_child; // Use result to avoid unused variable warning
    
    // 2. First pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = child_node.append_attribute(cstr);
    
    // 3. Second pugi::xml_node::append_attribute  
    pugi::xml_attribute attr2 = child_node.append_attribute("attr2");
    
    // 4. First pugi::xml_attribute::set_value (both overloads)
    if (attr1) {
        // Convert part of input to numeric values
        unsigned long ul_val = 0;
        long l_val = 0;
        
        if (Size >= sizeof(unsigned long)) {
            memcpy(&ul_val, Data, sizeof(unsigned long));
        }
        if (Size >= sizeof(long)) {
            memcpy(&l_val, Data, sizeof(long));
        }
        
        attr1.set_value(ul_val);
        attr1.set_value(l_val);
    }
    
    // 5. Third pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = child_node.append_attribute("attr3");
    
    // 6. Fourth pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = child_node.append_attribute(cstr);
    
    // 7. Second pugi::xml_attribute::set_value (both overloads)
    if (attr3) {
        // Use different parts of input for different values
        unsigned long ul_val2 = 0;
        long l_val2 = 0;
        
        size_t offset = Size > 8 ? 8 : 0;
        if (Size - offset >= sizeof(unsigned long)) {
            memcpy(&ul_val2, Data + offset, sizeof(unsigned long));
        }
        if (Size - offset >= sizeof(long)) {
            memcpy(&l_val2, Data + offset, sizeof(long));
        }
        
        attr3.set_value(ul_val2);
        attr3.set_value(l_val2);
    }
    
    return 0;
}