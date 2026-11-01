// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
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
#include <functional>

static bool predicate_true(pugi::xml_attribute attr) {
    return true;
}

static bool predicate_false(pugi::xml_attribute attr) {
    return false;
}

static bool predicate_name_check(pugi::xml_attribute attr) {
    return attr && attr.name() && attr.name()[0] == 'a';
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a simple document to test attribute functions
        doc.append_child("root");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Ensure we have some attributes to work with for insert_before/after tests
    pugi::xml_attribute ref_attr = root.append_attribute("existing_attr");
    if (!ref_attr) {
        // If allocation fails, we can't test insert functions effectively
        return 0;
    }

    // Convert input data to string for attribute names
    std::string name_str(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety with char_t*
    if (name_str.size() > 0 && name_str.back() == '\0') {
        name_str.pop_back();
    }
    name_str.push_back('\0');

    const char* name_cstr = name_str.c_str();
    std::string_view name_sv(name_cstr);

    // Test 1: insert_attribute_before
    pugi::xml_attribute new_attr_before = root.insert_attribute_before(name_sv, ref_attr);
    (void)new_attr_before;

    // Test 2: attribute
    pugi::xml_attribute found_attr = root.attribute(name_cstr);
    (void)found_attr;

    // Test 3: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Test 4: insert_attribute_after
    pugi::xml_attribute new_attr_after = root.insert_attribute_after(name_sv, ref_attr);
    (void)new_attr_after;

    // Test 5: find_attribute with different predicates
    pugi::xml_attribute found_pred_true = root.find_attribute(predicate_true);
    (void)found_pred_true;

    pugi::xml_attribute found_pred_false = root.find_attribute(predicate_false);
    (void)found_pred_false;

    pugi::xml_attribute found_pred_name = root.find_attribute(predicate_name_check);
    (void)found_pred_name;

    // Test 6: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr;

    // Additional edge case: try operations on an empty node
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_attr = empty_node.first_attribute();
    (void)empty_attr;
    pugi::xml_attribute empty_last = empty_node.last_attribute();
    (void)empty_last;
    pugi::xml_attribute empty_find = empty_node.find_attribute(predicate_true);
    (void)empty_find;
    pugi::xml_attribute empty_before = empty_node.insert_attribute_before(name_sv, ref_attr);
    (void)empty_before;
    pugi::xml_attribute empty_after = empty_node.insert_attribute_after(name_sv, ref_attr);
    (void)empty_after;
    pugi::xml_attribute empty_attr_lookup = empty_node.attribute(name_cstr);
    (void)empty_attr_lookup;

    // Cleanup is handled by RAII (doc destructor)

    return 0;
}