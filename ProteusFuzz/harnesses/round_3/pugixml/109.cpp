// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <string_view>

static bool fuzzer_predicate(const pugi::xml_attribute& attr) {
    if (!attr) return false;
    const pugi::char_t* name = attr.name();
    if (!name) return false;
    size_t len = 0;
    while (name[len]) ++len;
    return len > 0 && (name[0] >= 'a' && name[0] <= 'z');
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for load_string
    // We create a buffer with a null terminator to prevent buffer overflows
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // If parsing fails, try to create a minimal valid document to test attribute functions
    if (!result) {
        // Create a simple valid XML document to ensure we have a node to test on
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        root.append_attribute("id").set_value("test");
    }

    pugi::xml_node node = doc.child("root");
    if (!node) {
        // If no root, try document itself or first child
        node = doc;
    }

    // Test 1: first_attribute
    pugi::xml_attribute first_attr = node.first_attribute();
    (void)first_attr;

    // Test 2: find_attribute with predicate
    pugi::xml_attribute found_attr = node.find_attribute(fuzzer_predicate);
    (void)found_attr;

    // Test 3: attribute by name
    std::string_view a_str("id");
    pugi::xml_attribute attr_by_name = node.attribute(a_str.data());
    (void)attr_by_name;

    // Test 4: insert_attribute_after
    pugi::xml_attribute ref_attr = node.first_attribute();
    if (ref_attr) {
        std::string_view new_name("new_attr_after");
        pugi::xml_attribute inserted_after = node.insert_attribute_after(new_name.data(), ref_attr);
        if (inserted_after) {
            inserted_after.set_value("value_after");
        }
    }

    // Test 5: insert_attribute_before
    ref_attr = node.first_attribute();
    if (ref_attr) {
        std::string_view new_name_before("new_attr_before");
        pugi::xml_attribute inserted_before = node.insert_attribute_before(new_name_before.data(), ref_attr);
        if (inserted_before) {
            inserted_before.set_value("value_before");
        }
    }

    // Test 6: last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();
    (void)last_attr;

    return 0;
}