// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* Data, size_t Size) {
    // Create a document that will stay alive for the entire function
    pugi::xml_document doc;
    
    // Create a node and attribute within the document
    pugi::xml_node node = doc.append_child("test");
    
    // Convert fuzz data to a string (handle potential null bytes)
    std::string attr_value(reinterpret_cast<const char*>(Data), Size);
    
    // Create an attribute and set its value
    pugi::xml_attribute attr = node.append_attribute("fuzz_attr");
    attr.set_value(attr_value.c_str());
    
    // Call the target API functions in the specified order
    // 1. as_string
    const char* str_result = attr.as_string("default_string");
    (void)str_result; // Prevent unused variable warning
    
    // 2. as_int
    int int_result = attr.as_int(42);
    (void)int_result;
    
    // 3. as_uint
    unsigned int uint_result = attr.as_uint(123u);
    (void)uint_result;
    
    // 4. as_double
    double double_result = attr.as_double(3.14159);
    (void)double_result;
    
    // 5. as_float
    float float_result = attr.as_float(2.718f);
    (void)float_result;
    
    // 6. as_bool
    bool bool_result = attr.as_bool(true);
    (void)bool_result;
    
    // 7. as_llong
    long long llong_result = attr.as_llong(999LL);
    (void)llong_result;
    
    // 8. as_ullong
    unsigned long long ullong_result = attr.as_ullong(888ULL);
    (void)ullong_result;
    
    // Also test with different default values to explore more states
    attr.as_string(nullptr);
    attr.as_int(-100);
    attr.as_uint(0xFFFFFFFFu);
    attr.as_double(-1.0e308);
    attr.as_float(-1.0e38f);
    attr.as_bool(false);
    attr.as_llong(-9223372036854775807LL);
    attr.as_ullong(18446744073709551615ULL);
    
    return 0;
}