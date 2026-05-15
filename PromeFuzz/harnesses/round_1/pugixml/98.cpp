// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
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
#include <string_view>

static void test_text_functions(pugi::xml_text& text) {
    // Test as_llong with various default values
    text.as_llong();
    text.as_llong(42);
    text.as_llong(-100);
    
    // Test as_uint with various default values
    text.as_uint();
    text.as_uint(100);
    text.as_uint(0xFFFFFFFF);
    
    // Test as_int with various default values
    text.as_int();
    text.as_int(-50);
    text.as_int(2147483647);
    
    // Test as_double with various default values
    text.as_double();
    text.as_double(3.14159);
    text.as_double(-2.71828);
    
    // Test as_bool with various default values
    text.as_bool();
    text.as_bool(true);
    text.as_bool(false);
    
    // Test set with various long long values
    text.set(0LL);
    text.set(1234567890LL);
    text.set(-987654321LL);
}

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Create a node with text content from fuzzer input
    pugi::xml_node node = doc.append_child("test_node");
    
    if (Size > 0) {
        // Use fuzzer data as text content (truncate if needed for null termination)
        size_t copy_size = Size < 1024 ? Size : 1024;
        char* buffer = new char[copy_size + 1];
        memcpy(buffer, Data, copy_size);
        buffer[copy_size] = '\0';
        
        node.append_child(pugi::node_pcdata).set_value(buffer);
        delete[] buffer;
    } else {
        node.append_child(pugi::node_pcdata).set_value("");
    }
    
    // Get xml_text object from the node
    pugi::xml_text text = node.text();
    
    // Test all the target functions
    test_text_functions(text);
    
    // Additional test: create empty xml_text and test functions
    pugi::xml_text empty_text;
    test_text_functions(empty_text);
    
    // Additional test: create another node with different content
    pugi::xml_node node2 = doc.append_child("node2");
    
    // Test with string_view for set (if available)
    // Note: pugi::xml_text::set has overloads including one for string_view_t
    // which is typically pugi::string_t or std::string_view depending on configuration
    if (Size > 0) {
        std::string_view sv(reinterpret_cast<const char*>(Data), 
                           Size > 100 ? 100 : Size);
        // Use the overload that accepts both data pointer and size
        node2.append_child(pugi::node_pcdata).set_value(sv.data(), sv.size());
        
        pugi::xml_text text2 = node2.text();
        test_text_functions(text2);
    }
    
    return 0;
}