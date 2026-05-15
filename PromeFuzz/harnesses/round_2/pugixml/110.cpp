// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Add some initial attributes to the node
    pugi::xml_attribute attr1 = node.append_attribute("attr1");
    attr1.set_value("value1");
    
    pugi::xml_attribute attr2 = node.append_attribute("attr2");
    attr2.set_value("value2");
    
    // Create another attribute for copying operations
    pugi::xml_attribute attr3 = node.append_attribute("attr3");
    attr3.set_value("value3");
    
    // Use fuzzer data to create attribute names
    std::string_view data_view(reinterpret_cast<const char*>(Data), Size);
    
    // Split the input data into parts for different operations
    size_t split_point = Size / 2;
    std::string_view name_part1(data_view.data(), split_point);
    std::string_view name_part2(data_view.data() + split_point, Size - split_point);
    
    // Test 1: insert_copy_before
    if (!attr1.empty() && !attr2.empty()) {
        pugi::xml_attribute copy_before = node.insert_copy_before(attr1, attr2);
        (void)copy_before; // Use result to avoid unused variable warning
    }
    
    // Test 2: insert_attribute_before
    if (!attr1.empty() && !name_part1.empty()) {
        // Ensure we have a valid C-string
        std::string name_str1(name_part1);
        pugi::xml_attribute new_attr_before = node.insert_attribute_before(name_str1.c_str(), attr1);
        (void)new_attr_before;
    }
    
    // Test 3: insert_copy_after
    if (!attr2.empty() && !attr3.empty()) {
        pugi::xml_attribute copy_after = node.insert_copy_after(attr2, attr3);
        (void)copy_after;
    }
    
    // Test 4: insert_attribute_after
    if (!attr2.empty() && !name_part2.empty()) {
        std::string name_str2(name_part2);
        pugi::xml_attribute new_attr_after = node.insert_attribute_after(std::string_view(name_str2), attr2);
        (void)new_attr_after;
    }
    
    // Test 5: prepend_attribute
    if (!name_part1.empty()) {
        std::string prepend_name(name_part1);
        pugi::xml_attribute prepended = node.prepend_attribute(prepend_name.c_str());
        (void)prepended;
    }
    
    // Test 6: attribute
    if (!name_part2.empty()) {
        std::string lookup_name(name_part2);
        pugi::xml_attribute found = node.attribute(lookup_name.c_str());
        (void)found;
    }
    
    // Additional test: Try operations with empty/null attributes
    pugi::xml_attribute empty_attr;
    pugi::xml_node empty_node;
    
    // These should handle empty cases gracefully
    if (!empty_attr.empty()) {
        pugi::xml_attribute test1 = empty_node.insert_copy_before(empty_attr, empty_attr);
        (void)test1;
    }
    
    if (!empty_attr.empty()) {
        pugi::xml_attribute test2 = empty_node.insert_attribute_before("test", empty_attr);
        (void)test2;
    }
    
    return 0;
}