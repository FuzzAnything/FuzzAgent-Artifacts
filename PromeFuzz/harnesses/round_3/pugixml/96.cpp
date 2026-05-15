// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
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
#include <stdint.h>
#include <string>
#include <cstring>

static pugi::xml_node create_test_node(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    pugi::xml_node node = doc.append_child("test_node");
    
    // Add some initial attributes using the fuzzer data
    size_t attr_count = size > 0 ? (data[0] % 5) + 1 : 3;
    for (size_t i = 0; i < attr_count && i < size; ++i) {
        char attr_name[32];
        char attr_value[32];
        snprintf(attr_name, sizeof(attr_name), "attr_%zu", i);
        snprintf(attr_value, sizeof(attr_value), "value_%02x", data[i % size]);
        node.append_attribute(attr_name).set_value(attr_value);
    }
    
    return node;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* data, size_t size) {
    static pugi::xml_document doc;
    doc.reset();
    
    // Create a test node with some initial attributes
    pugi::xml_node node = create_test_node(doc, data, size);
    
    // 1. Test first_attribute
    pugi::xml_attribute first_attr = node.first_attribute();
    
    // 2. Test last_attribute
    pugi::xml_attribute last_attr = node.last_attribute();
    
    // 3. Test attribute by name
    if (size > 0) {
        size_t name_len = size % 32;
        if (name_len > 0) {
            std::string attr_name(reinterpret_cast<const char*>(data), 
                                 std::min(name_len, size));
            pugi::xml_attribute found_attr = node.attribute(attr_name.c_str());
        }
    }
    
    // 4. Test find_attribute with a predicate
    auto pred = [](pugi::xml_attribute attr) {
        return attr && attr.value() && std::strstr(attr.value(), "value") != nullptr;
    };
    pugi::xml_attribute found_by_pred = node.find_attribute(pred);
    
    // 5. Test insert_attribute_before (if we have a reference attribute)
    if (first_attr) {
        std::string new_attr_name = "inserted_attr";
        if (size > 0) {
            size_t name_len = size % 16;
            if (name_len > 0) {
                new_attr_name = std::string(reinterpret_cast<const char*>(data), 
                                           std::min(name_len, size));
            }
        }
        
        pugi::xml_attribute inserted = node.insert_attribute_before(new_attr_name.c_str(), first_attr);
        
        // Set a value for the inserted attribute
        if (inserted) {
            std::string attr_value = "inserted_value";
            if (size > 1) {
                size_t value_len = size % 16;
                size_t offset = 1;
                if (value_len > 0 && offset < size) {
                    size_t actual_len = std::min(value_len, size - offset);
                    attr_value = std::string(reinterpret_cast<const char*>(data + offset), 
                                            actual_len);
                }
            }
            inserted.set_value(attr_value.c_str());
        }
        
        // 6. Test remove_attribute on the inserted attribute
        if (inserted) {
            bool removed = node.remove_attribute(inserted);
        }
    }
    
    // Clean up by removing all attributes
    pugi::xml_attribute attr = node.first_attribute();
    while (attr) {
        pugi::xml_attribute next = attr.next_attribute();
        node.remove_attribute(attr);
        attr = next;
    }
    
    return 0;
}