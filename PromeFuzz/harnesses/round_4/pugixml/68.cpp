// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Add some initial attributes to work with
    node.append_attribute("attr1").set_value("value1");
    node.append_attribute("attr2").set_value("value2");
    node.append_attribute("attr3").set_value("value3");
    
    // Use fuzzer data to create attribute names/values
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);
    
    if (Size > 0) {
        // Test attribute retrieval
        pugi::xml_attribute attr1 = node.attribute("attr1");
        pugi::xml_attribute attr_nonexistent = node.attribute(input_str.data());
        
        // Test first and last attributes
        pugi::xml_attribute first_attr = node.first_attribute();
        pugi::xml_attribute last_attr = node.last_attribute();
        
        // Test insert_attribute_before - insert before first attribute
        if (first_attr) {
            pugi::xml_attribute new_attr_before = node.insert_attribute_before(
                input_str.data(), first_attr);
        }
        
        // Test insert_attribute_after - insert after first attribute
        if (first_attr) {
            pugi::xml_attribute new_attr_after = node.insert_attribute_after(
                input_str.data(), first_attr);
        }
        
        // Test remove_attribute - try to remove first attribute
        if (first_attr) {
            bool removed = node.remove_attribute(first_attr);
            
            // If removal succeeded, try to remove a non-existent attribute
            if (removed) {
                pugi::xml_attribute dummy_attr;
                node.remove_attribute(dummy_attr);
            }
        }
        
        // Test attribute retrieval with different names
        // Split input to create multiple attribute names
        if (Size > 1) {
            std::string_view name1(input_str.data(), Size/2);
            std::string_view name2(input_str.data() + Size/2, Size - Size/2);
            
            pugi::xml_attribute test_attr1 = node.attribute(name1.data());
            pugi::xml_attribute test_attr2 = node.attribute(name2.data());
            
            // Try to remove attributes with these names
            if (test_attr1) {
                node.remove_attribute(test_attr1);
            }
            if (test_attr2) {
                node.remove_attribute(test_attr2);
            }
        }
        
        // Test edge cases with empty strings
        pugi::xml_attribute empty_attr = node.attribute("");
        
        // Test insert with empty name
        if (last_attr) {
            pugi::xml_attribute empty_name_attr = node.insert_attribute_before(
                "", last_attr);
        }
    }
    
    // Clean up - remove all attributes
    while (node.first_attribute()) {
        node.remove_attribute(node.first_attribute());
    }
    
    // Test functions on empty node
    pugi::xml_attribute empty_first = node.first_attribute();
    pugi::xml_attribute empty_last = node.last_attribute();
    pugi::xml_attribute empty_find = node.attribute("nonexistent");
    
    // Try to insert when there are no attributes (should fail)
    pugi::xml_attribute dummy;
    pugi::xml_attribute insert_empty = node.insert_attribute_before("test", dummy);
    
    return 0;
}