// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static void populate_with_attributes(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Create some attributes using data from fuzzer input
    if (size == 0) return;
    
    // Use first few bytes to determine number of attributes to create
    size_t num_attrs = (data[0] % 5) + 1;
    size_t offset = 1;
    
    for (size_t i = 0; i < num_attrs; i++) {
        if (offset >= size) break;
        
        // Create attribute name from fuzzer data
        size_t name_len = (data[offset] % 10) + 1;
        offset = (offset + 1) % size;
        
        char name[11];
        for (size_t j = 0; j < name_len && offset < size; j++) {
            name[j] = data[offset];
            offset = (offset + 1) % size;
        }
        name[name_len] = '\0';
        
        node.append_attribute(name);
    }
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size) {
    // Create XML document and parse from fuzzer input
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // Get a node to work with - use root or create one if parsing failed
    pugi::xml_node node;
    if (result) {
        node = doc.root();
        if (!node) {
            node = doc.append_child("root");
        }
    } else {
        // Create a dummy document with a node
        node = doc.append_child("test_node");
    }
    
    // Populate node with some attributes for testing
    populate_with_attributes(node, data, size);
    
    // Test 1: last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();
    
    // Test 2: first_attribute
    pugi::xml_attribute first_attr = node.first_attribute();
    
    // Test 3: attribute by name
    if (size > 0) {
        // Create a name from fuzzer data
        size_t name_len = (data[0] % 20) + 1;
        if (name_len > size) name_len = size;
        
        char name[21];
        for (size_t i = 0; i < name_len; i++) {
            name[i] = data[i % size];
        }
        name[name_len] = '\0';
        
        pugi::xml_attribute found_attr = node.attribute(name);
    }
    
    // Test 4: insert_attribute_before
    if (first_attr) {
        // Create name for new attribute
        std::string_view attr_name = "new_attr";
        pugi::xml_attribute inserted = node.insert_attribute_before(attr_name.data(), first_attr);
    }
    
    // Test 5: prepend_attribute
    std::string_view prepend_name = "prepended";
    pugi::xml_attribute prepended = node.prepend_attribute(prepend_name.data());
    
    // Test 6: find_attribute with lambda predicate
    auto predicate = [](pugi::xml_attribute attr) -> bool {
        return attr && attr.name() && attr.name()[0] != '\0';
    };
    pugi::xml_attribute found_by_predicate = node.find_attribute(predicate);
    
    // Cleanup - nothing specific needed as pugixml handles memory
    
    return 0;
}