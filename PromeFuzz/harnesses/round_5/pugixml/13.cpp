// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <cstring>

static void set_text_all_overloads(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (size >= sizeof(unsigned int)) {
        unsigned int ui_val;
        memcpy(&ui_val, data, sizeof(unsigned int));
        text.set(ui_val);
    }
    
    if (size >= sizeof(double)) {
        double d_val;
        memcpy(&d_val, data, sizeof(double));
        text.set(d_val);
    }
    
    if (size >= sizeof(float)) {
        float f_val;
        memcpy(&f_val, data, sizeof(float));
        text.set(f_val);
    }
    
    if (size >= sizeof(int)) {
        int i_val;
        memcpy(&i_val, data, sizeof(int));
        text.set(i_val);
    }
    
    if (size >= 1) {
        bool b_val = data[0] & 1;
        text.set(b_val);
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    // Create root node
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;
    
    // First append_child
    pugi::xml_node node1 = root.append_child("node1");
    if (!node1) return 0;
    
    // Second append_child
    pugi::xml_node node2 = node1.append_child("node2");
    if (!node2) return 0;
    
    // First xml_text::set (all 5 overloads)
    pugi::xml_text text1 = node2.text();
    set_text_all_overloads(text1, Data, Size);
    
    // Third append_child
    pugi::xml_node node3 = node2.append_child("node3");
    if (!node3) return 0;
    
    // Fourth append_child
    pugi::xml_node node4 = node3.append_child("node4");
    if (!node4) return 0;
    
    // Second xml_text::set (all 5 overloads)
    pugi::xml_text text2 = node4.text();
    set_text_all_overloads(text2, Data, Size);
    
    // Fifth append_child
    pugi::xml_node node5 = node4.append_child("node5");
    if (!node5) return 0;
    
    // Third xml_text::set (all 5 overloads)
    pugi::xml_text text3 = node5.text();
    set_text_all_overloads(text3, Data, Size);
    
    // Sixth append_child
    pugi::xml_node node6 = node5.append_child("node6");
    if (!node6) return 0;
    
    // Fourth xml_text::set (all 5 overloads)
    pugi::xml_text text4 = node6.text();
    set_text_all_overloads(text4, Data, Size);
    
    // Seventh append_child
    pugi::xml_node node7 = node6.append_child("node7");
    if (!node7) return 0;
    
    // Fifth xml_text::set (all 5 overloads)
    pugi::xml_text text5 = node7.text();
    set_text_all_overloads(text5, Data, Size);
    
    return 0;
}