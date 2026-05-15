// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6858:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document and node for testing
    pugi::xml_document doc;
    pugi::xml_node node;
    
    // Use part of the fuzzer input to determine node type
    size_t type_selector = Data[0] % 9; // 9 node types in xml_node_type
    pugi::xml_node_type node_type = static_cast<pugi::xml_node_type>(type_selector);
    
    // Create appropriate node type
    switch (node_type) {
        case pugi::node_document:
            node = doc;
            break;
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
        case pugi::node_null:
        default:
            // Use default empty node
            break;
    }

    // Prepare string data from fuzzer input
    std::string input_str;
    if (Size > 1) {
        // Use remaining data for string operations
        input_str.assign(reinterpret_cast<const char*>(Data + 1), 
                        std::min(Size - 1, static_cast<size_t>(1000)));
    } else {
        input_str = "test";
    }

    // Ensure null termination for C-string APIs
    std::string null_terminated_str = input_str + '\0';
    const char* cstr_data = null_terminated_str.c_str();

    // Test set_name
    if (!node.empty()) {
        bool name_result = node.set_name(cstr_data);
        (void)name_result; // Ignore return value for fuzzing
    }

    // Test set_value
    if (!node.empty()) {
        bool value_result = node.set_value(cstr_data);
        (void)value_result; // Ignore return value for fuzzing
    }

    // Test value() function
    if (!node.empty()) {
        const char* node_value = node.value();
        (void)node_value; // Use result to avoid unused variable warning
    }

    // Test name() function
    if (!node.empty()) {
        const char* node_name = node.name();
        (void)node_name; // Use result to avoid unused variable warning
    }

    // Test type() function
    if (!node.empty()) {
        pugi::xml_node_type node_type_result = node.type();
        (void)node_type_result; // Use result to avoid unused variable warning
    }

    // Test print() function
    if (!node.empty()) {
        try {
            std::stringstream ss;
            // Use different encoding options based on input
            pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(
                (Data[0] % 10) % 9); // 9 encoding types
            unsigned int flags = Data[0] % 256;
            
            node.print(ss, "\t", flags, encoding, 0);
            
            // Also test with different indent strings
            std::stringstream ss2;
            const char* indent = (Data[0] % 2 == 0) ? "  " : "\t";
            node.print(ss2, indent, flags, pugi::encoding_auto, 0);
        } catch (...) {
            // Ignore exceptions during fuzzing
        }
    }

    // Additional test: Create another node and test operations
    if (Size > 10) {
        pugi::xml_node another_node = doc.append_child(pugi::node_element);
        if (!another_node.empty()) {
            // Use different portion of input for this node
            size_t offset = std::min(static_cast<size_t>(10), Size - 1);
            std::string another_str(reinterpret_cast<const char*>(Data + offset),
                                   std::min(Size - offset, static_cast<size_t>(500)));
            another_str += '\0';
            
            another_node.set_name(another_str.c_str());
            another_node.set_value(another_str.c_str());
            
            // Test print with this node
            std::stringstream ss3;
            another_node.print(ss3, "  ", pugi::format_default, pugi::encoding_utf8, 0);
        }
    }

    return 0;
}