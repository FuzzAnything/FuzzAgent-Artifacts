// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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
    // Create xml_text object
    pugi::xml_text text;
    
    // Create a dummy XML document with a node containing text from fuzzer input
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test");
    
    // Convert fuzzer data to string (handle potential null bytes)
    const char* text_data = reinterpret_cast<const char*>(Data);
    std::string content(text_data, Size);
    
    // Set node text
    node.text().set(content.c_str());
    
    // Get the xml_text object
    text = node.text();
    
    // Call the target API functions in specified order
    // We ignore return values since we're just testing for crashes
    static volatile auto unused = [] {
        // This lambda ensures we capture all return values without optimization
        return 0;
    }();
    
    // 1. as_string
    const char* str_result = text.as_string();
    unused = (uintptr_t)str_result;
    
    // 2. as_int
    int int_result = text.as_int();
    unused = int_result;
    
    // 3. as_uint
    unsigned int uint_result = text.as_uint();
    unused = uint_result;
    
    // 4. as_double
    double double_result = text.as_double();
    unused = (int)double_result;
    
    // 5. as_float
    float float_result = text.as_float();
    unused = (int)float_result;
    
    // 6. as_bool
    bool bool_result = text.as_bool();
    unused = bool_result;
    
    // 7. as_llong
    long long llong_result = text.as_llong();
    unused = (int)llong_result;
    
    // 8. as_ullong
    unsigned long long ullong_result = text.as_ullong();
    unused = (int)ullong_result;
    
    return 0;
}