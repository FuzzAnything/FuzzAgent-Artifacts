// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    // Create XML document and parse input
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // If parsing failed, create a simple document to test the functions
    if (!result) {
        // Create a simple XML structure to test removal functions
        const char* xml_content = R"(<root attr1="value1" attr2="value2"><child1/><child2 name="test"><subchild/></child2></root>)";
        doc.load_string(xml_content);
    }
    
    // Get the root node or first node
    pugi::xml_node node = doc.first_child();
    if (!node) {
        // If no node exists, create one
        node = doc.append_child("test_node");
        node.append_attribute("attr1") = "value1";
        node.append_attribute("attr2") = "value2";
        node.append_child("child1");
        node.append_child("child2");
    }
    
    // Test remove_attributes()
    node.remove_attributes();
    
    // Test remove_children()
    node.remove_children();
    
    // Re-add some attributes and children for further testing
    node.append_attribute("test_attr1") = "value1";
    node.append_attribute("test_attr2") = "value2";
    node.append_child("child1");
    node.append_child("child2");
    node.append_child("child3");
    
    // Test remove_attribute() overloads
    if (Size > 0) {
        // Create string_view from fuzz data
        std::string_view attr_name(reinterpret_cast<const char*>(Data), 
                                  Size > 100 ? 100 : Size); // Limit size
        
        // Test string_view_t overload
        node.remove_attribute(attr_name);
        
        // Test const char_t* overload
        // Create null-terminated string from fuzz data
        size_t name_len = Size > 50 ? 50 : Size;
        char* attr_name_cstr = new char[name_len + 1];
        if (name_len > 0) {
            std::memcpy(attr_name_cstr, Data, name_len);
        }
        attr_name_cstr[name_len] = '\0';
        
        node.remove_attribute(attr_name_cstr);
        delete[] attr_name_cstr;
    }
    
    // Test remove_child() overloads
    if (Size > 0) {
        // Create string_view from fuzz data (use different portion)
        size_t offset = Size > 10 ? 10 : 0;
        size_t view_size = (Size - offset) > 100 ? 100 : (Size - offset);
        if (view_size > 0) {
            std::string_view child_name(reinterpret_cast<const char*>(Data + offset), view_size);
            
            // Test string_view_t overload
            node.remove_child(child_name);
            
            // Test const char_t* overload
            // Create null-terminated string from fuzz data
            size_t name_len = view_size > 50 ? 50 : view_size;
            char* child_name_cstr = new char[name_len + 1];
            if (name_len > 0) {
                std::memcpy(child_name_cstr, Data + offset, name_len);
            }
            child_name_cstr[name_len] = '\0';
            
            node.remove_child(child_name_cstr);
            delete[] child_name_cstr;
        }
    }
    
    // Test removal on empty node
    pugi::xml_node empty_node;
    empty_node.remove_attributes();
    empty_node.remove_children();
    
    if (Size > 0) {
        std::string_view dummy_name("dummy");
        empty_node.remove_attribute(dummy_name);
        empty_node.remove_child(dummy_name);
    }
    
    return 0;
}