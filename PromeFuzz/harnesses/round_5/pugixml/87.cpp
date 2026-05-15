// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
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
#include <fstream>

static void test_attribute_functions(pugi::xml_attribute& attr) {
    // Test as_ullong with default value
    unsigned long long ull_val = attr.as_ullong();
    unsigned long long ull_val_def = attr.as_ullong(42);
    
    // Test as_double with default value
    double d_val = attr.as_double();
    double d_val_def = attr.as_double(3.14);
    
    // Test empty
    bool is_empty = attr.empty();
    
    // Test as_bool with default value
    bool b_val = attr.as_bool();
    bool b_val_def = attr.as_bool(true);
    
    // Test set_value overloads
    bool set_bool_result = attr.set_value(true);
    bool set_double_result = attr.set_value(2.718);
    
    // Use the results to avoid compiler optimizations
    (void)ull_val;
    (void)ull_val_def;
    (void)d_val;
    (void)d_val_def;
    (void)is_empty;
    (void)b_val;
    (void)b_val_def;
    (void)set_bool_result;
    (void)set_double_result;
}

static pugi::xml_attribute create_test_attribute(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Create attribute with name "attr"
    pugi::xml_attribute attr = node.append_attribute("attr");
    
    // Set attribute value from fuzzer data (truncate if too long)
    if (size > 0) {
        size_t value_len = size < 100 ? size : 100;
        std::string value_str(reinterpret_cast<const char*>(data), value_len);
        attr.set_value(value_str.c_str());
    } else {
        attr.set_value("default");
    }
    
    return attr;
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* data, size_t size) {
    // Create test attribute from fuzzer data
    pugi::xml_attribute attr = create_test_attribute(data, size);
    
    // Test all API functions
    test_attribute_functions(attr);
    
    // Additional test: create empty attribute and test empty()
    pugi::xml_document doc2;
    pugi::xml_node node2 = doc2.append_child("node2");
    pugi::xml_attribute empty_attr = node2.attribute("nonexistent");
    bool empty_check = empty_attr.empty();
    (void)empty_check;
    
    // Test with different default values
    unsigned long long ull_def = attr.as_ullong(999);
    double d_def = attr.as_double(1.414);
    bool b_def = attr.as_bool(false);
    (void)ull_def;
    (void)d_def;
    (void)b_def;
    
    // Test set_value with various inputs
    pugi::xml_document doc3;
    pugi::xml_node node3 = doc3.append_child("node3");
    pugi::xml_attribute attr3 = node3.append_attribute("attr3");
    
    // Test bool set_value with different values
    attr3.set_value(true);
    attr3.set_value(false);
    
    // Test double set_value with different values
    attr3.set_value(0.0);
    attr3.set_value(-1.5);
    attr3.set_value(100.0);
    
    return 0;
}