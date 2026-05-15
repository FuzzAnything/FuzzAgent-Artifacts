// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data for XML content
    std::string xml_content(reinterpret_cast<const char*>(Data), Size);
    
    // Create document and load XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    // If parsing failed, we can still continue with empty document
    (void)result;
    
    // Get first child with name from first byte (if available)
    pugi::xml_node node1;
    if (Size >= 1) {
        char name1[2] = {static_cast<char>(Data[0]), '\0'};
        node1 = doc.child(name1);
    } else {
        node1 = doc.child("");
    }
    
    // Get child of node1 with name from second byte (if available)
    pugi::xml_node node2;
    if (Size >= 2) {
        char name2[2] = {static_cast<char>(Data[1 % Size]), '\0'};
        node2 = node1.child(name2);
    } else {
        node2 = node1.child("");
    }
    
    // Set name of node2 using third byte (if available)
    if (Size >= 3) {
        char new_name[2] = {static_cast<char>(Data[2 % Size]), '\0'};
        node2.set_name(new_name);
    }
    
    // Get attribute from node2 with name from fourth byte (if available)
    pugi::xml_attribute attr1;
    if (Size >= 4) {
        char attr_name1[2] = {static_cast<char>(Data[3 % Size]), '\0'};
        attr1 = node2.attribute(attr_name1);
    } else {
        attr1 = node2.attribute("");
    }
    
    // Set name of attribute using fifth byte (if available)
    if (Size >= 5) {
        char new_attr_name[2] = {static_cast<char>(Data[4 % Size]), '\0'};
        attr1.set_name(new_attr_name);
    }
    
    // Get another attribute from node2 with name from sixth byte (if available)
    pugi::xml_attribute attr2;
    if (Size >= 6) {
        char attr_name2[2] = {static_cast<char>(Data[5 % Size]), '\0'};
        attr2 = node2.attribute(attr_name2);
    } else {
        attr2 = node2.attribute("");
    }
    
    // Set value of attribute using seventh byte (if available)
    if (Size >= 7) {
        char attr_value[2] = {static_cast<char>(Data[6 % Size]), '\0'};
        attr2.set_value(attr_value);
    }
    
    // Get child of node2 with name from eighth byte (if available)
    pugi::xml_node node3;
    if (Size >= 8) {
        char name3[2] = {static_cast<char>(Data[7 % Size]), '\0'};
        node3 = node2.child(name3);
    } else {
        node3 = node2.child("");
    }
    
    // Get text of node3 and set it using ninth byte (if available)
    pugi::xml_text text = node3.text();
    if (Size >= 9) {
        char text_value[2] = {static_cast<char>(Data[8 % Size]), '\0'};
        text.set(text_value);
    }
    
    // Append copy of node3 to node2
    node2.append_copy(node3);
    
    // Get child of node2 with name from tenth byte (if available)
    pugi::xml_node node4;
    if (Size >= 10) {
        char name4[2] = {static_cast<char>(Data[9 % Size]), '\0'};
        node4 = node2.child(name4);
    } else {
        node4 = node2.child("");
    }
    
    // Append copy of node4 to node2
    node2.append_copy(node4);
    
    return 0;
}