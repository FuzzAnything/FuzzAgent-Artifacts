// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create an xml_text object using default constructor
    pugi::xml_text text;
    
    // Call the functions in the specified order
    // as_string
    const pugi::char_t* str_result = text.as_string();
    const pugi::char_t* str_result_with_def = text.as_string(PUGIXML_TEXT("default"));
    
    // as_int
    int int_result = text.as_int();
    int int_result_with_def = text.as_int(42);
    
    // as_uint
    unsigned int uint_result = text.as_uint();
    unsigned int uint_result_with_def = text.as_uint(100);
    
    // as_double
    double double_result = text.as_double();
    double double_result_with_def = text.as_double(3.14);
    
    // as_float
    float float_result = text.as_float();
    float float_result_with_def = text.as_float(2.71f);
    
    // as_bool
    bool bool_result = text.as_bool();
    bool bool_result_with_def = text.as_bool(true);
    
    // as_llong
    long long llong_result = text.as_llong();
    long long llong_result_with_def = text.as_llong(1234567890LL);
    
    // as_ullong
    unsigned long long ullong_result = text.as_ullong();
    unsigned long long ullong_result_with_def = text.as_ullong(9876543210ULL);
    
    // Prevent unused variable warnings
    (void)str_result;
    (void)str_result_with_def;
    (void)int_result;
    (void)int_result_with_def;
    (void)uint_result;
    (void)uint_result_with_def;
    (void)double_result;
    (void)double_result_with_def;
    (void)float_result;
    (void)float_result_with_def;
    (void)bool_result;
    (void)bool_result_with_def;
    (void)llong_result;
    (void)llong_result_with_def;
    (void)ullong_result;
    (void)ullong_result_with_def;
    
    return 0;
}