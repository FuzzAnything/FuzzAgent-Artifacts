// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with some content
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some attributes to the root node using the fuzzer data
    if (Size > 0) {
        // Use first byte to determine how many attributes to add (1-5)
        size_t num_attrs = (Data[0] % 5) + 1;
        size_t data_index = 1;
        
        for (size_t i = 0; i < num_attrs && data_index < Size; i++) {
            // Create attribute name from fuzzer data
            size_t name_len = std::min<size_t>((Data[data_index] % 10) + 1, Size - data_index - 1);
            std::string_view attr_name(reinterpret_cast<const char*>(&Data[data_index]), name_len);
            data_index += name_len;
            
            if (data_index < Size) {
                // Create attribute value from fuzzer data
                size_t value_len = std::min<size_t>((Data[data_index] % 10) + 1, Size - data_index);
                std::string_view attr_value(reinterpret_cast<const char*>(&Data[data_index]), value_len);
                data_index += value_len;
                
                pugi::xml_attribute attr = root.append_attribute(attr_name.data());
                attr.set_value(attr_value.data());
            }
        }
    }
    
    // Test 1: find_attribute with lambda predicate
    static auto name_predicate = [](pugi::xml_attribute attr) {
        return std::string_view(attr.name()) == "test";
    };
    pugi::xml_attribute found_attr = root.find_attribute(name_predicate);
    
    // Test 2: attribute by name
    if (Size > 0) {
        size_t name_len = std::min<size_t>(Size, 20);
        std::string_view attr_name(reinterpret_cast<const char*>(Data), name_len);
        pugi::xml_attribute attr_by_name = root.attribute(attr_name.data());
    }
    
    // Test 3: prepend_attribute
    if (Size > 10) {
        std::string_view prepend_name(reinterpret_cast<const char*>(&Data[5]), 5);
        pugi::xml_attribute prepended = root.prepend_attribute(prepend_name.data());
        if (prepended) {
            prepended.set_value("prepended_value");
        }
    }
    
    // Test 4: first_attribute
    pugi::xml_attribute first = root.first_attribute();
    
    // Test 5: insert_attribute_before
    if (first) {
        std::string_view insert_name(reinterpret_cast<const char*>(Data), std::min<size_t>(Size, 8));
        pugi::xml_attribute inserted = root.insert_attribute_before(insert_name.data(), first);
        if (inserted) {
            inserted.set_value("inserted_value");
        }
    }
    
    // Test 6: last_attribute
    pugi::xml_attribute last = root.last_attribute();
    
    // Also test on child nodes
    pugi::xml_node child = root.append_child("child");
    if (Size > 20) {
        std::string_view child_attr_name(reinterpret_cast<const char*>(&Data[10]), 5);
        child.append_attribute(child_attr_name.data()).set_value("child_value");
        
        // Test functions on child node
        pugi::xml_attribute child_attr = child.attribute(child_attr_name.data());
        pugi::xml_attribute child_first = child.first_attribute();
        pugi::xml_attribute child_last = child.last_attribute();
    }
    
    // Clean up - document will be destroyed automatically when it goes out of scope
    return 0;
}