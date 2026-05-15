// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Create document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Create some initial attributes using the fuzzer data
    if (Size > 0) {
        // Use first byte to determine how many attributes to create
        size_t num_attrs = (Data[0] % 5) + 1;
        size_t data_index = 1;
        
        for (size_t i = 0; i < num_attrs && data_index < Size; i++) {
            // Create attribute name from fuzzer data
            size_t name_len = (Data[data_index] % 16) + 1;
            data_index = (data_index + 1) % Size;
            
            std::string attr_name;
            for (size_t j = 0; j < name_len && data_index < Size; j++) {
                attr_name.push_back(static_cast<char>(Data[data_index]));
                data_index = (data_index + 1) % Size;
            }
            
            if (!attr_name.empty()) {
                node.append_attribute(attr_name.c_str());
            }
        }
    }
    
    // Get existing attributes for operations
    pugi::xml_attribute first_attr = node.first_attribute();
    pugi::xml_attribute last_attr = node.last_attribute();
    
    // Test 1: insert_copy_before
    if (first_attr && last_attr && first_attr != last_attr) {
        static pugi::xml_attribute copied_attr = node.insert_copy_before(last_attr, first_attr);
    }
    
    // Test 2: insert_attribute_before
    if (first_attr) {
        std::string_view new_name = "fuzz_attr_before";
        static pugi::xml_attribute new_attr_before = node.insert_attribute_before(new_name, first_attr);
    }
    
    // Test 3: insert_copy_after
    if (first_attr && last_attr && first_attr != last_attr) {
        static pugi::xml_attribute copied_attr_after = node.insert_copy_after(first_attr, last_attr);
    }
    
    // Test 4: attribute (retrieve by name)
    if (Size > 0) {
        // Create attribute name from fuzzer data
        size_t name_len = Size % 32;
        std::string search_name;
        for (size_t i = 0; i < name_len && i < Size; i++) {
            search_name.push_back(static_cast<char>(Data[i]));
        }
        
        if (!search_name.empty()) {
            static pugi::xml_attribute found_attr = node.attribute(search_name.c_str());
        }
    }
    
    // Test 5: insert_attribute_after
    if (last_attr) {
        std::string_view new_name = "fuzz_attr_after";
        static pugi::xml_attribute new_attr_after = node.insert_attribute_after(new_name, last_attr);
    }
    
    // Test 6: prepend_attribute
    std::string_view prepend_name = "fuzz_prepend";
    static pugi::xml_attribute prepended_attr = node.prepend_attribute(prepend_name.data());
    
    // Additional tests with different data patterns
    if (Size > 1) {
        // Try to create attribute with name from fuzzer data
        std::string dynamic_name;
        for (size_t i = 0; i < Size && i < 50; i++) {
            dynamic_name.push_back(static_cast<char>(Data[i]));
        }
        
        if (!dynamic_name.empty()) {
            // Try prepend with dynamic name
            static pugi::xml_attribute dyn_prepend = node.prepend_attribute(dynamic_name.c_str());
            
            // Try to find it
            static pugi::xml_attribute dyn_found = node.attribute(dynamic_name.c_str());
            
            // Try insert operations with dynamic names if we have attributes
            if (first_attr) {
                std::string_view dyn_sv = dynamic_name;
                static pugi::xml_attribute dyn_insert_before = node.insert_attribute_before(dyn_sv, first_attr);
                
                if (last_attr) {
                    static pugi::xml_attribute dyn_insert_after = node.insert_attribute_after(dyn_sv, last_attr);
                }
            }
        }
    }
    
    // Test copy operations with various attribute combinations
    pugi::xml_attribute current = node.first_attribute();
    while (current) {
        pugi::xml_attribute next = current.next_attribute();
        if (next) {
            // Try copying before next
            static pugi::xml_attribute copy1 = node.insert_copy_before(current, next);
            
            // Try copying after current
            static pugi::xml_attribute copy2 = node.insert_copy_after(next, current);
        }
        current = next;
    }
    
    return 0;
}