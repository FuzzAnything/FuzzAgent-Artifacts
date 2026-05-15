// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7127:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_int at pugixml.cpp:6982:29 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_llong at pugixml.cpp:7023:35 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    // Create XML document and text objects
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    pugi::xml_text text = node.text();
    
    // Test as_llong with various defaults
    text.as_llong();
    text.as_llong(42);
    text.as_llong(-100);
    
    // Test as_uint with various defaults
    text.as_uint();
    text.as_uint(100);
    text.as_uint(0xFFFFFFFF);
    
    // Test as_int with various defaults
    text.as_int();
    text.as_int(-42);
    text.as_int(1000);
    
    // Test as_double with various defaults
    text.as_double();
    text.as_double(3.14159);
    text.as_double(-2.71828);
    
    // Test as_bool with various defaults
    text.as_bool();
    text.as_bool(true);
    text.as_bool(false);
    
    // Test set with long long
    if (Size >= sizeof(long long)) {
        long long value;
        // Copy enough bytes from input to form a long long
        // Use memcpy to avoid alignment issues
        std::memcpy(&value, Data, sizeof(long long));
        text.set(value);
    } else {
        // Use default values if input is too small
        text.set(0LL);
        text.set(12345LL);
        text.set(-67890LL);
    }
    
    // Test set with string_view
    if (Size > 0) {
        std::string_view sv(reinterpret_cast<const char*>(Data), Size);
        // Create temporary xml_text object for string_view set
        pugi::xml_text temp_text = node.text();
        // Note: pugixml's xml_text::set for string_view_t uses pugi::string_t
        // which is typically char_t* in pugixml, not std::string_view
        // We'll use the set(const char_t*) overload instead
        if (Size < 100) { // Limit size to avoid excessive memory usage
            char buffer[100];
            std::memcpy(buffer, Data, Size);
            buffer[Size] = '\0';
            temp_text.set(buffer);
        }
    }
    
    // Create empty xml_text to test default behaviors
    pugi::xml_text empty_text;
    empty_text.as_llong(999);
    empty_text.as_uint(777);
    empty_text.as_int(-555);
    empty_text.as_double(1.234);
    empty_text.as_bool(true);
    
    // Test with different text content
    node.text().set("123");
    text.as_llong();
    text.as_uint();
    text.as_int();
    text.as_double();
    text.as_bool();
    
    node.text().set("true");
    text.as_bool();
    
    node.text().set("3.14");
    text.as_double();
    
    node.text().set("-999");
    text.as_llong();
    text.as_int();
    
    // Test with empty string
    node.text().set("");
    text.as_llong(111);
    text.as_uint(222);
    text.as_int(333);
    text.as_double(4.56);
    text.as_bool(true);
    
    return 0;
}