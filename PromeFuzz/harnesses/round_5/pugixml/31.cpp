// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create XML document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create child node for testing
    pugi::xml_node child_node = root.append_child("child");
    
    // Prepare string data from fuzz input
    // Ensure we have enough data for string operations
    size_t str_len = Size > 100 ? 100 : Size;
    std::string str_data(reinterpret_cast<const char*>(Data), str_len);
    
    // Ensure null-terminated strings for API calls
    std::string child_name = "test_child";
    if (!str_data.empty()) {
        child_name = std::string(str_data.data(), str_data.length());
    }
    
    // 1. Call xml_node::child
    pugi::xml_node found_child = root.child(child_name.c_str());
    
    // Use child_node for subsequent operations since found_child might be empty
    pugi::xml_node target_node = child_node;
    
    // Prepare attribute names
    std::string attr_name1 = "attr1";
    std::string attr_name2 = "attr2";
    std::string attr_name3 = "attr3";
    std::string attr_name4 = "attr4";
    
    if (Size > 1) {
        // Use different parts of input for attribute names
        size_t offset = 0;
        size_t chunk = str_len / 4;
        
        if (chunk > 0) {
            attr_name1 = std::string(str_data.data() + offset, chunk);
            offset += chunk;
            attr_name2 = std::string(str_data.data() + offset, chunk);
            offset += chunk;
            attr_name3 = std::string(str_data.data() + offset, chunk);
            offset += chunk;
            attr_name4 = std::string(str_data.data() + offset, chunk);
        }
    }
    
    // 2. First append_attribute
    pugi::xml_attribute attr1 = target_node.append_attribute(attr_name1.c_str());
    
    // 3. Second append_attribute
    pugi::xml_attribute attr2 = target_node.append_attribute(attr_name2.c_str());
    
    // 4. First set_value with both overloads
    if (attr1) {
        // Extract integer values from fuzz data
        unsigned long long ull_val = 0;
        long long ll_val = 0;
        
        if (Size >= sizeof(unsigned long long)) {
            memcpy(&ull_val, Data, sizeof(unsigned long long));
        }
        if (Size >= sizeof(long long)) {
            memcpy(&ll_val, Data, sizeof(long long));
        }
        
        attr1.set_value(ull_val);
        attr1.set_value(ll_val);
    }
    
    // 5. Third append_attribute
    pugi::xml_attribute attr3 = target_node.append_attribute(attr_name3.c_str());
    
    // 6. Fourth append_attribute
    pugi::xml_attribute attr4 = target_node.append_attribute(attr_name4.c_str());
    
    // 7. Second set_value with both overloads
    if (attr3) {
        // Use different parts of data for second set of values
        unsigned long long ull_val2 = 0;
        long long ll_val2 = 0;
        
        size_t offset = Size > 8 ? 8 : 0;
        if (Size - offset >= sizeof(unsigned long long)) {
            memcpy(&ull_val2, Data + offset, sizeof(unsigned long long));
        }
        if (Size - offset >= sizeof(long long)) {
            memcpy(&ll_val2, Data + offset, sizeof(long long));
        }
        
        attr3.set_value(ull_val2);
        attr3.set_value(ll_val2);
    }
    
    return 0;
}