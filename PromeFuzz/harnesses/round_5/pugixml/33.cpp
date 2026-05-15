// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
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

static std::string make_null_terminated(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Split input into parts for different parameters
    size_t part_size = Size / 7;
    if (part_size == 0) return 0;
    
    // Create parts for each parameter
    std::string part1 = make_null_terminated(Data, part_size);
    std::string part2 = make_null_terminated(Data + part_size, part_size);
    std::string part3 = make_null_terminated(Data + 2*part_size, part_size);
    std::string part4 = make_null_terminated(Data + 3*part_size, part_size);
    std::string part5 = make_null_terminated(Data + 4*part_size, part_size);
    std::string part6 = make_null_terminated(Data + 5*part_size, part_size);
    std::string part7 = make_null_terminated(Data + 6*part_size, Size - 6*part_size);
    
    // Create XML document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create a child node to work with
    pugi::xml_node child_node = root.append_child("child");
    
    // 1. pugi::xml_node::child
    std::string_view sv1(part1);
    pugi::xml_node found_child = root.child(part1.c_str());
    
    // 2. pugi::xml_node::append_attribute (first)
    std::string_view sv2(part2);
    pugi::xml_attribute attr1 = child_node.append_attribute(part2.c_str());
    
    // 3. pugi::xml_node::append_attribute (second)
    std::string_view sv3(part3);
    pugi::xml_attribute attr2 = child_node.append_attribute(part3.c_str());
    
    // 4. pugi::xml_attribute::set_value (first, both overloads)
    if (attr1) {
        // Convert part of input to numeric values
        long long_val = 0;
        unsigned long ulong_val = 0;
        
        if (!part4.empty()) {
            // Use first character for long value
            long_val = static_cast<long>(part4[0]);
            // Use sum of characters for unsigned long
            for (char c : part4) {
                ulong_val += static_cast<unsigned char>(c);
            }
        }
        
        attr1.set_value(long_val);
        attr1.set_value(ulong_val);
    }
    
    // 5. pugi::xml_node::append_attribute (third)
    std::string_view sv5(part5);
    pugi::xml_attribute attr3 = child_node.append_attribute(part5.c_str());
    
    // 6. pugi::xml_node::append_attribute (fourth)
    std::string_view sv6(part6);
    pugi::xml_attribute attr4 = child_node.append_attribute(part6.c_str());
    
    // 7. pugi::xml_attribute::set_value (second, both overloads)
    if (attr3) {
        // Convert part of input to numeric values
        long long_val2 = 0;
        unsigned long ulong_val2 = 0;
        
        if (!part7.empty()) {
            // Use first character for long value
            long_val2 = static_cast<long>(part7[0]);
            // Use sum of characters for unsigned long
            for (char c : part7) {
                ulong_val2 += static_cast<unsigned char>(c);
            }
        }
        
        attr3.set_value(long_val2);
        attr3.set_value(ulong_val2);
    }
    
    return 0;
}