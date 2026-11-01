// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
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

static bool parse_document_from_fuzz(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    // Ensure null termination for C-string parsing
    std::string buffer(reinterpret_cast<const char*>(data), size);
    if (buffer.back() != '\0') {
        buffer += '\0';
    }
    return doc.load_string(buffer.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    // Try to parse the input as XML
    parse_document_from_fuzz(doc, Data, Size);

    // Iterate through all attributes in the document to test the target functions
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            // Test 1: pugi::xml_attribute::set_name (overload 1: const char_t* rhs)
            // We use a safe string from the fuzz input or a default
            const char* new_name = "fuzzed_name";
            if (Size > 0) {
                // Use first byte as length hint, capped to avoid overflow
                size_t len = Data[0] % 20;
                if (len > 0 && len <= Size) {
                    // Create a temporary buffer for the new name
                    std::string temp_name(reinterpret_cast<const char*>(Data), len);
                    // Ensure no nulls in the middle for safety
                    if (temp_name.find('\0') == std::string::npos) {
                        attr.set_name(temp_name.c_str());
                    }
                }
            } else {
                attr.set_name(new_name);
            }

            // Test 2: pugi::xml_attribute::set_name (overload 2: const char_t* rhs, size_t size)
            if (Size > 0) {
                size_t len = Data[0] % 20;
                if (len > 0 && len <= Size) {
                    std::string temp_name(reinterpret_cast<const char*>(Data), len);
                    if (temp_name.find('\0') == std::string::npos) {
                        attr.set_name(temp_name.c_str(), temp_name.size());
                    }
                }
            } else {
                attr.set_name(new_name, strlen(new_name));
            }

            // Test 3: pugi::xml_attribute::as_string
            const pugi::char_t* default_val = PUGIXML_TEXT("default");
            const pugi::char_t* str_val = attr.as_string(default_val);
            (void)str_val; // Suppress unused variable warning

            // Test 4: pugi::xml_attribute::name
            const pugi::char_t* name_val = attr.name();
            (void)name_val; // Suppress unused variable warning

            // Test 5: pugi::xml_attribute::set_value
            if (Size > 0) {
                size_t len = Data[0] % 20;
                if (len > 0 && len <= Size) {
                    std::string temp_val(reinterpret_cast<const char*>(Data), len);
                    if (temp_val.find('\0') == std::string::npos) {
                        attr.set_value(temp_val.c_str(), temp_val.size());
                    }
                }
            } else {
                attr.set_value(PUGIXML_TEXT("fuzzed_value"));
            }

            // Test 6: pugi::xml_attribute::value
            const pugi::char_t* value_val = attr.value();
            (void)value_val; // Suppress unused variable warning
        }
    }

    // Test on empty attribute object (default constructed)
    pugi::xml_attribute empty_attr;
    
    // These should return false or empty strings safely
    empty_attr.set_name("test");
    empty_attr.set_name("test", 4);
    const pugi::char_t* empty_str = empty_attr.as_string();
    const pugi::char_t* empty_name = empty_attr.name();
    empty_attr.set_value("test");
    const pugi::char_t* empty_val = empty_attr.value();

    (void)empty_str;
    (void)empty_name;
    (void)empty_val;

    return 0;
}