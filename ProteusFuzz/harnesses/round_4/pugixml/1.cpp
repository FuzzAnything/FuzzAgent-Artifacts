// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* Data, size_t Size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input to a file to simulate loading XML from disk
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // We need a valid attribute to test the conversion functions.
    pugi::xml_attribute attr;
    
    // Try to find an attribute in the loaded document
    pugi::xml_node root = doc.first_child();
    if (root) {
        attr = root.attribute("test");
    }
    
    // If no attribute found, create a new document with an attribute
    // We declare doc2 here so it stays alive for the duration of the function
    pugi::xml_document doc2;
    
    if (!attr) {
        pugi::xml_node node = doc2.append_child("root");
        
        // Create a safe string from input for the attribute value
        // Limit length to prevent huge allocations and handle potential nulls
        size_t val_len = (Size > 100) ? 100 : Size;
        std::string val_str(reinterpret_cast<const char*>(Data), val_len);
        
        node.append_attribute("test").set_value(val_str.c_str());
        attr = node.attribute("test");
    }

    // Invoke target functions in the specified order
    // 1. as_string
    const char* str_val = attr.as_string();
    (void)str_val;

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
    long long llong_val = attr.as_llong(0);
    (void)llong_val;

    // 8. as_ullong
    unsigned long long ullong_val = attr.as_ullong(0);
    (void)ullong_val;

    return 0;
}