// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with attributes to test the functions
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Add some attributes using the fuzzer data
    // Split the input into chunks to create multiple attributes
    const uint8_t* current = Data;
    size_t remaining = Size;
    int attr_count = 0;
    
    while (remaining > 0 && attr_count < 10) {
        size_t chunk_size = remaining > 100 ? 100 : remaining;
        std::string attr_name(reinterpret_cast<const char*>(current), chunk_size);
        
        // Ensure the attribute name is not empty
        if (!attr_name.empty()) {
            // Replace null characters to avoid early termination
            for (char& c : attr_name) {
                if (c == '\0') c = 'a';
            }
            
            pugi::xml_attribute attr = node.append_attribute(attr_name.c_str());
            if (attr) {
                std::string attr_value = "value_" + attr_name;
                attr.set_value(attr_value.c_str());
            }
        }
        
        current += chunk_size;
        remaining -= chunk_size;
        attr_count++;
    }
    
    // Test 1: first_attribute
    pugi::xml_attribute first_attr = node.first_attribute();
    (void)first_attr; // Use to avoid unused variable warning
    
    // Test 2: last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();
    (void)last_attr;
    
    // Test 3: attribute by name
    if (Size > 0) {
        std::string_view attr_name_sv(reinterpret_cast<const char*>(Data), 
                                     Size > 100 ? 100 : Size);
        std::string attr_name_str(attr_name_sv);
        
        // Replace null characters
        for (char& c : attr_name_str) {
            if (c == '\0') c = 'b';
        }
        
        pugi::xml_attribute found_attr = node.attribute(attr_name_str.c_str());
        (void)found_attr;
    }
    
    // Test 4: find_attribute with predicate
    pugi::xml_attribute found_pred_attr = node.find_attribute(
        [](pugi::xml_attribute attr) {
            return std::string_view(attr.name()).find('a') != std::string_view::npos;
        }
    );
    (void)found_pred_attr;
    
    // Test 5: insert_attribute_before
    if (node.first_attribute()) {
        std::string new_attr_name;
        if (Size > 0) {
            new_attr_name = std::string(reinterpret_cast<const char*>(Data), 
                                       Size > 50 ? 50 : Size);
            for (char& c : new_attr_name) {
                if (c == '\0') c = 'c';
            }
        } else {
            new_attr_name = "new_attr";
        }
        
        pugi::xml_attribute inserted = node.insert_attribute_before(
            new_attr_name.c_str(), node.first_attribute());
        if (inserted) {
            inserted.set_value("inserted_value");
        }
    }
    
    // Test 6: prepend_attribute
    std::string prepend_attr_name;
    if (Size > 0) {
        size_t offset = Size > 1 ? 1 : 0;
        prepend_attr_name = std::string(reinterpret_cast<const char*>(Data + offset), 
                                       (Size - offset) > 30 ? 30 : (Size - offset));
        for (char& c : prepend_attr_name) {
            if (c == '\0') c = 'd';
        }
    } else {
        prepend_attr_name = "prepended_attr";
    }
    
    pugi::xml_attribute prepended = node.prepend_attribute(prepend_attr_name.c_str());
    if (prepended) {
        prepended.set_value("prepended_value");
    }
    
    // Additional state exploration: iterate through all attributes
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        (void)attr.name();
        (void)attr.value();
    }
    
    // Iterate backwards
    for (pugi::xml_attribute attr = node.last_attribute(); attr; attr = attr.previous_attribute()) {
        (void)attr.name();
        (void)attr.value();
    }
    
    return 0;
}