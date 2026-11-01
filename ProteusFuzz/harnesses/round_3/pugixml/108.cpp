// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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

static pugi::xml_document* parse_fuzz_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    
    // Ensure the input is null-terminated for load_string
    // We create a temporary buffer that is guaranteed to be null-terminated
    // to prevent heap-buffer-overflow when pugixml calls strlen internally.
    std::vector<char> buffer(size + 1, '\0');
    std::memcpy(buffer.data(), data, size);
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_string(buffer.data());
    
    if (!result) {
        delete doc;
        return nullptr;
    }
    return doc;
}

static void cleanup_document(pugi::xml_document* doc) {
    delete doc;
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document* doc = parse_fuzz_input(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->root();
    if (!root) {
        cleanup_document(doc);
        return 0;
    }

    // Test 1: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused warning

    // Test 2: find_attribute with a simple predicate
    // Predicate: find attribute with name length > 0 (trivial check)
    auto pred = [](const pugi::xml_attribute& attr) {
        return attr && attr.name() && attr.name()[0] != '\0';
    };
    pugi::xml_attribute found_attr = root.find_attribute(pred);
    (void)found_attr;

    // Test 3: insert_attribute_after
    // Try to insert after first_attr if it exists
    if (first_attr) {
        std::string_view new_name = "fuzzed_attr_after";
        pugi::xml_attribute inserted_after = root.insert_attribute_after(new_name, first_attr);
        if (inserted_after) {
            inserted_after.set_value("value_after");
        }
    }

    // Test 4: attribute
    std::string_view attr_name = "test_attr";
    pugi::xml_attribute attr = root.attribute(attr_name.data());
    (void)attr;

    // Test 5: insert_attribute_before
    // Try to insert before first_attr if it exists
    if (first_attr) {
        std::string_view new_name_before = "fuzzed_attr_before";
        pugi::xml_attribute inserted_before = root.insert_attribute_before(new_name_before, first_attr);
        if (inserted_before) {
            inserted_before.set_value("value_before");
        }
    }

    // Test 6: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Additional exploration: try operations on child nodes if any
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        pugi::xml_attribute c_first = child.first_attribute();
        (void)c_first;
        
        pugi::xml_attribute c_last = child.last_attribute();
        (void)c_last;
        
        if (c_first) {
            std::string_view c_name = "child_fuzz";
            child.insert_attribute_after(c_name, c_first);
            child.insert_attribute_before(c_name, c_first);
        }
        
        pugi::xml_attribute c_attr = child.attribute("dummy");
        (void)c_attr;
    }

    cleanup_document(doc);
    return 0;
}