// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_int at pugixml.cpp:5309:34 in pugixml.hpp
// pugi::xml_attribute::as_uint at pugixml.cpp:5316:43 in pugixml.hpp
// pugi::xml_attribute::as_bool at pugixml.cpp:5337:35 in pugixml.hpp
// pugi::xml_attribute::as_llong at pugixml.cpp:5345:40 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
// pugi::xml_attribute::as_float at pugixml.cpp:5330:36 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Try to parse as XML string
    if (size > 0) {
        // Ensure null termination for safety
        char* buffer = new char[size + 1];
        if (!buffer) {
            delete doc;
            return nullptr;
        }
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc->load_string(buffer);
        delete[] buffer;
        
        // If parsing fails, try to create a minimal valid XML structure
        if (!result) {
            // Create a minimal valid XML document
            doc->reset();
            pugi::xml_node root = doc->append_child("root");
            if (root) {
                // Add an attribute to test with
                pugi::xml_attribute attr = root.append_attribute("test_attr");
                if (attr) {
                    // Set a default value that can be modified
                    attr.set_value("0");
                }
            }
        }
    } else {
        // Empty input, create minimal document
        doc->reset();
        pugi::xml_node root = doc->append_child("root");
        if (root) {
            pugi::xml_attribute attr = root.append_attribute("test_attr");
            if (attr) {
                attr.set_value("0");
            }
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Create document from input
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;
    
    // Get root node
    pugi::xml_node root = doc->first_child();
    if (!root) {
        delete doc;
        return 0;
    }
    
    // Get or create attribute for testing
    pugi::xml_attribute attr = root.attribute("test_attr");
    if (!attr) {
        attr = root.append_attribute("test_attr");
        if (!attr) {
            delete doc;
            return 0;
        }
        attr.set_value("0");
    }
    
    // Fuzz as_float with various default values
    float float_def1 = 0.0f;
    float float_def2 = -1.0f;
    float float_def3 = 999.99f;
    
    attr.as_float(float_def1);
    attr.as_float(float_def2);
    attr.as_float(float_def3);
    attr.as_float(); // default 0.0f
    
    // Fuzz as_int with various default values
    int int_def1 = 0;
    int int_def2 = -1;
    int int_def3 = 12345;
    
    attr.as_int(int_def1);
    attr.as_int(int_def2);
    attr.as_int(int_def3);
    attr.as_int(); // default 0
    
    // Fuzz as_uint with various default values
    unsigned int uint_def1 = 0;
    unsigned int uint_def2 = 1;
    unsigned int uint_def3 = 65535;
    
    attr.as_uint(uint_def1);
    attr.as_uint(uint_def2);
    attr.as_uint(uint_def3);
    attr.as_uint(); // default 0
    
    // Fuzz as_bool with various default values
    bool bool_def1 = false;
    bool bool_def2 = true;
    
    attr.as_bool(bool_def1);
    attr.as_bool(bool_def2);
    attr.as_bool(); // default false
    
    // Fuzz as_llong with various default values
    long long llong_def1 = 0;
    long long llong_def2 = -1;
    long long llong_def3 = 9223372036854775807LL;
    
    attr.as_llong(llong_def1);
    attr.as_llong(llong_def2);
    attr.as_llong(llong_def3);
    attr.as_llong(); // default 0
    
    // Fuzz set_value with float values
    float set_val1 = 0.0f;
    float set_val2 = -1.5f;
    float set_val3 = 3.14159f;
    float set_val4 = 1e10f;
    float set_val5 = -1e10f;
    
    attr.set_value(set_val1);
    attr.set_value(set_val2);
    attr.set_value(set_val3);
    attr.set_value(set_val4);
    attr.set_value(set_val5);
    
    // Test with different attribute values by setting strings first
    // Then test conversions again
    attr.set_value("123");
    attr.as_float();
    attr.as_int();
    attr.as_uint();
    attr.as_bool();
    attr.as_llong();
    
    attr.set_value("true");
    attr.as_bool();
    
    attr.set_value("false");
    attr.as_bool();
    
    attr.set_value("invalid");
    attr.as_float();
    attr.as_int();
    attr.as_uint();
    attr.as_bool();
    attr.as_llong();
    
    attr.set_value("");
    attr.as_float();
    attr.as_int();
    attr.as_uint();
    attr.as_bool();
    attr.as_llong();
    
    // Clean up
    delete doc;
    return 0;
}