// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create an XML document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test");
    
    // Create text node with fuzzer data
    // Convert Data to a string (treat as char array, handle null bytes)
    std::string text_content(reinterpret_cast<const char*>(Data), Size);
    node.append_child(pugi::node_pcdata).set_value(text_content.c_str());
    
    // Get xml_text object from the node
    pugi::xml_text text = node.text();
    
    // Call the API functions in the specified order
    // 1. as_string
    const char* str_result = text.as_string("default");
    (void)str_result; // Prevent unused variable warning
    
    // 2. as_int
    int int_result = text.as_int(0);
    (void)int_result;
    
    // 3. as_uint
    unsigned int uint_result = text.as_uint(0u);
    (void)uint_result;
    
    // 4. as_double
    double double_result = text.as_double(0.0);
    (void)double_result;
    
    // 5. as_float
    float float_result = text.as_float(0.0f);
    (void)float_result;
    
    // 6. as_bool
    bool bool_result = text.as_bool(false);
    (void)bool_result;
    
    // 7. as_llong
    long long llong_result = text.as_llong(0LL);
    (void)llong_result;
    
    // 8. as_ullong
    unsigned long long ullong_result = text.as_ullong(0ULL);
    (void)ullong_result;
    
    return 0;
}