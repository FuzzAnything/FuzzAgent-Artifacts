// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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

static void test_attribute_conversions(pugi::xml_attribute& attr) {
    // Call all conversion functions in the specified order
    attr.as_string();
    attr.as_int();
    attr.as_uint();
    attr.as_double();
    attr.as_float();
    attr.as_bool();
    attr.as_llong();
    attr.as_ullong();
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create an XML document and populate it with fuzzer data
    pugi::xml_document doc;
    
    // Create a root node
    pugi::xml_node root = doc.append_child("root");
    
    // Create an attribute with fuzzer data as value
    pugi::xml_attribute attr = root.append_attribute("test");
    
    // Set attribute value from fuzzer input
    if (Size > 0) {
        // Convert fuzzer data to null-terminated string
        // Use a buffer to ensure null termination
        char* buffer = new char[Size + 1];
        if (buffer) {
            for (size_t i = 0; i < Size; ++i) {
                buffer[i] = static_cast<char>(Data[i] % 128); // Keep as printable ASCII
            }
            buffer[Size] = '\0';
            
            attr.set_value(buffer);
            delete[] buffer;
        }
    } else {
        attr.set_value("");
    }
    
    // Test the attribute conversion functions
    test_attribute_conversions(attr);
    
    // Also test with empty attribute
    pugi::xml_attribute empty_attr;
    test_attribute_conversions(empty_attr);
    
    // Test with another attribute with different value
    pugi::xml_attribute attr2 = root.append_attribute("test2");
    attr2.set_value("123");
    test_attribute_conversions(attr2);
    
    // Test with boolean-like values
    pugi::xml_attribute attr3 = root.append_attribute("test3");
    attr3.set_value("true");
    test_attribute_conversions(attr3);
    
    pugi::xml_attribute attr4 = root.append_attribute("test4");
    attr4.set_value("1");
    test_attribute_conversions(attr4);
    
    pugi::xml_attribute attr5 = root.append_attribute("test5");
    attr5.set_value("3.14");
    test_attribute_conversions(attr5);
    
    return 0;
}