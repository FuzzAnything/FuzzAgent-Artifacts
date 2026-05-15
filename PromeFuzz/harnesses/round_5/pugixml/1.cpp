// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create an empty xml_text object
    pugi::xml_text text;
    
    // Call all the conversion functions in the specified order
    // Each function will use its default value since the xml_text is empty
    const char* str_result = text.as_string();
    int int_result = text.as_int();
    unsigned int uint_result = text.as_uint();
    double double_result = text.as_double();
    float float_result = text.as_float();
    bool bool_result = text.as_bool();
    long long llong_result = text.as_llong();
    unsigned long long ullong_result = text.as_ullong();
    
    // The results are not used, but we call the functions to exercise them
    (void)str_result;
    (void)int_result;
    (void)uint_result;
    (void)double_result;
    (void)float_result;
    (void)bool_result;
    (void)llong_result;
    (void)ullong_result;
    
    return 0;
}