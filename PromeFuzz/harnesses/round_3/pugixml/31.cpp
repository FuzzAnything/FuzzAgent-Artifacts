// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
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

static void fuzz_pugixml(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Create document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Extract string for node names from fuzzer input
    // Ensure null termination by copying to std::string
    std::string node_name;
    if (size > 0) {
        size_t name_len = std::min(size, (size_t)100);
        node_name.assign(reinterpret_cast<const char*>(data), name_len);
        // Ensure null termination
        if (!node_name.empty() && node_name.back() != '\0') {
            node_name.push_back('\0');
        }
    } else {
        node_name = "node\0";
    }
    
    // Step 1: pugi::xml_node::child
    pugi::xml_node child_node = root.child(node_name.c_str());
    
    // If child doesn't exist, create one for subsequent operations
    if (!child_node) {
        child_node = root.append_child(node_name.c_str());
    }
    
    // Step 2: pugi::xml_node::append_child
    pugi::xml_node appended1 = child_node.append_child("child1");
    
    // Step 3: pugi::xml_node::append_child
    pugi::xml_node appended2 = child_node.append_child("child2");
    
    // Step 4: pugi::xml_text::set (both overloads)
    if (appended1) {
        pugi::xml_text text1 = appended1.text();
        // Use fuzzer data to generate integer values
        unsigned long ul_val = 0;
        long l_val = 0;
        
        if (size >= sizeof(unsigned long)) {
            memcpy(&ul_val, data, sizeof(unsigned long));
        }
        if (size >= sizeof(long)) {
            memcpy(&l_val, data, sizeof(long));
        }
        
        text1.set(ul_val);  // unsigned long overload
        text1.set(l_val);   // long overload
    }
    
    // Step 5: pugi::xml_node::append_child
    pugi::xml_node appended3 = child_node.append_child("child3");
    
    // Step 6: pugi::xml_node::append_child
    pugi::xml_node appended4 = child_node.append_child("child4");
    
    // Step 7: pugi::xml_text::set (both overloads)
    if (appended3) {
        pugi::xml_text text3 = appended3.text();
        // Use different parts of fuzzer data for these values
        unsigned long ul_val2 = 0;
        long l_val2 = 0;
        
        size_t offset = size > 8 ? 8 : 0;
        if (size >= offset + sizeof(unsigned long)) {
            memcpy(&ul_val2, data + offset, sizeof(unsigned long));
        }
        if (size >= offset + sizeof(long)) {
            memcpy(&l_val2, data + offset, sizeof(long));
        }
        
        text3.set(ul_val2);  // unsigned long overload
        text3.set(l_val2);   // long overload
    }
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    fuzz_pugixml(Data, Size);
    return 0;
}