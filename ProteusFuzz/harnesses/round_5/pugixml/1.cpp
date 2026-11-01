// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <pugixml.hpp>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated to prevent buffer overflows
    // when passed to load_string which expects a C-string.
    // We create a temporary buffer with a null terminator.
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());

    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.first_child();
    if (!root) {
        return 0;
    }

    pugi::xml_attribute attr = root.attribute("test");

    // Invoke as_string
    const pugi::char_t* str_val = attr.as_string(PUGIXML_TEXT("default"));
    (void)str_val;

    // Invoke as_int
    int int_val = attr.as_int(0);
    (void)int_val;

    // Invoke as_uint
    unsigned int uint_val = attr.as_uint(0);
    (void)uint_val;

    // Invoke as_double
    double double_val = attr.as_double(0.0);
    (void)double_val;

    // Invoke as_float
    float float_val = attr.as_float(0.0f);
    (void)float_val;

    // Invoke as_bool
    bool bool_val = attr.as_bool(false);
    (void)bool_val;

    // Invoke as_llong
    long long llong_val = attr.as_llong(0);
    (void)llong_val;

    // Invoke as_ullong
    unsigned long long ullong_val = attr.as_ullong(0);
    (void)ullong_val;

    return 0;
}