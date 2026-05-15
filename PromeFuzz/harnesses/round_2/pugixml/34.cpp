// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Create root node
    pugi::xml_node root = doc.append_child("root");
    
    // Prepare string for node names - ensure null termination
    std::string node_name;
    if (Size > 0) {
        size_t name_len = Size > 100 ? 100 : Size; // Limit name length
        node_name.assign(reinterpret_cast<const char*>(Data), name_len);
        // Ensure null termination
        if (node_name.back() != '\0') {
            node_name.push_back('\0');
        }
    } else {
        node_name = "default\0";
    }
    
    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child(node_name.c_str());
    
    // 2. pugi::xml_node::append_child (first)
    pugi::xml_node child1 = root.append_child(node_name.c_str());
    
    // 3. pugi::xml_node::append_child (second)
    pugi::xml_node child2 = root.append_child(node_name.c_str());
    
    // Get text object for child2
    pugi::xml_text text2 = child2.text();
    
    // 4. pugi::xml_text::set (long long overload)
    long long ll_value = 0;
    if (Size >= sizeof(long long)) {
        memcpy(&ll_value, Data, sizeof(long long));
    }
    text2.set(ll_value);
    
    // 5. pugi::xml_text::set (unsigned long long overload)
    unsigned long long ull_value = 0;
    if (Size >= sizeof(unsigned long long)) {
        memcpy(&ull_value, Data, sizeof(unsigned long long));
    }
    text2.set(ull_value);
    
    // 6. pugi::xml_node::append_child (third)
    pugi::xml_node child3 = root.append_child(node_name.c_str());
    
    // 7. pugi::xml_node::append_child (fourth)
    pugi::xml_node child4 = root.append_child(node_name.c_str());
    
    // Get text object for child4
    pugi::xml_text text4 = child4.text();
    
    // 8. pugi::xml_text::set (long long overload) - second call
    long long ll_value2 = 0;
    if (Size >= sizeof(long long) * 2) {
        memcpy(&ll_value2, Data + sizeof(long long), sizeof(long long));
    }
    text4.set(ll_value2);
    
    // 9. pugi::xml_text::set (unsigned long long overload) - second call
    unsigned long long ull_value2 = 0;
    if (Size >= sizeof(unsigned long long) * 2) {
        memcpy(&ull_value2, Data + sizeof(unsigned long long), sizeof(unsigned long long));
    }
    text4.set(ull_value2);
    
    return 0;
}