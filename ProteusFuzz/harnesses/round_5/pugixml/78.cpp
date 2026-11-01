// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Attempt to parse the input as XML
    doc.load_buffer(Data, Size);

    // Create a valid attribute by adding one to a node if parsing succeeded
    pugi::xml_attribute attr;
    if (doc.root()) {
        // Try to set an attribute on the root to ensure we have a valid attribute to test
        doc.root().append_attribute("test_attr").set_value("initial");
        attr = doc.root().attribute("test_attr");
    } else {
        // If parsing failed, try loading from a dummy file created from input
        write_dummy_file(Data, Size);
        if (doc.load_file("./dummy_file")) {
            if (doc.root()) {
                doc.root().append_attribute("test_attr").set_value("initial");
                attr = doc.root().attribute("test_attr");
            }
        }
    }

    // Prepare test data from input
    const char* raw_data = reinterpret_cast<const char*>(Data);
    size_t max_len = Size;
    
    // 1. Test pugi::xml_attribute::set_name (overload 1: const char_t* rhs)
    // This overload expects a null-terminated string. We must ensure safety.
    // We search for a null terminator within the buffer bounds.
    size_t null_pos = 0;
    bool found_null = false;
    for (size_t i = 0; i < max_len; ++i) {
        if (raw_data[i] == '\0') {
            null_pos = i;
            found_null = true;
            break;
        }
    }
    
    if (found_null) {
        // Safe to call with the pointer as it points to a null-terminated string within bounds
        attr.set_name(raw_data);
    } else {
        // If no null terminator, we cannot safely call the C-string overload.
        // We skip this specific overload to prevent undefined behavior.
        // The fuzzer will eventually generate inputs with null terminators.
    }

    // 1. Test pugi::xml_attribute::set_name (overload 2: const char_t* rhs, size_t size)
    // This overload allows non-null-terminated strings, so we can use the raw buffer safely.
    if (max_len > 0) {
        // Use the entire buffer or a portion of it
        size_t test_size = (max_len > 100) ? 100 : max_len;
        attr.set_name(raw_data, test_size);
    }

    // 2. Test pugi::xml_attribute::value
    // Safe to call on empty attributes too, returns ""
    const pugi::char_t* val = attr.value();
    (void)val; // Suppress unused warning

    // 3. Test pugi::xml_attribute::set_value (const char_t* rhs, size_t size)
    if (max_len > 0) {
        size_t test_size = (max_len > 100) ? 100 : max_len;
        attr.set_value(raw_data, test_size);
    }

    // 4. Test pugi::xml_attribute::name
    // Safe to call on empty attributes
    const pugi::char_t* name = attr.name();
    (void)name;

    // 5. Test pugi::xml_attribute::as_string
    // Safe to call on empty attributes
    const pugi::char_t* str = attr.as_string();
    (void)str;

    // Test as_string with a default value
    const pugi::char_t* str_def = attr.as_string("default");
    (void)str_def;

    return 0;
}