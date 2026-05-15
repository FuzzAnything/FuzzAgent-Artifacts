// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_uint at pugixml.cpp:6990:38 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_float at pugixml.cpp:7006:31 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_double at pugixml.cpp:6998:32 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_ullong at pugixml.cpp:7031:44 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7134:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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

static void test_xml_text_functions(pugi::xml_text& text, unsigned long long set_value) {
    // Test as_uint with default and custom default value
    text.as_uint();
    text.as_uint(42);
    
    // Test as_float with default and custom default value
    text.as_float();
    text.as_float(3.14f);
    
    // Test as_double with default and custom default value
    text.as_double();
    text.as_double(2.71828);
    
    // Test as_ullong with default and custom default value
    text.as_ullong();
    text.as_ullong(123456789ULL);
    
    // Test as_bool with default and custom default value
    text.as_bool();
    text.as_bool(true);
    
    // Test set with unsigned long long
    text.set(set_value);
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document
    pugi::xml_document doc;
    
    // Create a node with some name
    pugi::xml_node node = doc.append_child("test_node");
    
    // Create xml_text object
    pugi::xml_text text = node.text();
    
    // Extract a value for set() from the fuzzer input
    unsigned long long set_value = 0;
    if (Size >= sizeof(unsigned long long)) {
        // Use first 8 bytes for the set value
        set_value = *reinterpret_cast<const unsigned long long*>(Data);
    } else {
        // Use available bytes
        for (size_t i = 0; i < Size; ++i) {
            set_value = (set_value << 8) | Data[i];
        }
    }
    
    // Test with empty text (default constructed state)
    pugi::xml_text empty_text;
    test_xml_text_functions(empty_text, set_value);
    
    // Create another node
    pugi::xml_node data_node = doc.append_child("data_node");
    
    // Create a null-terminated string from fuzzer data for safe use
    // We need to ensure the string is null-terminated to avoid buffer overflows
    std::vector<char> safe_data(Size + 1);
    if (Size > 0) {
        memcpy(safe_data.data(), Data, Size);
    }
    safe_data[Size] = '\0';  // Ensure null termination
    
    // Set text using the safe null-terminated string
    data_node.text().set(safe_data.data());
    
    // Get the text object and test functions
    pugi::xml_text data_text = data_node.text();
    test_xml_text_functions(data_text, set_value);
    
    // Also test the original text object
    test_xml_text_functions(text, set_value);
    
    return 0;
}