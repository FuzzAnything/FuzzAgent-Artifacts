// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
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

static std::string_view extract_string_view(const uint8_t* data, size_t size) {
    if (size == 0) {
        return std::string_view("");
    }
    size_t len = size < 100 ? size : 100;
    const char* str = reinterpret_cast<const char*>(data);
    return std::string_view(str, len);
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    
    std::string_view child_name = extract_string_view(Data, Size);
    std::string child_name_null_term(child_name);
    
    pugi::xml_node child_node = root.child(child_name_null_term.c_str());
    
    pugi::xml_attribute first_attr = child_node.first_attribute();
    
    pugi::xml_node first_child = child_node.first_child();
    
    pugi::xml_node_struct* node_struct1 = child_node.internal_object();
    
    pugi::xml_attribute_struct* attr_struct1 = first_attr.internal_object();
    
    pugi::xml_node_struct* node_struct2 = child_node.internal_object();
    
    pugi::xml_node_struct* node_struct3 = child_node.internal_object();
    
    pugi::xml_node_struct* node_struct4 = child_node.internal_object();
    
    pugi::xml_attribute_struct* attr_struct2 = first_attr.internal_object();
    
    pugi::xml_node_struct* node_struct5 = child_node.internal_object();
    
    pugi::xml_attribute_struct* attr_struct3 = first_attr.internal_object();

    (void)node_struct1;
    (void)attr_struct1;
    (void)node_struct2;
    (void)node_struct3;
    (void)node_struct4;
    (void)attr_struct2;
    (void)node_struct5;
    (void)attr_struct3;

    return 0;
}