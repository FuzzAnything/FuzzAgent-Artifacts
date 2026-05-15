// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Add some initial attributes to the node for testing
    pugi::xml_attribute attr1 = node.append_attribute("attr1");
    attr1.set_value("value1");
    pugi::xml_attribute attr2 = node.append_attribute("attr2");
    attr2.set_value("value2");
    pugi::xml_attribute attr3 = node.append_attribute("attr3");
    attr3.set_value("value3");
    
    // Use fuzz data to create strings for attribute names
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }
    
    // Create attribute name from fuzz data (limit length to avoid huge strings)
    size_t name_len = Size > 100 ? 100 : Size;
    std::string attr_name(reinterpret_cast<const char*>(Data), name_len);
    
    // Test 1: xml_node::attribute
    pugi::xml_attribute retrieved_attr = node.attribute(attr_name.c_str());
    (void)retrieved_attr; // Use variable to avoid unused warning
    
    // Test 2: xml_node::prepend_attribute
    pugi::xml_attribute prepended_attr = node.prepend_attribute(attr_name.c_str());
    if (prepended_attr) {
        prepended_attr.set_value("prepended_value");
    }
    
    // Test 3: xml_node::insert_copy_before
    // Use existing attributes for copying
    if (attr1 && attr2) {
        pugi::xml_attribute copied_attr = node.insert_copy_before(attr1, attr2);
        (void)copied_attr;
    }
    
    // Test 4: xml_node::insert_attribute_before
    // Create another name for new attribute
    std::string_view new_attr_name(attr_name);
    if (attr2) {
        pugi::xml_attribute inserted_before_attr = node.insert_attribute_before(new_attr_name, attr2);
        if (inserted_before_attr) {
            inserted_before_attr.set_value("inserted_before_value");
        }
    }
    
    // Test 5: xml_node::insert_copy_after
    if (attr2 && attr3) {
        pugi::xml_attribute copied_after_attr = node.insert_copy_after(attr2, attr3);
        (void)copied_after_attr;
    }
    
    // Test 6: xml_node::insert_attribute_after
    if (attr1) {
        pugi::xml_attribute inserted_after_attr = node.insert_attribute_after(new_attr_name, attr1);
        if (inserted_after_attr) {
            inserted_after_attr.set_value("inserted_after_value");
        }
    }
    
    // Try to retrieve attributes with different names to explore more states
    if (Size > 1) {
        std::string another_name(reinterpret_cast<const char*>(Data) + 1, 
                                (Size - 1) > 50 ? 50 : (Size - 1));
        node.attribute(another_name.c_str());
    }
    
    return 0;
}