// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with a root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Ensure we have enough data for operations
    if (Size < 1) return 0;
    
    // Prepare child name - ensure null termination
    size_t child_name_len = Size > 100 ? 100 : Size;
    std::string child_name_str(reinterpret_cast<const char*>(Data), child_name_len);
    if (!child_name_str.empty()) {
        // Ensure null termination
        child_name_str.push_back('\0');
    }
    
    // 1. Call pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name_str.c_str());
    
    // Prepare attribute names - use different parts of input data
    size_t attr1_len = Size > 50 ? 50 : Size/2;
    size_t attr2_len = Size > 30 ? 30 : Size/3;
    size_t attr3_len = Size > 20 ? 20 : Size/4;
    size_t attr4_len = Size > 10 ? 10 : Size/5;
    
    std::string attr_name1_str, attr_name2_str, attr_name3_str, attr_name4_str;
    
    if (Size >= 1) {
        attr_name1_str = std::string(reinterpret_cast<const char*>(Data), 
                                    attr1_len > 0 ? attr1_len : 1);
        attr_name1_str.push_back('\0');
    }
    
    if (Size >= 2) {
        size_t offset = attr1_len % Size;
        attr_name2_str = std::string(reinterpret_cast<const char*>(Data + offset), 
                                    attr2_len > 0 ? attr2_len : 1);
        attr_name2_str.push_back('\0');
    }
    
    // 2. Call pugi::xml_node::append_attribute (first time)
    pugi::xml_attribute attr1 = root.append_attribute(attr_name1_str.c_str());
    
    // 3. Call pugi::xml_node::append_attribute (second time)
    pugi::xml_attribute attr2 = root.append_attribute(attr_name2_str.c_str());
    
    // 4. Call pugi::xml_attribute::set_value (both overloads)
    if (attr2) {
        // Use parts of input data for values
        unsigned long ul_val = 0;
        long l_val = 0;
        
        if (Size >= sizeof(unsigned long)) {
            memcpy(&ul_val, Data, sizeof(unsigned long));
        }
        if (Size >= sizeof(long)) {
            memcpy(&l_val, Data, sizeof(long));
        }
        
        attr2.set_value(ul_val);  // unsigned long overload
        attr2.set_value(l_val);   // long overload
    }
    
    // Prepare more attribute names
    if (Size >= 3) {
        size_t offset = (attr1_len + attr2_len) % Size;
        attr_name3_str = std::string(reinterpret_cast<const char*>(Data + offset), 
                                    attr3_len > 0 ? attr3_len : 1);
        attr_name3_str.push_back('\0');
    }
    
    if (Size >= 4) {
        size_t offset = (attr1_len + attr2_len + attr3_len) % Size;
        attr_name4_str = std::string(reinterpret_cast<const char*>(Data + offset), 
                                    attr4_len > 0 ? attr4_len : 1);
        attr_name4_str.push_back('\0');
    }
    
    // 5. Call pugi::xml_node::append_attribute (third time)
    pugi::xml_attribute attr3 = root.append_attribute(attr_name3_str.c_str());
    
    // 6. Call pugi::xml_node::append_attribute (fourth time)
    pugi::xml_attribute attr4 = root.append_attribute(attr_name4_str.c_str());
    
    // 7. Call pugi::xml_attribute::set_value (both overloads again)
    if (attr4) {
        // Use different parts of input data for values
        unsigned long ul_val2 = 0;
        long l_val2 = 0;
        
        if (Size >= sizeof(unsigned long) * 2) {
            memcpy(&ul_val2, Data + sizeof(unsigned long), sizeof(unsigned long));
        }
        if (Size >= sizeof(long) * 2) {
            memcpy(&l_val2, Data + sizeof(long), sizeof(long));
        }
        
        attr4.set_value(ul_val2);  // unsigned long overload
        attr4.set_value(l_val2);   // long overload
    }
    
    return 0;
}