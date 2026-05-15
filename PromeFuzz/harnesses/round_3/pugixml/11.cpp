// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Initialize document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Use a simple approach to generate values from fuzz data
    // We'll use the first few bytes for different parameter types
    int int_val = 0;
    unsigned int uint_val = 0;
    bool bool_val = false;
    float float_val = 0.0f;
    double double_val = 0.0;
    
    if (Size >= sizeof(int)) {
        std::memcpy(&int_val, Data, sizeof(int));
    }
    if (Size >= sizeof(unsigned int)) {
        std::memcpy(&uint_val, Data, sizeof(unsigned int));
    }
    if (Size >= 1) {
        bool_val = Data[0] & 1;
    }
    if (Size >= sizeof(float)) {
        std::memcpy(&float_val, Data, sizeof(float));
    }
    if (Size >= sizeof(double)) {
        std::memcpy(&double_val, Data, sizeof(double));
    }
    
    // Create string view from data (safe bounded)
    std::string_view str_view(reinterpret_cast<const char*>(Data), 
                              Size > 100 ? 100 : Size);
    
    // Sequence of API calls as specified
    // First: append_child -> append_child -> set (5 overloads)
    pugi::xml_node node1 = root.append_child("node1");
    pugi::xml_node node2 = node1.append_child("node2");
    pugi::xml_text text2 = node2.text();
    text2.set(int_val);
    text2.set(uint_val);
    text2.set(bool_val);
    text2.set(float_val);
    text2.set(double_val);
    
    // Second: append_child -> append_child -> set (5 overloads)
    pugi::xml_node node3 = node2.append_child("node3");
    pugi::xml_node node4 = node3.append_child("node4");
    pugi::xml_text text4 = node4.text();
    text4.set(int_val);
    text4.set(uint_val);
    text4.set(bool_val);
    text4.set(float_val);
    text4.set(double_val);
    
    // Third: append_child -> set (5 overloads)
    pugi::xml_node node5 = node4.append_child("node5");
    pugi::xml_text text5 = node5.text();
    text5.set(int_val);
    text5.set(uint_val);
    text5.set(bool_val);
    text5.set(float_val);
    text5.set(double_val);
    
    // Fourth: append_child -> set (5 overloads)
    pugi::xml_node node6 = node5.append_child("node6");
    pugi::xml_text text6 = node6.text();
    text6.set(int_val);
    text6.set(uint_val);
    text6.set(bool_val);
    text6.set(float_val);
    text6.set(double_val);
    
    // Fifth: append_child -> set (5 overloads)
    pugi::xml_node node7 = node6.append_child("node7");
    pugi::xml_text text7 = node7.text();
    text7.set(int_val);
    text7.set(uint_val);
    text7.set(bool_val);
    text7.set(float_val);
    text7.set(double_val);
    
    return 0;
}