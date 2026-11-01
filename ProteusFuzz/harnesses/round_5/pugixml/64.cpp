// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

static bool parse_xml_from_fuzz_data(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    // Ensure null termination for safety, though parse_buffer handles size
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_node root;
    
    // Attempt to parse the input data as XML
    if (!parse_xml_from_fuzz_data(doc, Data, Size)) {
        // If parsing fails, try to create a minimal valid XML structure to test API behavior
        // This ensures we can still test the API functions even with invalid XML input
        doc.append_child("root");
        root = doc.first_child();
    } else {
        root = doc.first_child();
        if (!root) {
            // If parsed but no root, create one
            root = doc.append_child("root");
        }
    }

    // Ensure we have a valid node to work with
    if (!root) {
        return 0;
    }

    // Helper to safely get an attribute
    auto get_attr = [&](const pugi::xml_node& node, const char* name) -> pugi::xml_attribute {
        return node.attribute(name);
    };

    // Helper to create an attribute if it doesn't exist
    auto ensure_attr = [&](pugi::xml_node& node, const char* name) -> pugi::xml_attribute {
        pugi::xml_attribute attr = node.attribute(name);
        if (!attr) {
            attr = node.append_attribute(name);
        }
        return attr;
    };

    // Test 1: insert_copy_before
    // Requires two valid attributes: proto and attr
    pugi::xml_attribute proto = ensure_attr(root, "proto_attr");
    pugi::xml_attribute attr_ref = ensure_attr(root, "ref_attr");
    
    // Insert a copy of proto before attr_ref
    pugi::xml_attribute copied = root.insert_copy_before(proto, attr_ref);
    (void)copied; // Suppress unused variable warning

    // Test with invalid attributes (empty)
    pugi::xml_attribute empty_attr;
    pugi::xml_attribute copied_invalid = root.insert_copy_before(empty_attr, attr_ref);
    (void)copied_invalid;

    pugi::xml_attribute copied_invalid2 = root.insert_copy_before(proto, empty_attr);
    (void)copied_invalid2;

    // Test 2: insert_attribute_after
    // Insert new attribute after an existing one
    pugi::xml_attribute new_after = root.insert_attribute_after("new_attr_after", attr_ref);
    (void)new_after;

    // Insert after empty attribute
    pugi::xml_attribute new_after_invalid = root.insert_attribute_after("new_attr_after_invalid", empty_attr);
    (void)new_after_invalid;

    // Test 3: insert_attribute_before
    // Insert new attribute before an existing one
    pugi::xml_attribute new_before = root.insert_attribute_before("new_attr_before", attr_ref);
    (void)new_before;

    // Insert before empty attribute
    pugi::xml_attribute new_before_invalid = root.insert_attribute_before("new_attr_before_invalid", empty_attr);
    (void)new_before_invalid;

    // Test 4: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Test 5: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr;

    // Test 6: find_attribute with predicate
    // Predicate to find attribute with name containing "attr"
    auto pred = [](const pugi::xml_attribute& a) {
        if (!a) return false;
        const char* name = a.name();
        if (!name) return false;
        return std::strstr(name, "attr") != nullptr;
    };
    pugi::xml_attribute found = root.find_attribute(pred);
    (void)found;

    // Predicate that always returns false
    auto pred_false = [](const pugi::xml_attribute&) { return false; };
    pugi::xml_attribute found_false = root.find_attribute(pred_false);
    (void)found_false;

    // Predicate that always returns true
    auto pred_true = [](const pugi::xml_attribute& a) { return !!a; };
    pugi::xml_attribute found_true = root.find_attribute(pred_true);
    (void)found_true;

    // Test with empty node
    pugi::xml_node empty_node;
    (void)empty_node.last_attribute();
    (void)empty_node.first_attribute();
    (void)empty_node.find_attribute(pred);
    (void)empty_node.insert_copy_before(proto, attr_ref);
    (void)empty_node.insert_attribute_after("test", attr_ref);
    (void)empty_node.insert_attribute_before("test", attr_ref);

    return 0;
}