// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Split the input data for name and value operations
    size_t name_size = Size / 2;
    size_t value_size = Size - name_size;
    
    const char* name_data = reinterpret_cast<const char*>(Data);
    const char* value_data = reinterpret_cast<const char*>(Data + name_size);
    
    // Create XML document and nodes
    pugi::xml_document doc;
    
    // Create different types of nodes to test various scenarios
    pugi::xml_node element_node = doc.append_child(pugi::node_element);
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    pugi::xml_node declaration_node = doc.append_child(pugi::node_declaration);
    pugi::xml_node doctype_node = doc.append_child(pugi::node_doctype);
    pugi::xml_node pcdata_node = doc.append_child(pugi::node_pcdata);
    pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
    
    // Also test with empty node
    pugi::xml_node empty_node;
    
    // Test set_name on different nodes
    std::string_view name_str(name_data, name_size);
    element_node.set_name(name_str.data(), name_str.size());
    pi_node.set_name(name_str.data(), name_str.size());
    declaration_node.set_name(name_str.data(), name_str.size());
    
    // Test type() function
    (void)element_node.type();
    (void)comment_node.type();
    (void)pi_node.type();
    (void)declaration_node.type();
    (void)doctype_node.type();
    (void)pcdata_node.type();
    (void)cdata_node.type();
    (void)empty_node.type();
    
    // Test name() function
    (void)element_node.name();
    (void)pi_node.name();
    (void)declaration_node.name();
    (void)empty_node.name();
    
    // Test set_value with both overloads
    std::string_view value_str(value_data, value_size);
    
    // First overload: set_value(const char_t* rhs, size_t size)
    comment_node.set_value(value_str.data(), value_str.size());
    pi_node.set_value(value_str.data(), value_str.size());
    doctype_node.set_value(value_str.data(), value_str.size());
    pcdata_node.set_value(value_str.data(), value_str.size());
    cdata_node.set_value(value_str.data(), value_str.size());
    
    // Second overload: set_value(const char_t* rhs) - requires null termination
    // Create a null-terminated copy of the value data
    if (value_size > 0) {
        char* null_terminated_value = new char[value_size + 1];
        std::memcpy(null_terminated_value, value_data, value_size);
        null_terminated_value[value_size] = '\0';
        
        comment_node.set_value(null_terminated_value);
        pi_node.set_value(null_terminated_value);
        doctype_node.set_value(null_terminated_value);
        pcdata_node.set_value(null_terminated_value);
        cdata_node.set_value(null_terminated_value);
        
        delete[] null_terminated_value;
    }
    
    // Test value() function
    (void)comment_node.value();
    (void)pi_node.value();
    (void)doctype_node.value();
    (void)pcdata_node.value();
    (void)cdata_node.value();
    (void)empty_node.value();
    
    // Test set_name on empty node (should return false)
    element_node.set_name(nullptr, 0);
    empty_node.set_name(nullptr, 0);
    
    // Test set_value on element node (should return false for unsupported type)
    element_node.set_value(value_str.data(), value_str.size());
    
    return 0;
}