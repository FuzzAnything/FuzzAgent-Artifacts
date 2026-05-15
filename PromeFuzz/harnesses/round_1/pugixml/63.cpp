// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create string view from fuzzer input
    std::string_view str_view(reinterpret_cast<const char*>(Data), Size);
    
    // Create boolean value from first byte
    bool bool_val = Data[0] & 1;

    // Test xml_node functions
    {
        // Create document and nodes
        pugi::xml_document doc;
        
        // Test set_value on different node types
        pugi::xml_node node1 = doc.append_child(pugi::node_pcdata);
        node1.set_value(str_view);
        
        pugi::xml_node node2 = doc.append_child(pugi::node_cdata);
        node2.set_value(str_view);
        
        pugi::xml_node node3 = doc.append_child(pugi::node_comment);
        node3.set_value(str_view);
        
        pugi::xml_node node4 = doc.append_child(pugi::node_pi);
        node4.set_value(str_view);
        
        pugi::xml_node node5 = doc.append_child(pugi::node_doctype);
        node5.set_value(str_view);
        
        // Test set_name on different node types
        pugi::xml_node node6 = doc.append_child(pugi::node_element);
        node6.set_name(str_view);
        
        pugi::xml_node node7 = doc.append_child(pugi::node_pi);
        node7.set_name(str_view);
        
        pugi::xml_node node8 = doc.append_child(pugi::node_declaration);
        node8.set_name(str_view);
        
        // Test on empty/invalid node
        pugi::xml_node empty_node;
        empty_node.set_value(str_view);
        empty_node.set_name(str_view);
    }

    // Test xml_attribute functions
    {
        pugi::xml_document doc;
        pugi::xml_node node = doc.append_child(pugi::node_element);
        
        // Create attribute
        pugi::xml_attribute attr = node.append_attribute("initial_name");
        
        // Test set_value overloads
        attr.set_value(str_view);
        attr.set_value(bool_val);
        
        // Test set_name overloads
        attr.set_name(str_view);
        attr.set_name("constant_name");
        
        // Test on empty attribute
        pugi::xml_attribute empty_attr;
        empty_attr.set_value(str_view);
        empty_attr.set_value(bool_val);
        empty_attr.set_name(str_view);
        empty_attr.set_name("test_name");
    }

    return 0;
}