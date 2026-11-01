// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
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
#include <memory>

static std::string generate_xml_content(const uint8_t* data, size_t size) {
    std::string result = "<root";
    // Add attributes based on input data to ensure diverse states
    for (size_t i = 0; i < size && i < 100; ++i) {
        char name[32];
        char value[32];
        snprintf(name, sizeof(name), "attr_%zu", i);
        snprintf(value, sizeof(value), "val_%zu", data[i]);
        result += " ";
        result += name;
        result += "=\"";
        result += value;
        result += "\"";
    }
    result += ">";
    // Add some text content
    for (size_t i = 0; i < size && i < 100; ++i) {
        result += static_cast<char>(data[i] % 26 + 'a');
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    std::string xml_content = generate_xml_content(Data, Size);
    
    if (!doc.load_string(xml_content.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Test 1: first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused variable warning

    // Test 2: last_attribute
    pugi::xml_attribute last_attr = root.last_attribute();
    (void)last_attr;

    // Test 3: attribute
    std::string attr_name = "attr_0";
    pugi::xml_attribute found_attr = root.attribute(attr_name.c_str());
    (void)found_attr;

    // Test 4: insert_attribute_after
    // Only attempt if there is at least one attribute
    if (first_attr) {
        pugi::xml_attribute new_attr_after = root.insert_attribute_after("new_attr_after", first_attr);
        if (new_attr_after) {
            new_attr_after.set_value("value_after");
        }
    }

    // Test 5: insert_attribute_before
    // Only attempt if there is at least one attribute
    if (first_attr) {
        pugi::xml_attribute new_attr_before = root.insert_attribute_before("new_attr_before", first_attr);
        if (new_attr_before) {
            new_attr_before.set_value("value_before");
        }
    }

    // Test 6: find_attribute with a simple predicate
    // Find an attribute whose name starts with "attr_"
    auto predicate = [](pugi::xml_attribute attr) {
        if (!attr) return false;
        const char* name = attr.name();
        if (!name) return false;
        return name[0] == 'a' && name[1] == 't' && name[2] == 't' && name[3] == 'r';
    };
    pugi::xml_attribute found_by_pred = root.find_attribute(predicate);
    (void)found_by_pred;

    // Additional stress: try operations on null nodes or empty states
    pugi::xml_node null_node;
    pugi::xml_attribute null_first = null_node.first_attribute();
    pugi::xml_attribute null_last = null_node.last_attribute();
    pugi::xml_attribute null_attr = null_node.attribute("test");
    pugi::xml_attribute null_find = null_node.find_attribute(predicate);
    (void)null_first;
    (void)null_last;
    (void)null_attr;
    (void)null_find;

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}