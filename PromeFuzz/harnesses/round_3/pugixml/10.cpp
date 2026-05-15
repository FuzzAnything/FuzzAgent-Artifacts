// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7098:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7112:30 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Create XML document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Prepare strings from fuzzer input for node/attribute names
    std::string_view input(reinterpret_cast<const char*>(Data), Size);
    
    // Create child node
    std::string child_name = "child";
    if (!input.empty()) {
        child_name = std::string(input.data(), std::min(input.length(), size_t(100)));
    }
    pugi::xml_node child = root.append_child(child_name.c_str());
    
    // Create attribute
    std::string attr_name1 = "attr1";
    if (!input.empty()) {
        size_t start = std::min(input.length()/2, input.length());
        size_t len = std::min(input.length() - start, size_t(100));
        attr_name1 = std::string(input.data() + start, len);
    }
    pugi::xml_attribute attr1 = child.append_attribute(attr_name1.c_str());
    
    // Set attribute values with different types and call as_double
    if (attr1) {
        // First set_value with double
        attr1.set_value(3.14159, 6);
        attr1.as_double();
        
        // Second set_value with float
        attr1.set_value(2.71828f, 5);
        attr1.as_double();
        
        // Third set_value with double again
        attr1.set_value(1.41421, 5);
        attr1.as_double();
    }
    
    // Get text object and set values
    pugi::xml_text text1 = child.text();
    if (text1) {
        // First set with double
        text1.set(3.14159, 6);
        
        // Get text again (two calls as per sequence)
        child.text();
        child.text();
        
        // Second set with float
        text1.set(2.71828f, 5);
        
        // Get text again (two calls as per sequence)
        child.text();
        child.text();
        
        // Third set with double again
        text1.set(1.41421, 5);
        
        // Final text call
        child.text();
    }
    
    return 0;
}