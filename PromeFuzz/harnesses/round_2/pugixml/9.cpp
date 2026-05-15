// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5554:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5540:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5554:35 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to generate strings for node/attribute names and values
    // We'll use a simple approach: create strings from the input data
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Create document
    pugi::xml_document doc;
    
    // Append child node
    pugi::xml_node node = doc.append_child("test_node");
    
    // Append attribute
    pugi::xml_attribute attr = node.append_attribute("test_attr");
    
    // First set_value (double) and as_double
    attr.set_value(3.14159, 6);
    double val1 = attr.as_double(0.0);
    
    // Second set_value (float) and as_double
    attr.set_value(2.71828f, 6);
    double val2 = attr.as_double(0.0);
    
    // Third set_value (double) and as_double
    attr.set_value(1.41421, 6);
    double val3 = attr.as_double(0.0);
    
    // First text() and set (double)
    pugi::xml_text text1 = node.text();
    text1.set(3.14159, 6);
    
    // Second text() and third text()
    pugi::xml_text text2 = node.text();
    pugi::xml_text text3 = node.text();
    
    // Second set (float)
    text2.set(2.71828f, 6);
    
    // Fourth text() and fifth text()
    pugi::xml_text text4 = node.text();
    pugi::xml_text text5 = node.text();
    
    // Third set (double)
    text4.set(1.41421, 6);
    
    // Sixth text()
    pugi::xml_text text6 = node.text();
    
    // Use the fuzzer input to create more diverse test cases
    if (Size > 0) {
        // Create node name from input
        std::string node_name = input_str.substr(0, Size % 32);
        if (!node_name.empty()) {
            pugi::xml_node node2 = doc.append_child(node_name.c_str());
            
            // Create attribute name from input
            std::string attr_name = input_str.substr(0, (Size / 2) % 32);
            if (!attr_name.empty()) {
                pugi::xml_attribute attr2 = node2.append_attribute(attr_name.c_str());
                
                // Use input to generate double and float values
                double dval = 0.0;
                float fval = 0.0f;
                
                if (Size >= sizeof(double)) {
                    memcpy(&dval, Data, sizeof(double));
                }
                if (Size >= sizeof(float)) {
                    memcpy(&fval, Data, sizeof(float));
                }
                
                // Test set_value overloads
                attr2.set_value(dval, 6);
                attr2.set_value(fval, 6);
                
                // Test as_double
                double test_val = attr2.as_double(0.0);
                
                // Test text() and set() overloads
                pugi::xml_text text_node2 = node2.text();
                text_node2.set(dval, 6);
                text_node2.set(fval, 6);
            }
        }
    }
    
    return 0;
}