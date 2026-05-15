// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    // Create a document to work with
    pugi::xml_document doc;
    
    // Create a node and attribute to test on
    pugi::xml_node node = doc.append_child(pugi::node_element);
    pugi::xml_attribute attr = node.append_attribute("initial_attr");
    
    // Ensure we have valid objects
    if (node.empty() || attr.empty()) {
        return 0;
    }
    
    // Convert fuzzer input to string view for string operations
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test pugi::xml_attribute::set_name overloads
    if (!input_str.empty()) {
        // Overload 1: with size parameter
        attr.set_name(input_str.data(), input_str.size());
        
        // Overload 2: without size parameter (requires null-terminated string)
        // Create a null-terminated copy of the input
        static char null_terminated_buf[4096];
        size_t copy_size = (Size < sizeof(null_terminated_buf) - 1) ? Size : sizeof(null_terminated_buf) - 1;
        if (copy_size > 0) {
            memcpy(null_terminated_buf, Data, copy_size);
            null_terminated_buf[copy_size] = '\0';
            attr.set_name(null_terminated_buf);
        }
    }
    
    // Test pugi::xml_node::set_name
    if (!input_str.empty()) {
        node.set_name(input_str.data(), input_str.size());
    }
    
    // Test pugi::xml_node::set_value
    if (!input_str.empty()) {
        node.set_value(input_str.data(), input_str.size());
    }
    
    // Test pugi::xml_attribute::set_value numeric overloads
    if (Size >= sizeof(long)) {
        long long_val;
        memcpy(&long_val, Data, sizeof(long));
        attr.set_value(long_val);
    }
    
    if (Size >= sizeof(unsigned long)) {
        unsigned long ulong_val;
        memcpy(&ulong_val, Data, sizeof(unsigned long));
        attr.set_value(ulong_val);
    }
    
    return 0;
}