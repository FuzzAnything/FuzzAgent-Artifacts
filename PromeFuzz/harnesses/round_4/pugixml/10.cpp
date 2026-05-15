// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5554:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
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
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");

    // Use data to create node name
    std::string_view node_name(reinterpret_cast<const char*>(Data), 
                               Size > 100 ? 100 : Size);
    std::string node_name_str(node_name.data(), node_name.length());
    
    // Append child node
    pugi::xml_node child = root.append_child(node_name_str.c_str());
    
    // Use data for attribute name
    std::string_view attr_name1(reinterpret_cast<const char*>(Data), 
                                Size > 50 ? 50 : Size);
    std::string attr_name1_str(attr_name1.data(), attr_name1.length());
    
    // Append attribute
    pugi::xml_attribute attr1 = child.append_attribute(attr_name1_str.c_str());
    
    // Use data for numeric values - interpret bytes as double/float
    double d1 = 0.0;
    float f1 = 0.0f;
    double d2 = 0.0;
    float f2 = 0.0f;
    double d3 = 0.0;
    
    if (Size >= sizeof(double)) {
        d1 = *reinterpret_cast<const double*>(Data);
    }
    if (Size >= sizeof(double) + sizeof(float)) {
        f1 = *reinterpret_cast<const float*>(Data + sizeof(double));
    }
    if (Size >= sizeof(double) + sizeof(float) + sizeof(double)) {
        d2 = *reinterpret_cast<const double*>(Data + sizeof(double) + sizeof(float));
    }
    if (Size >= sizeof(double) + sizeof(float) + sizeof(double) + sizeof(float)) {
        f2 = *reinterpret_cast<const float*>(Data + sizeof(double) + sizeof(float) + sizeof(double));
    }
    if (Size >= sizeof(double) + sizeof(float) + sizeof(double) + sizeof(float) + sizeof(double)) {
        d3 = *reinterpret_cast<const double*>(Data + sizeof(double) + sizeof(float) + sizeof(double) + sizeof(float));
    }
    
    int precision = 6;
    
    // First set_value (double)
    attr1.set_value(d1, precision);
    
    // First as_double
    double result1 = attr1.as_double();
    
    // Second set_value (float)
    attr1.set_value(f1, precision);
    
    // Second as_double
    double result2 = attr1.as_double();
    
    // Third set_value (double)
    attr1.set_value(d2, precision);
    
    // Third as_double
    double result3 = attr1.as_double();
    
    // Get text object
    pugi::xml_text text1 = child.text();
    
    // First set (float)
    text1.set(f1, precision);
    
    // Get text object again
    pugi::xml_text text2 = child.text();
    
    // Get text object again
    pugi::xml_text text3 = child.text();
    
    // Second set (double)
    text3.set(d3, precision);
    
    // Get text object again
    pugi::xml_text text4 = child.text();
    
    // Get text object again
    pugi::xml_text text5 = child.text();
    
    // Third set (float) - using f2
    text5.set(f2, precision);
    
    // Final text get
    pugi::xml_text text6 = child.text();
    
    // Use results to avoid optimization
    volatile double dummy = result1 + result2 + result3;
    (void)dummy;
    
    return 0;
}