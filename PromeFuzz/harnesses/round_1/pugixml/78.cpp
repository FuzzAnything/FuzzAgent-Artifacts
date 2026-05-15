// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6865:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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
#include <sstream>
#include <iostream>

static void test_xml_node_functions(pugi::xml_node& node, const std::string& name_str, const std::string& value_str) {
    // Test set_name
    bool set_name_result = node.set_name(name_str.c_str());
    (void)set_name_result; // Avoid unused variable warning
    
    // Test set_value
    bool set_value_result = node.set_value(value_str.c_str());
    (void)set_value_result; // Avoid unused variable warning
    
    // Test value
    const char* node_value = node.value();
    (void)node_value; // Avoid unused variable warning
    
    // Test name
    const char* node_name = node.name();
    (void)node_name; // Avoid unused variable warning
    
    // Test type
    pugi::xml_node_type node_type = node.type();
    (void)node_type; // Avoid unused variable warning
    
    // Test print with wide stream
    std::wstringstream wss;
    node.print(wss, PUGIXML_TEXT("\t"), pugi::format_default, 0);
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    // Split input into two parts for name and value
    size_t split_point = Size / 2;
    std::string name_str(reinterpret_cast<const char*>(Data), split_point);
    std::string value_str(reinterpret_cast<const char*>(Data + split_point), Size - split_point);
    
    // Create a document and different node types to test
    pugi::xml_document doc;
    
    // Test with element node
    pugi::xml_node element_node = doc.append_child(pugi::node_element);
    test_xml_node_functions(element_node, name_str, value_str);
    
    // Test with comment node
    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    test_xml_node_functions(comment_node, name_str, value_str);
    
    // Test with processing instruction node
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    test_xml_node_functions(pi_node, name_str, value_str);
    
    // Test with declaration node
    pugi::xml_node decl_node = doc.append_child(pugi::node_declaration);
    test_xml_node_functions(decl_node, name_str, value_str);
    
    // Test with doctype node
    pugi::xml_node doctype_node = doc.append_child(pugi::node_doctype);
    test_xml_node_functions(doctype_node, name_str, value_str);
    
    // Test with PCDATA node
    pugi::xml_node pcdata_node = doc.append_child(pugi::node_pcdata);
    test_xml_node_functions(pcdata_node, name_str, value_str);
    
    // Test with CDATA node
    pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
    test_xml_node_functions(cdata_node, name_str, value_str);
    
    // Test with null node
    pugi::xml_node null_node;
    test_xml_node_functions(null_node, name_str, value_str);
    
    return 0;
}