// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
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

static std::string create_safe_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        return std::string();
    }
    // Ensure the string is null-terminated and safe for C-style string functions
    // We copy the data and ensure a null terminator exists at the end
    // We limit the length to avoid excessively large strings if fuzzing produces huge inputs
    size_t safe_size = size;
    if (safe_size > 1024) {
        safe_size = 1024;
    }
    
    std::string result(reinterpret_cast<const char*>(data), safe_size);
    // Replace any null bytes within the string to ensure it's treated as a single token
    // or simply ensure it's null-terminated (std::string does this, but we want to avoid internal nulls for attribute names)
    for (size_t i = 0; i < result.length(); ++i) {
        if (result[i] == '\0') {
            result[i] = ' '; // Replace null with space to prevent premature termination
        }
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_string("<root><child>text</child></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Step 1: pugi::xml_node::child
    std::string child_name_str = create_safe_string(Data, Size);
    const char* child_name = child_name_str.empty() ? "" : child_name_str.c_str();
    pugi::xml_node child_node = root.child(child_name);

    if (!child_node) {
        // If child not found, try to create one or use root for subsequent operations
        child_node = root;
    }

    // Step 2: pugi::xml_node::append_attribute
    std::string attr1_name_str = create_safe_string(Data, Size);
    const char* attr1_name = attr1_name_str.empty() ? "attr1" : attr1_name_str.c_str();
    pugi::xml_attribute attr1 = child_node.append_attribute(attr1_name);

    // Step 3: pugi::xml_node::append_attribute
    std::string attr2_name_str = create_safe_string(Data, Size);
    const char* attr2_name = attr2_name_str.empty() ? "attr2" : attr2_name_str.c_str();
    pugi::xml_attribute attr2 = child_node.append_attribute(attr2_name);

    // Step 4: pugi::xml_attribute::set_value (unsigned long)
    if (attr1) {
        unsigned long val_ul = Size % 1000000;
        attr1.set_value(val_ul);
    }

    // Step 5: pugi::xml_node::append_attribute
    std::string attr3_name_str = create_safe_string(Data, Size);
    const char* attr3_name = attr3_name_str.empty() ? "attr3" : attr3_name_str.c_str();
    pugi::xml_attribute attr3 = child_node.append_attribute(attr3_name);

    // Step 6: pugi::xml_node::append_attribute
    std::string attr4_name_str = create_safe_string(Data, Size);
    const char* attr4_name = attr4_name_str.empty() ? "attr4" : attr4_name_str.c_str();
    pugi::xml_attribute attr4 = child_node.append_attribute(attr4_name);

    // Step 7: pugi::xml_attribute::set_value (long)
    if (attr2) {
        long val_l = static_cast<long>(Size) - 500000;
        attr2.set_value(val_l);
    }

    // Cleanup is handled by pugi::xml_document destructor when it goes out of scope
    return 0;
}