// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
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

static void fuzz_sequence(pugi::xml_document& doc, double d_val, unsigned int u_val, int i_val, bool b_val, float f_val, int i_val2) {
    // Create root node
    pugi::xml_node root = doc.append_child("root");
    
    // First append_child
    pugi::xml_node child1 = root.append_child("child1");
    
    // Second append_child
    pugi::xml_node child2 = root.append_child("child2");
    
    // First set (double)
    pugi::xml_text text1 = child1.text();
    text1.set(d_val);
    
    // Third append_child
    pugi::xml_node child3 = root.append_child("child3");
    
    // Fourth append_child
    pugi::xml_node child4 = root.append_child("child4");
    
    // Second set (unsigned int)
    pugi::xml_text text2 = child2.text();
    text2.set(u_val);
    
    // Fifth append_child
    pugi::xml_node child5 = root.append_child("child5");
    
    // Third set (int)
    pugi::xml_text text3 = child3.text();
    text3.set(i_val);
    
    // Sixth append_child
    pugi::xml_node child6 = root.append_child("child6");
    
    // Fourth set (bool)
    pugi::xml_text text4 = child4.text();
    text4.set(b_val);
    
    // Seventh append_child
    pugi::xml_node child7 = root.append_child("child7");
    
    // Fifth set (float)
    pugi::xml_text text5 = child5.text();
    text5.set(f_val);
    
    // Sixth set (int again)
    pugi::xml_text text6 = child6.text();
    text6.set(i_val2);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0; // Need enough data for all parameters
    
    // Extract values from fuzzer input
    double d_val = *reinterpret_cast<const double*>(Data);
    unsigned int u_val = *reinterpret_cast<const unsigned int*>(Data + 8);
    int i_val = *reinterpret_cast<const int*>(Data + 12);
    bool b_val = *reinterpret_cast<const bool*>(Data + 16);
    float f_val = *reinterpret_cast<const float*>(Data + 17);
    int i_val2 = *reinterpret_cast<const int*>(Data + 21);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Run the sequence
    fuzz_sequence(doc, d_val, u_val, i_val, b_val, f_val, i_val2);
    
    return 0;
}