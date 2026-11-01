// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static void fuzz_attribute_methods(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Load data as XML, ignore parse errors to continue fuzzing
    doc.load_buffer(data, size);
    
    // Iterate through all nodes and their attributes
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            // Test as_string with default and custom default
            const pugi::char_t* str1 = attr.as_string();
            const pugi::char_t* str2 = attr.as_string(PUGIXML_TEXT("default"));
            
            // Test value
            const pugi::char_t* val = attr.value();
            
            // Test name
            const pugi::char_t* name = attr.name();
            
            // Test set_value
            if (size > 0) {
                pugi::char_t new_value[256];
                size_t copy_len = (size < 255) ? size : 255;
                std::memcpy(new_value, data, copy_len);
                new_value[copy_len] = 0;
                attr.set_value(new_value);
            }
            
            // Test set_name with C-string
            if (size > 0) {
                pugi::char_t new_name[256];
                size_t copy_len = (size < 255) ? size : 255;
                std::memcpy(new_name, data, copy_len);
                new_name[copy_len] = 0;
                attr.set_name(new_name);
            }
            
            // Test set_name with size parameter
            if (size > 0) {
                pugi::char_t new_name[256];
                size_t copy_len = (size < 255) ? size : 255;
                std::memcpy(new_name, data, copy_len);
                new_name[copy_len] = 0;
                attr.set_name(new_name, copy_len);
            }
        }
    }
    
    // Test with empty attribute object
    pugi::xml_attribute empty_attr;
    const pugi::char_t* empty_str1 = empty_attr.as_string();
    const pugi::char_t* empty_str2 = empty_attr.as_string(PUGIXML_TEXT("fallback"));
    const pugi::char_t* empty_val = empty_attr.value();
    const pugi::char_t* empty_name = empty_attr.name();
    
    // These should return false as attribute is empty
    empty_attr.set_value(PUGIXML_TEXT("test"));
    empty_attr.set_name(PUGIXML_TEXT("test"));
    empty_attr.set_name(PUGIXML_TEXT("test"), 4);
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_attribute_methods(Data, Size);
    
    return 0;
}