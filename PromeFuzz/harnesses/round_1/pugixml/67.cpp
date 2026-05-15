// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
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

static pugi::xml_node create_test_node(pugi::xml_node_type type) {
    static pugi::xml_document doc;
    return doc.append_child(type);
}

static pugi::xml_attribute create_test_attribute() {
    static pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    return node.append_attribute("dummy");
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create string view from fuzzer input
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test pugi::xml_node::set_value
    {
        // Test on supported node types
        pugi::xml_node_type supported_types[] = {
            pugi::node_pcdata,
            pugi::node_cdata,
            pugi::node_comment,
            pugi::node_pi,
            pugi::node_doctype
        };
        
        for (auto type : supported_types) {
            pugi::xml_node node = create_test_node(type);
            node.set_value(input_str);
        }
        
        // Test on unsupported node types
        pugi::xml_node_type unsupported_types[] = {
            pugi::node_element,
            pugi::node_document
        };
        
        for (auto type : unsupported_types) {
            pugi::xml_node node = create_test_node(type);
            node.set_value(input_str);
        }
    }
    
    // Test pugi::xml_node::set_name
    {
        // Test on element node (should work)
        pugi::xml_node element = create_test_node(pugi::node_element);
        element.set_name(input_str);
        
        // Test on PI node (should work)
        pugi::xml_node pi = create_test_node(pugi::node_pi);
        pi.set_name(input_str);
        
        // Test on empty/invalid node
        pugi::xml_node empty_node;
        empty_node.set_name(input_str);
    }
    
    // Test pugi::xml_attribute::set_value overloads
    {
        pugi::xml_attribute attr = create_test_attribute();
        
        // String overload
        attr.set_value(input_str);
        
        // Bool overload - use first byte to determine bool value
        bool bool_val = (Data[0] & 0x1) != 0;
        attr.set_value(bool_val);
        
        // Test on null attribute
        pugi::xml_attribute null_attr;
        null_attr.set_value(input_str);
        null_attr.set_value(bool_val);
    }
    
    // Test pugi::xml_attribute::set_name overloads
    {
        pugi::xml_attribute attr = create_test_attribute();
        
        // string_view_t overload
        attr.set_name(input_str);
        
        // const char_t* overload - need null-terminated string
        if (Size > 0) {
            // Create a null-terminated copy of the input
            char* null_term_str = new char[Size + 1];
            std::memcpy(null_term_str, Data, Size);
            null_term_str[Size] = '\0';
            
            attr.set_name(null_term_str);
            delete[] null_term_str;
        }
        
        // Test on empty attribute
        pugi::xml_attribute empty_attr;
        empty_attr.set_name(input_str);
        if (Size > 0) {
            char* null_term_str = new char[Size + 1];
            std::memcpy(null_term_str, Data, Size);
            null_term_str[Size] = '\0';
            
            empty_attr.set_name(null_term_str);
            delete[] null_term_str;
        }
    }
    
    return 0;
}