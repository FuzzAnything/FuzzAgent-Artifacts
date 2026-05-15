// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
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

static pugi::xml_node create_test_node(pugi::xml_node_type type) {
    static pugi::xml_document doc;
    return doc.append_child(type);
}

static pugi::xml_attribute create_test_attribute() {
    static pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    return node.append_attribute("initial_name");
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from fuzzer input
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test pugi::xml_node::set_value
    {
        // Test on different node types that support set_value
        pugi::xml_node_type node_types[] = {
            pugi::node_pcdata,
            pugi::node_cdata,
            pugi::node_comment,
            pugi::node_pi,
            pugi::node_doctype
        };
        
        for (auto type : node_types) {
            pugi::xml_node node = create_test_node(type);
            std::string_view sv(input_str);
            node.set_value(sv);
        }
        
        // Also test on unsupported node type
        pugi::xml_node element_node = create_test_node(pugi::node_element);
        std::string_view sv_unsupported(input_str);
        element_node.set_value(sv_unsupported);
    }
    
    // Test pugi::xml_node::set_name
    {
        // Test on element node (should work)
        pugi::xml_node element_node = create_test_node(pugi::node_element);
        std::string_view sv1(input_str);
        element_node.set_name(sv1);
        
        // Test on PI node (should work)
        pugi::xml_node pi_node = create_test_node(pugi::node_pi);
        std::string_view sv2(input_str);
        pi_node.set_name(sv2);
        
        // Test on declaration node (should work)
        pugi::xml_node decl_node = create_test_node(pugi::node_declaration);
        std::string_view sv3(input_str);
        decl_node.set_name(sv3);
        
        // Test on empty/invalid node
        pugi::xml_node empty_node;
        std::string_view sv4(input_str);
        empty_node.set_name(sv4);
    }
    
    // Test pugi::xml_attribute::set_value (both overloads)
    {
        pugi::xml_attribute attr = create_test_attribute();
        
        // String overload
        std::string_view sv_str(input_str);
        attr.set_value(sv_str);
        
        // Bool overload - use first byte to determine bool value
        bool bool_val = (Data[0] & 1) != 0;
        attr.set_value(bool_val);
        
        // Test on empty attribute
        pugi::xml_attribute empty_attr;
        std::string_view sv_empty(input_str);
        empty_attr.set_value(sv_empty);
    }
    
    // Test pugi::xml_attribute::set_name (both overloads)
    {
        pugi::xml_attribute attr = create_test_attribute();
        
        // string_view_t overload
        std::string_view sv1(input_str);
        attr.set_name(sv1);
        
        // const char_t* overload
        const pugi::char_t* cstr = reinterpret_cast<const pugi::char_t*>(input_str.c_str());
        attr.set_name(cstr);
        
        // Test on empty attribute
        pugi::xml_attribute empty_attr;
        std::string_view sv2(input_str);
        empty_attr.set_name(sv2);
    }
    
    return 0;
}