// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_ullong at pugixml.cpp:5352:49 in pugixml.hpp
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
#include "src/pugixml.hpp"

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create an XML document with an attribute
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    pugi::xml_attribute attr = node.append_attribute("test_attr");
    
    // Set attribute value from fuzzer input
    // Use the first Size bytes of Data as the attribute value
    // If Size is 0, the attribute will be empty (which is also a valid test case)
    if (Size > 0) {
        attr.set_value(reinterpret_cast<const char*>(Data), Size);
    }
    
    // Call the target API functions in the specified order
    // We'll call each function twice: once with default parameter and once with explicit value
    // to explore more program states
    
    // 1. as_string
    const char* str_default = attr.as_string();
    const char* str_explicit = attr.as_string("default_string");
    
    // 2. as_int
    int int_default = attr.as_int();
    int int_explicit = attr.as_int(42);
    
    // 3. as_uint
    unsigned int uint_default = attr.as_uint();
    unsigned int uint_explicit = attr.as_uint(100u);
    
    // 4. as_double
    double double_default = attr.as_double();
    double double_explicit = attr.as_double(3.14159);
    
    // 5. as_float
    float float_default = attr.as_float();
    float float_explicit = attr.as_float(2.71828f);
    
    // 6. as_bool
    bool bool_default = attr.as_bool();
    bool bool_explicit = attr.as_bool(true);
    
    // 7. as_llong
    long long llong_default = attr.as_llong();
    long long llong_explicit = attr.as_llong(1234567890123LL);
    
    // 8. as_ullong
    unsigned long long ullong_default = attr.as_ullong();
    unsigned long long ullong_explicit = attr.as_ullong(9876543210987ULL);
    
    // Force the compiler to consider all return values as used
    // to prevent optimization from removing our function calls
    static volatile auto force_use = str_default;
    force_use = str_explicit;
    static volatile int force_use_int = int_default + int_explicit;
    static volatile unsigned int force_use_uint = uint_default + uint_explicit;
    static volatile double force_use_double = double_default + double_explicit;
    static volatile float force_use_float = float_default + float_explicit;
    static volatile bool force_use_bool = bool_default || bool_explicit;
    static volatile long long force_use_llong = llong_default + llong_explicit;
    static volatile unsigned long long force_use_ullong = ullong_default + ullong_explicit;
    
    return 0;
}