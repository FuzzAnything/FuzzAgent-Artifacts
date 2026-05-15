// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_double at pugixml.cpp:5323:37 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
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

static void create_dummy_xml(const uint8_t* data, size_t size, const char* filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Create dummy XML file with fuzzer data
    const char* filename = "./dummy_file";
    create_dummy_xml(Data, Size, filename);
    
    // Try to parse the XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    if (result) {
        // Get the root element
        pugi::xml_node root = doc.root();
        
        if (root) {
            // Get the first attribute
            pugi::xml_attribute attr = root.first_attribute();
            
            if (attr) {
                // Call the API functions in the specified order
                volatile const char* str_val = attr.as_string();
                (void)str_val;
                
                volatile int int_val = attr.as_int();
                (void)int_val;
                
                volatile unsigned int uint_val = attr.as_uint();
                (void)uint_val;
                
                volatile double double_val = attr.as_double();
                (void)double_val;
                
                volatile float float_val = attr.as_float();
                (void)float_val;
                
                volatile bool bool_val = attr.as_bool();
                (void)bool_val;
                
                volatile long long llong_val = attr.as_llong();
                (void)llong_val;
                
                volatile unsigned long long ullong_val = attr.as_ullong();
                (void)ullong_val;
            }
        }
    }
    
    // Also test with direct attribute creation from fuzzer data
    // Create a minimal XML structure with attribute
    const char* test_xml = "<root attr=\"test\"/>";
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_string(test_xml);
    
    if (result2) {
        pugi::xml_node root2 = doc2.child("root");
        if (root2) {
            pugi::xml_attribute attr2 = root2.attribute("attr");
            if (attr2) {
                // Test with different default values
                attr2.as_string("default");
                attr2.as_int(42);
                attr2.as_uint(42u);
                attr2.as_double(3.14);
                attr2.as_float(3.14f);
                attr2.as_bool(true);
                attr2.as_llong(1234567890123LL);
                attr2.as_ullong(1234567890123ULL);
            }
        }
    }
    
    // Test with empty/null attribute
    pugi::xml_attribute empty_attr;
    empty_attr.as_string();
    empty_attr.as_int();
    empty_attr.as_uint();
    empty_attr.as_double();
    empty_attr.as_float();
    empty_attr.as_bool();
    empty_attr.as_llong();
    empty_attr.as_ullong();
    
    return 0;
}