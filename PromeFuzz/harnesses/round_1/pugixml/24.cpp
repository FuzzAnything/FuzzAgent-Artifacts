// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Use a fixed-size buffer to avoid dynamic allocation issues during fuzzing
    static constexpr size_t MAX_BUFFER_SIZE = 4096;
    char buffer[MAX_BUFFER_SIZE];
    
    if (Size == 0 || Size > MAX_BUFFER_SIZE) {
        return 0;
    }
    
    // Copy fuzzer data to buffer and ensure null termination
    std::memcpy(buffer, Data, Size);
    buffer[Size] = '\0';
    
    try {
        // Create XML document
        pugi::xml_document doc;
        
        // Create root node
        pugi::xml_node root = doc.append_child("root");
        
        // Create child node with name from fuzzer data
        std::string_view child_name(buffer, Size);
        std::string child_name_str(child_name.data(), child_name.length());
        pugi::xml_node child_node = root.append_child(child_name_str.c_str());
        
        // 1. pugi::xml_node::child
        std::string_view child_search(buffer, std::min(Size, (size_t)10));
        std::string child_search_str(child_search.data(), child_search.length());
        pugi::xml_node found_child = root.child(child_search_str.c_str());
        
        // 2. pugi::xml_node::append_attribute (first)
        std::string_view attr_name1(buffer, std::min(Size, (size_t)20));
        std::string attr_name1_str(attr_name1.data(), attr_name1.length());
        pugi::xml_attribute attr1 = child_node.append_attribute(attr_name1_str.c_str());
        
        // 3. pugi::xml_attribute::set_value (string overload)
        if (attr1) {
            std::string_view value1(buffer, std::min(Size, (size_t)30));
            std::string value1_str(value1.data(), value1.length());
            attr1.set_value(value1_str.c_str());
        }
        
        // 4. pugi::xml_node::append_attribute (second)
        std::string_view attr_name2(buffer + std::min(Size, (size_t)5), 
                                   std::min(Size - std::min(Size, (size_t)5), (size_t)20));
        std::string attr_name2_str(attr_name2.data(), attr_name2.length());
        pugi::xml_attribute attr2 = child_node.append_attribute(attr_name2_str.c_str());
        
        // 5. pugi::xml_node::append_attribute (third)
        std::string_view attr_name3(buffer + std::min(Size, (size_t)10), 
                                   std::min(Size - std::min(Size, (size_t)10), (size_t)20));
        std::string attr_name3_str(attr_name3.data(), attr_name3.length());
        pugi::xml_attribute attr3 = child_node.append_attribute(attr_name3_str.c_str());
        
        // 6. pugi::xml_attribute::set_value (int overload)
        if (attr3) {
            // Use part of fuzzer data as integer value
            int int_value = 0;
            if (Size >= sizeof(int)) {
                std::memcpy(&int_value, Data, sizeof(int));
            }
            attr3.set_value(int_value);
        }
        
        // 7. pugi::xml_node::append_attribute (fourth)
        std::string_view attr_name4(buffer + std::min(Size, (size_t)15), 
                                   std::min(Size - std::min(Size, (size_t)15), (size_t)20));
        std::string attr_name4_str(attr_name4.data(), attr_name4.length());
        pugi::xml_attribute attr4 = child_node.append_attribute(attr_name4_str.c_str());
        
        // 8. pugi::xml_node::append_attribute (fifth)
        std::string_view attr_name5(buffer + std::min(Size, (size_t)20), 
                                   std::min(Size - std::min(Size, (size_t)20), (size_t)20));
        std::string attr_name5_str(attr_name5.data(), attr_name5.length());
        pugi::xml_attribute attr5 = child_node.append_attribute(attr_name5_str.c_str());
        
        // 9. pugi::xml_attribute::set_value (unsigned int overload)
        if (attr5) {
            // Use part of fuzzer data as unsigned integer value
            unsigned int uint_value = 0;
            if (Size >= sizeof(unsigned int)) {
                std::memcpy(&uint_value, Data, sizeof(unsigned int));
            }
            attr5.set_value(uint_value);
        }
        
        // Additional exploration: try set_value with different data
        if (attr2) {
            // Try set_value with another string
            std::string_view value2(buffer + std::min(Size, (size_t)25), 
                                   std::min(Size - std::min(Size, (size_t)25), (size_t)30));
            std::string value2_str(value2.data(), value2.length());
            attr2.set_value(value2_str.c_str());
            
            // Try set_value with integer
            int int_value2 = static_cast<int>(Size);
            attr2.set_value(int_value2);
            
            // Try set_value with unsigned integer
            unsigned int uint_value2 = static_cast<unsigned int>(Size);
            attr2.set_value(uint_value2);
        }
        
    } catch (...) {
        // Catch all exceptions to prevent crashes from propagating
    }
    
    return 0;
}