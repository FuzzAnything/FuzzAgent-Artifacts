// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    static pugi::xml_document doc;
    static pugi::xml_node node;
    static pugi::xml_attribute attr1, attr2, attr3;
    static std::vector<pugi::xml_attribute> attributes;
    
    // Reset state for each fuzzing iteration
    doc.reset();
    node = pugi::xml_node();
    attr1 = pugi::xml_attribute();
    attr2 = pugi::xml_attribute();
    attr3 = pugi::xml_attribute();
    attributes.clear();
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing failed, create a simple document to work with
    if (!result) {
        doc.reset();
        node = doc.append_child("root");
        // Add some dummy attributes to have something to work with
        node.append_attribute("attr1");
        node.append_attribute("attr2");
        node.append_attribute("attr3");
    } else {
        // Get the first non-empty node
        node = doc.first_child();
        if (!node) {
            node = doc.append_child("root");
        }
    }
    
    // Ensure we have at least 3 attributes to work with
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        attributes.push_back(attr);
    }
    
    while (attributes.size() < 3) {
        std::string name = "attr" + std::to_string(attributes.size() + 1);
        attributes.push_back(node.append_attribute(name.c_str()));
    }
    
    attr1 = attributes[0];
    attr2 = attributes[1];
    attr3 = attributes[2];
    
    // Use fuzzer data to generate attribute names
    std::string_view attr_name;
    if (Size > 0) {
        // Use first byte modulo 10 + 1 to determine name length (1-10 chars)
        size_t name_len = (Data[0] % 10) + 1;
        if (name_len > Size) name_len = Size;
        attr_name = std::string_view(reinterpret_cast<const char*>(Data), name_len);
    } else {
        attr_name = "fuzz_attr";
    }
    
    // Test pugi::xml_node::attribute with hint
    pugi::xml_attribute hint = attr1;
    pugi::xml_attribute found_attr = node.attribute(attr_name.data(), hint);
    (void)found_attr; // Use result to avoid unused variable warning
    
    // Test pugi::xml_node::insert_attribute_after
    pugi::xml_attribute new_attr_after = node.insert_attribute_after(attr_name.data(), attr1);
    (void)new_attr_after;
    
    // Test pugi::xml_node::insert_attribute_before
    pugi::xml_attribute new_attr_before = node.insert_attribute_before(attr_name.data(), attr2);
    (void)new_attr_before;
    
    // Test pugi::xml_node::insert_copy_before
    pugi::xml_attribute copy_before = node.insert_copy_before(attr1, attr3);
    (void)copy_before;
    
    // Test pugi::xml_node::insert_copy_after
    pugi::xml_attribute copy_after = node.insert_copy_after(attr2, attr1);
    (void)copy_after;
    
    // Test pugi::xml_node::prepend_attribute
    pugi::xml_attribute prepended = node.prepend_attribute(attr_name.data());
    (void)prepended;
    
    return 0;
}