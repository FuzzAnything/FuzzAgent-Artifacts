// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string_view>

static void fuzz_xml_attribute_set_value(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a valid document and node to attach an attribute
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Append an attribute to the node
    pugi::xml_attribute test_attr = root.append_attribute("test");
    
    if (!test_attr) return;
    
    // Test with string_view_t (std::string_view)
    std::string_view str_view(reinterpret_cast<const char*>(data), size);
    test_attr.set_value(str_view);
    
    // Test with integer conversion
    int int_val = 0;
    if (size >= sizeof(int)) {
        int_val = *reinterpret_cast<const int*>(data);
    }
    test_attr.set_value(int_val);
    
    // Test with double conversion
    double double_val = 0.0;
    if (size >= sizeof(double)) {
        double_val = *reinterpret_cast<const double*>(data);
    }
    test_attr.set_value(double_val);
    
    // Test with boolean
    bool bool_val = (size > 0 && data[0] != 0);
    test_attr.set_value(bool_val);
}

static void fuzz_xml_node_set_value(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a valid document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("root");
    
    if (!node) return;
    
    // Test set_value with string_view_t
    std::string_view str_view(reinterpret_cast<const char*>(data), size);
    node.set_value(str_view);
    
    // Test set_value with const char_t*
    // Ensure null termination for char_t* overload
    std::string str_data(reinterpret_cast<const char*>(data), size);
    node.set_value(str_data.c_str());
}

static void fuzz_xml_node_set_name(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a valid document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("root");
    
    if (!node) return;
    
    // Test set_name with string_view_t
    std::string_view str_view(reinterpret_cast<const char*>(data), size);
    node.set_name(str_view);
}

static void fuzz_xml_text_set(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create a valid document and node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_text text = root.text();
    
    if (!text) return;
    
    // Test set with string_view_t
    std::string_view str_view(reinterpret_cast<const char*>(data), size);
    text.set(str_view);
    
    // Test set with bool
    bool bool_val = (size > 0 && data[0] != 0);
    text.set(bool_val);
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz xml_attribute::set_value
    fuzz_xml_attribute_set_value(Data, Size);

    // Fuzz xml_node::set_value
    fuzz_xml_node_set_value(Data, Size);

    // Fuzz xml_node::set_name
    fuzz_xml_node_set_name(Data, Size);

    // Fuzz xml_text::set
    fuzz_xml_text_set(Data, Size);

    return 0;
}