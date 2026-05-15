// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node node;
    
    // Use first byte to determine node type to create
    uint8_t node_type_selector = Data[0] % 9; // 9 possible node types
    const uint8_t* string_data = Data + 1;
    size_t string_size = Size - 1;
    
    // Create different node types based on selector
    switch (node_type_selector) {
        case pugi::node_element:
            node = doc.append_child(pugi::node_element);
            break;
        case pugi::node_pcdata:
            node = doc.append_child(pugi::node_pcdata);
            break;
        case pugi::node_cdata:
            node = doc.append_child(pugi::node_cdata);
            break;
        case pugi::node_comment:
            node = doc.append_child(pugi::node_comment);
            break;
        case pugi::node_pi:
            node = doc.append_child(pugi::node_pi);
            break;
        case pugi::node_declaration:
            node = doc.append_child(pugi::node_declaration);
            break;
        case pugi::node_doctype:
            node = doc.append_child(pugi::node_doctype);
            break;
        case pugi::node_document:
            node = doc;
            break;
        case pugi::node_null:
        default:
            // Use default empty node
            break;
    }
    
    // Prepare string data for set_name and set_value
    std::string input_string;
    if (string_size > 0) {
        input_string.assign(reinterpret_cast<const char*>(string_data), 
                           std::min(string_size, static_cast<size_t>(1000)));
    } else {
        input_string = "test";
    }
    
    // Test set_name
    if (!node.empty()) {
        node.set_name(input_string.c_str(), input_string.size());
    }
    
    // Test set_value overloads
    if (!node.empty()) {
        // First overload with size parameter
        node.set_value(input_string.c_str(), input_string.size());
        
        // Second overload without size parameter
        // Ensure null termination for C-string overload
        std::string null_term_string = input_string;
        node.set_value(null_term_string.c_str());
    }
    
    // Test getter functions
    static auto test_getters = [](pugi::xml_node n) {
        // Test value() getter
        const char* node_value = n.value();
        (void)node_value; // Prevent unused variable warning
        
        // Test name() getter
        const char* node_name = n.name();
        (void)node_name; // Prevent unused variable warning
        
        // Test type() getter
        pugi::xml_node_type node_type = n.type();
        (void)node_type; // Prevent unused variable warning
    };
    
    test_getters(node);
    
    // Also test with empty node
    pugi::xml_node empty_node;
    test_getters(empty_node);
    
    // Test with child nodes
    if (node.type() == pugi::node_element) {
        auto child = node.append_child(pugi::node_pcdata);
        child.set_value("child_data");
        test_getters(child);
    }
    
    return 0;
}