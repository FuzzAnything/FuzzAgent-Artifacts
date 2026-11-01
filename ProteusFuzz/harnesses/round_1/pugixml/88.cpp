// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <string>
#include <vector>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    if (!doc.load_string("<?xml version=\"1.0\"?><root><child attr1=\"val1\" attr2=\"val2\"/></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    pugi::xml_node child = root.child("child");
    if (!child) {
        return 0;
    }

    // Get existing attributes for reference
    pugi::xml_attribute attr1 = child.attribute("attr1");
    pugi::xml_attribute attr2 = child.attribute("attr2");

    // Convert input data to a string for attribute names
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for char_t* functions
    std::string safe_str = input_str;
    safe_str.push_back('\0');
    const char* name_cstr = safe_str.c_str();

    // Fuzz insert_copy_before
    // Requires proto and attr to be valid siblings
    if (attr1 && attr2) {
        child.insert_copy_before(attr1, attr2);
        child.insert_copy_before(attr2, attr1);
    }
    // Try with invalid proto
    pugi::xml_attribute empty_attr;
    child.insert_copy_before(empty_attr, attr1);
    child.insert_copy_before(attr1, empty_attr);

    // Fuzz insert_attribute_after
    // Requires valid attr
    if (attr1) {
        child.insert_attribute_after(name_cstr, attr1);
    }
    if (attr2) {
        child.insert_attribute_after(name_cstr, attr2);
    }
    // Try with invalid attr
    child.insert_attribute_after(name_cstr, empty_attr);

    // Fuzz insert_attribute_before
    // Requires valid attr
    if (attr1) {
        std::string_view sv_name(safe_str);
        child.insert_attribute_before(sv_name, attr1);
    }
    if (attr2) {
        std::string_view sv_name(safe_str);
        child.insert_attribute_before(sv_name, attr2);
    }
    // Try with invalid attr
    std::string_view sv_empty;
    std::string_view sv_name_invalid(safe_str);
    child.insert_attribute_before(sv_empty, empty_attr);
    child.insert_attribute_before(sv_name_invalid, empty_attr);

    // Fuzz attribute
    pugi::xml_attribute found = child.attribute(name_cstr);
    (void)found; // Suppress unused warning
    
    // Fuzz with hint optimization
    pugi::xml_attribute hint = attr1;
    found = child.attribute(name_cstr, hint);
    (void)found;
    
    // Try with empty name
    child.attribute("");

    // Fuzz prepend_attribute
    child.prepend_attribute(name_cstr);
    child.prepend_attribute("");

    // Fuzz insert_copy_after
    // Requires proto and attr to be valid siblings
    if (attr1 && attr2) {
        child.insert_copy_after(attr1, attr2);
        child.insert_copy_after(attr2, attr1);
    }
    // Try with invalid proto or attr
    child.insert_copy_after(empty_attr, attr1);
    child.insert_copy_after(attr1, empty_attr);

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}