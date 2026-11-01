// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
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
#include <functional>

static bool attribute_predicate(const pugi::xml_attribute& attr) {
    if (!attr) return false;
    const pugi::char_t* name = attr.name();
    if (!name) return false;
    // Simple predicate: check if name is not empty
    return name[0] != '\0';
}

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Attempt to parse the input as XML
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a simple document to test attribute functions
        doc.append_child("root");
    }

    // Get the root node
    pugi::xml_node root = doc.root();
    if (!root) {
        // If no root, create one
        root = doc.append_child("root");
    }

    // Test 1: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused warning

    // Test 2: find_attribute with predicate
    pugi::xml_attribute found_attr = root.find_attribute(attribute_predicate);
    (void)found_attr;

    // Test 3: insert_attribute_after
    // First, ensure there is at least one attribute to insert after
    if (first_attr) {
        std::string_view new_name = "new_attr_after";
        pugi::xml_attribute inserted_after = root.insert_attribute_after(new_name, first_attr);
        if (inserted_after) {
            inserted_after.set_value("value_after");
        }
    } else {
        // If no attribute exists, try inserting after a null attribute (should fail gracefully)
        std::string_view new_name = "new_attr_after_null";
        pugi::xml_attribute dummy_attr;
        pugi::xml_attribute inserted_after = root.insert_attribute_after(new_name, dummy_attr);
        (void)inserted_after;
    }

    // Test 4: attribute
    std::string_view attr_name = "test_attr";
    pugi::xml_attribute attr = root.attribute(attr_name.data());
    (void)attr;

    // Test 5: insert_attribute_before
    if (first_attr) {
        std::string_view new_name_before = "new_attr_before";
        pugi::xml_attribute inserted_before = root.insert_attribute_before(new_name_before, first_attr);
        if (inserted_before) {
            inserted_before.set_value("value_before");
        }
    } else {
        // Try inserting before a null attribute
        std::string_view new_name_before = "new_attr_before_null";
        pugi::xml_attribute dummy_attr;
        pugi::xml_attribute inserted_before = root.insert_attribute_before(new_name_before, dummy_attr);
        (void)inserted_before;
    }

    // Test 6: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}