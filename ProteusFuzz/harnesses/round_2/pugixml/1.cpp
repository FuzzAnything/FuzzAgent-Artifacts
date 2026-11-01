// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>

static void run_fuzz_test(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Convert input data to a C-string for parsing, ensuring null termination
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Ensure the string is null-terminated for pugi::xml_document::load_string
    xml_str += '\0';
    
    // Load the XML document
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    // Even if parsing fails, we can still test with empty attributes or traverse what we have
    pugi::xml_node root = doc.first_child();
    
    // If no root, try to create a dummy node structure to test attribute methods on null/empty
    if (!root) {
        // Create a simple document to ensure we have a node to work with if input is invalid
        doc.append_child("root");
        root = doc.first_child();
    }
    
    // Iterate through all nodes and their attributes to maximize coverage
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            // 1. as_string
            const pugi::char_t* str_val = attr.as_string(PUGIXML_TEXT("default"));
            (void)str_val; // Suppress unused variable warning
            
            // 2. as_int
            int int_val = attr.as_int(0);
            (void)int_val;
            
            // 3. as_uint
            unsigned int uint_val = attr.as_uint(0);
            (void)uint_val;
            
            // 4. as_double
            double double_val = attr.as_double(0.0);
            (void)double_val;
            
            // 5. as_float
            float float_val = attr.as_float(0.0f);
            (void)float_val;
            
            // 6. as_bool
            bool bool_val = attr.as_bool(false);
            (void)bool_val;
            
            // 7. as_llong
            long long llong_val = attr.as_llong(0LL);
            (void)llong_val;
            
            // 8. as_ullong
            unsigned long long ullong_val = attr.as_ullong(0ULL);
            (void)ullong_val;
        }
        
        // Also test on a null attribute to ensure robustness
        pugi::xml_attribute null_attr(nullptr);
        (void)null_attr.as_string(PUGIXML_TEXT("null_default"));
        (void)null_attr.as_int(-1);
        (void)null_attr.as_uint(999);
        (void)null_attr.as_double(-1.0);
        (void)null_attr.as_float(-1.0f);
        (void)null_attr.as_bool(true);
        (void)null_attr.as_llong(-1LL);
        (void)null_attr.as_ullong(999ULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Run the fuzz test logic
    run_fuzz_test(Data, Size);
    
    return 0;
}