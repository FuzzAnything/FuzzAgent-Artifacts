// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Initialize document
    pugi::xml_document doc;
    
    // Create root node
    pugi::xml_node root = doc.append_child("root");
    
    // First append_child
    pugi::xml_node node1 = root.append_child("node1");
    
    // Second append_child
    pugi::xml_node node2 = node1.append_child("node2");
    
    // First set (on node2)
    pugi::xml_text text2 = node2.text();
    text2.set(false);  // bool
    text2.set(0);      // int
    text2.set(0u);     // unsigned int
    text2.set(0.0);    // double
    text2.set(0.0f);   // float
    
    // Third append_child
    pugi::xml_node node3 = node2.append_child("node3");
    
    // Fourth append_child
    pugi::xml_node node4 = node3.append_child("node4");
    
    // Second set (on node4)
    pugi::xml_text text4 = node4.text();
    text4.set(true);   // bool
    text4.set(1);      // int
    text4.set(1u);     // unsigned int
    text4.set(1.0);    // double
    text4.set(1.0f);   // float
    
    // Fifth append_child
    pugi::xml_node node5 = node4.append_child("node5");
    
    // Third set (on node5)
    pugi::xml_text text5 = node5.text();
    text5.set(false);  // bool
    text5.set(-1);     // int
    text5.set(2u);     // unsigned int
    text5.set(2.0);    // double
    text5.set(2.0f);   // float
    
    // Sixth append_child
    pugi::xml_node node6 = node5.append_child("node6");
    
    // Fourth set (on node6)
    pugi::xml_text text6 = node6.text();
    text6.set(true);   // bool
    text6.set(3);      // int
    text6.set(3u);     // unsigned int
    text6.set(3.0);    // double
    text6.set(3.0f);   // float
    
    // Seventh append_child
    pugi::xml_node node7 = node6.append_child("node7");
    
    // Fifth set (on node7)
    pugi::xml_text text7 = node7.text();
    text7.set(false);  // bool
    text7.set(4);      // int
    text7.set(4u);     // unsigned int
    text7.set(4.0);    // double
    text7.set(4.0f);   // float
    
    // Use fuzz data to create more diverse values for set operations
    if (Size > 0) {
        // Use first byte for bool
        bool bool_val = Data[0] & 1;
        
        // Use next bytes for int values
        int int_val = 0;
        unsigned int uint_val = 0;
        double double_val = 0.0;
        float float_val = 0.0f;
        
        if (Size >= sizeof(int) + 1) {
            memcpy(&int_val, Data + 1, sizeof(int));
        }
        
        if (Size >= sizeof(int) * 2 + 1) {
            memcpy(&uint_val, Data + 1 + sizeof(int), sizeof(unsigned int));
        }
        
        if (Size >= sizeof(int) * 2 + sizeof(double) + 1) {
            memcpy(&double_val, Data + 1 + sizeof(int) * 2, sizeof(double));
        }
        
        if (Size >= sizeof(int) * 2 + sizeof(double) + sizeof(float) + 1) {
            memcpy(&float_val, Data + 1 + sizeof(int) * 2 + sizeof(double), sizeof(float));
        }
        
        // Create additional nodes with fuzz data
        pugi::xml_node extra_node = root.append_child("extra");
        pugi::xml_text extra_text = extra_node.text();
        extra_text.set(bool_val);
        extra_text.set(int_val);
        extra_text.set(uint_val);
        extra_text.set(double_val);
        extra_text.set(float_val);
    }
    
    return 0;
}