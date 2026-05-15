// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    // Initialize a dummy XML document to get valid xml_attribute and xml_node objects
    pugi::xml_document doc;
    
    // Create a node and attribute to work with (non-empty)
    pugi::xml_node node = doc.append_child(pugi::node_element);
    pugi::xml_attribute attr = node.append_attribute("initial_attr");
    
    // Also create default-constructed empty objects
    pugi::xml_attribute empty_attr;
    pugi::xml_node empty_node;
    
    // Convert fuzzer input to string view for API calls
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test 1: pugi::xml_attribute::empty
    // Test on both empty and non-empty attributes
    bool empty1 = empty_attr.empty();
    bool empty2 = attr.empty();
    
    // Test 2: pugi::xml_attribute::set_name (both overloads)
    if (Size > 0) {
        // First overload with size parameter
        bool result1 = attr.set_name(input_str.data(), input_str.size());
        
        // Second overload without size parameter (requires null-terminated string)
        // Create a null-terminated copy of the input
        char* null_term_str = static_cast<char*>(malloc(Size + 1));
        if (null_term_str) {
            memcpy(null_term_str, Data, Size);
            null_term_str[Size] = '\0';
            bool result2 = attr.set_name(null_term_str);
            free(null_term_str);
        }
        
        // Also test on empty attribute (should fail)
        if (Size > 0) {
            empty_attr.set_name(input_str.data(), input_str.size());
        }
    }
    
    // Test 3: pugi::xml_node::set_name
    if (Size > 0) {
        bool result3 = node.set_name(input_str.data(), input_str.size());
        
        // Also test on empty node (should fail)
        empty_node.set_name(input_str.data(), input_str.size());
    }
    
    // Test 4: pugi::xml_attribute::set_value
    if (Size > 0) {
        bool result4 = attr.set_value(input_str.data(), input_str.size());
        
        // Also test on empty attribute (should fail)
        empty_attr.set_value(input_str.data(), input_str.size());
    }
    
    // Test 5: pugi::xml_attribute::name
    const char* name1 = attr.name();
    const char* name2 = empty_attr.name();
    
    // Additional testing: create attribute from internal pointer
    if (Size > 0) {
        // Create another document and get internal structures
        pugi::xml_document doc2;
        pugi::xml_node node2 = doc2.append_child(pugi::node_element);
        pugi::xml_attribute attr2 = node2.append_attribute("test");
        
        // Test with constructed attribute from internal pointer
        pugi::xml_attribute_struct* attr_struct = attr2.internal_object();
        if (attr_struct) {
            pugi::xml_attribute attr3(attr_struct);
            bool test_empty = attr3.empty();
            const char* test_name = attr3.name();
            
            if (Size > 0) {
                attr3.set_name(input_str.data(), input_str.size());
                attr3.set_value(input_str.data(), input_str.size());
            }
        }
        
        // Test with constructed node from internal pointer
        pugi::xml_node_struct* node_struct = node2.internal_object();
        if (node_struct) {
            pugi::xml_node node3(node_struct);
            if (Size > 0) {
                node3.set_name(input_str.data(), input_str.size());
            }
        }
    }
    
    return 0;
}