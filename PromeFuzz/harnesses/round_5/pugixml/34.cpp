// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with a root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Ensure we have enough data for operations
    if (Size < 5) return 0;
    
    // Create null-terminated strings from fuzzer input
    // We'll use different segments of the input for different operations
    size_t quarter = Size / 4;
    
    // First string for child() operation
    std::string child_name;
    if (quarter > 0) {
        child_name.assign(reinterpret_cast<const char*>(Data), 
                         std::min(quarter, Size));
        // Ensure null termination
        if (!child_name.empty()) {
            child_name[child_name.size() - 1] = '\0';
        }
    }
    
    // Second string for first append_child
    std::string append_name1;
    if (quarter > 0 && Size >= quarter) {
        append_name1.assign(reinterpret_cast<const char*>(Data + quarter), 
                           std::min(quarter, Size - quarter));
        if (!append_name1.empty()) {
            append_name1[append_name1.size() - 1] = '\0';
        }
    }
    
    // Third string for second append_child
    std::string append_name2;
    if (quarter > 0 && Size >= 2*quarter) {
        append_name2.assign(reinterpret_cast<const char*>(Data + 2*quarter), 
                           std::min(quarter, Size - 2*quarter));
        if (!append_name2.empty()) {
            append_name2[append_name2.size() - 1] = '\0';
        }
    }
    
    // Fourth string for third append_child
    std::string append_name3;
    if (quarter > 0 && Size >= 3*quarter) {
        append_name3.assign(reinterpret_cast<const char*>(Data + 3*quarter), 
                           std::min(quarter, Size - 3*quarter));
        if (!append_name3.empty()) {
            append_name3[append_name3.size() - 1] = '\0';
        }
    }
    
    // Fifth string for fourth append_child
    std::string append_name4;
    if (Size > 0) {
        // Use remaining data or wrap around
        size_t start = std::min(4*quarter, Size - 1);
        append_name4.assign(reinterpret_cast<const char*>(Data + start), 
                           Size - start);
        if (!append_name4.empty()) {
            append_name4[append_name4.size() - 1] = '\0';
        }
    }
    
    // 1. Call child() on root node
    pugi::xml_node child_node = root.child(child_name.c_str());
    
    // If child doesn't exist, create it for subsequent operations
    if (!child_node) {
        child_node = root.append_child(child_name.c_str());
    }
    
    // 2. First append_child
    pugi::xml_node node1 = child_node.append_child(append_name1.c_str());
    
    // 3. Second append_child
    pugi::xml_node node2 = child_node.append_child(append_name2.c_str());
    
    // 4. Set text on node1 using both overloads
    if (node1) {
        pugi::xml_text text1 = node1.text();
        
        // Create integer values from fuzzer data
        long long ll_value = 0;
        unsigned long long ull_value = 0;
        
        if (Size >= sizeof(long long)) {
            std::memcpy(&ll_value, Data, sizeof(long long));
        }
        if (Size >= sizeof(unsigned long long)) {
            std::memcpy(&ull_value, Data, sizeof(unsigned long long));
        }
        
        // Call both overloads
        text1.set(ll_value);
        text1.set(ull_value);
    }
    
    // 5. Third append_child
    pugi::xml_node node3 = child_node.append_child(append_name3.c_str());
    
    // 6. Fourth append_child
    pugi::xml_node node4 = child_node.append_child(append_name4.c_str());
    
    // 7. Set text on node3 using both overloads
    if (node3) {
        pugi::xml_text text3 = node3.text();
        
        // Create different integer values from fuzzer data
        long long ll_value2 = 0;
        unsigned long long ull_value2 = 0;
        
        // Use different parts of the input for variety
        size_t offset = Size / 2;
        if (offset + sizeof(long long) <= Size) {
            std::memcpy(&ll_value2, Data + offset, sizeof(long long));
        }
        if (offset + sizeof(unsigned long long) <= Size) {
            std::memcpy(&ull_value2, Data + offset, sizeof(unsigned long long));
        }
        
        // Call both overloads
        text3.set(ll_value2);
        text3.set(ull_value2);
    }
    
    return 0;
}