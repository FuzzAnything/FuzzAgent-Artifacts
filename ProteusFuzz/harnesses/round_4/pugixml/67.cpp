// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
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
#include <string_view>

static void fuzz_attribute(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (!doc.load_buffer(data, size)) {
        // If parsing fails, try to create a minimal valid document to test attribute methods on empty/null attributes
        doc.append_child("root");
    }

    // Get a node to work with
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Create an attribute to test with
    pugi::xml_attribute attr = root.append_attribute("test_attr");
    if (!attr) {
        // Fallback if memory allocation failed, test with empty attribute
        attr = pugi::xml_attribute();
    }

    // Test set_value with buffer data
    if (size > 0) {
        // Use a portion of the input as the value
        size_t val_size = size;
        if (val_size > 1024) val_size = 1024;
        attr.set_value(reinterpret_cast<const pugi::char_t*>(data), val_size);
    }

    // Test as_string with default
    const pugi::char_t* str1 = attr.as_string();
    (void)str1;

    // Test as_string with custom default from input if possible
    if (size > 10) {
        const pugi::char_t* def = reinterpret_cast<const pugi::char_t*>(data + 10);
        const pugi::char_t* str2 = attr.as_string(def);
        (void)str2;
    }

    // Test name
    const pugi::char_t* name = attr.name();
    (void)name;

    // Test value
    const pugi::char_t* value = attr.value();
    (void)value;

    // Test set_name with buffer data (overload 1: with size)
    if (size > 0) {
        size_t name_size = size;
        if (name_size > 64) name_size = 64;
        attr.set_name(reinterpret_cast<const pugi::char_t*>(data), name_size);
    }

    // Test set_name with buffer data (overload 2: null-terminated)
    // We need to ensure null termination for the second overload
    if (size > 0) {
        size_t name_len = size;
        if (name_len > 64) name_len = 64;
        // Check if there's a null terminator within the range
        size_t null_pos = name_len;
        for (size_t i = 0; i < name_len; ++i) {
            if (data[i] == 0) {
                null_pos = i;
                break;
            }
        }
        // If no null terminator, we can't safely use the C-string overload directly without copying
        // But the fuzzer might provide null-terminated strings.
        // We attempt to call it if we found a null terminator or if the buffer is small enough to be safe-ish in fuzzing context
        // However, to be strictly safe and avoid OOB, we only call if null terminated.
        if (null_pos < name_len) {
            attr.set_name(reinterpret_cast<const pugi::char_t*>(data));
        }
    }

    // Test with an empty attribute object to ensure safety
    pugi::xml_attribute empty_attr;
    const pugi::char_t* empty_name = empty_attr.name();
    const pugi::char_t* empty_value = empty_attr.value();
    const pugi::char_t* empty_as_string = empty_attr.as_string();
    bool empty_set_val = empty_attr.set_value("test", 4);
    bool empty_set_name1 = empty_attr.set_name("new_name", 8);
    bool empty_set_name2 = empty_attr.set_name("new_name");
    
    (void)empty_name;
    (void)empty_value;
    (void)empty_as_string;
    (void)empty_set_val;
    (void)empty_set_name1;
    (void)empty_set_name2;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_attribute(Data, Size);
    
    return 0;
}