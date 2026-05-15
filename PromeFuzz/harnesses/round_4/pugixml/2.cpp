// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create a temporary file with the fuzzer input
    const char* filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse the XML document from the file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    // If parsing succeeded, try to get an attribute to test
    if (result) {
        // Try to get the first attribute from the root element
        pugi::xml_node root = doc.root();
        if (root) {
            pugi::xml_attribute attr = root.first_attribute();
            
            // Call the conversion functions in the specified order
            if (attr) {
                // Use volatile variables to prevent optimization
                volatile auto str_val = attr.as_string();
                volatile auto int_val = attr.as_int();
                volatile auto uint_val = attr.as_uint();
                volatile auto double_val = attr.as_double();
                volatile auto float_val = attr.as_float();
                volatile auto bool_val = attr.as_bool();
                volatile auto llong_val = attr.as_llong();
                volatile auto ullong_val = attr.as_ullong();
                
                // Also test with custom default values
                volatile auto str_val_def = attr.as_string("default");
                volatile auto int_val_def = attr.as_int(42);
                volatile auto uint_val_def = attr.as_uint(42u);
                volatile auto double_val_def = attr.as_double(3.14);
                volatile auto float_val_def = attr.as_float(3.14f);
                volatile auto bool_val_def = attr.as_bool(true);
                volatile auto llong_val_def = attr.as_llong(42LL);
                volatile auto ullong_val_def = attr.as_ullong(42ULL);
            }
            
            // Also test on subsequent attributes
            for (pugi::xml_attribute a = root.first_attribute(); a; a = a.next_attribute()) {
                volatile auto v1 = a.as_string();
                volatile auto v2 = a.as_int();
                volatile auto v3 = a.as_uint();
                volatile auto v4 = a.as_double();
                volatile auto v5 = a.as_float();
                volatile auto v6 = a.as_bool();
                volatile auto v7 = a.as_llong();
                volatile auto v8 = a.as_ullong();
            }
        }
    }
    
    // Also test with manually created attribute structure
    // Create a simple XML document with an attribute
    const char* xml_content = "<root attr=\"test123\"/>";
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_string(xml_content);
    
    if (result2) {
        pugi::xml_node root2 = doc2.child("root");
        if (root2) {
            pugi::xml_attribute attr2 = root2.attribute("attr");
            if (attr2) {
                volatile auto v1 = attr2.as_string();
                volatile auto v2 = attr2.as_int();
                volatile auto v3 = attr2.as_uint();
                volatile auto v4 = attr2.as_double();
                volatile auto v5 = attr2.as_float();
                volatile auto v6 = attr2.as_bool();
                volatile auto v7 = attr2.as_llong();
                volatile auto v8 = attr2.as_ullong();
            }
        }
    }
    
    // Test with empty/null attribute
    pugi::xml_attribute empty_attr;
    volatile auto empty_str = empty_attr.as_string();
    volatile auto empty_int = empty_attr.as_int();
    volatile auto empty_uint = empty_attr.as_uint();
    volatile auto empty_double = empty_attr.as_double();
    volatile auto empty_float = empty_attr.as_float();
    volatile auto empty_bool = empty_attr.as_bool();
    volatile auto empty_llong = empty_attr.as_llong();
    volatile auto empty_ullong = empty_attr.as_ullong();
    
    // Test with custom defaults for empty attribute
    volatile auto empty_str_def = empty_attr.as_string("empty");
    volatile auto empty_int_def = empty_attr.as_int(-1);
    volatile auto empty_uint_def = empty_attr.as_uint(99u);
    volatile auto empty_double_def = empty_attr.as_double(2.71);
    volatile auto empty_float_def = empty_attr.as_float(2.71f);
    volatile auto empty_bool_def = empty_attr.as_bool(true);
    volatile auto empty_llong_def = empty_attr.as_llong(-99LL);
    volatile auto empty_ullong_def = empty_attr.as_ullong(99ULL);

    return 0;
}