// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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

static pugi::xml_attribute get_nth_attribute(pugi::xml_node& node, size_t index) {
    pugi::xml_attribute attr = node.first_attribute();
    for (size_t i = 0; i < index && attr; ++i) {
        attr = attr.next_attribute();
    }
    return attr;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a document with some initial structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some initial attributes to work with
    root.append_attribute("attr1").set_value("value1");
    root.append_attribute("attr2").set_value("value2");
    root.append_attribute("attr3").set_value("value3");
    
    // Use input data to create strings for attribute names
    std::string name1, name2;
    if (Size >= 2) {
        name1 = std::string(reinterpret_cast<const char*>(Data), 1);
        name2 = std::string(reinterpret_cast<const char*>(Data + 1), 1);
    } else {
        name1 = "a";
        name2 = "b";
    }
    
    // Get some existing attributes to use as references
    pugi::xml_attribute attr1 = root.attribute("attr1");
    pugi::xml_attribute attr2 = root.attribute("attr2");
    pugi::xml_attribute attr3 = root.attribute("attr3");
    
    // Test 1: insert_copy_before
    if (attr1 && attr2) {
        root.insert_copy_before(attr1, attr2);
    }
    
    // Test 2: insert_attribute_before
    if (attr2) {
        root.insert_attribute_before(name1.c_str(), attr2);
    }
    
    // Test 3: insert_copy_after
    if (attr2 && attr3) {
        root.insert_copy_after(attr2, attr3);
    }
    
    // Test 4: attribute lookup
    root.attribute(name1.c_str());
    root.attribute(name2.c_str());
    
    // Test 5: insert_attribute_after
    if (attr1) {
        std::string_view name_sv(name1);
        root.insert_attribute_after(name_sv, attr1);
    }
    
    // Test 6: prepend_attribute
    root.prepend_attribute(name2.c_str());
    
    // Additional tests with different combinations
    if (Size >= 3) {
        // Use more data for additional attribute names
        std::string name3(reinterpret_cast<const char*>(Data + 2), 
                         std::min(static_cast<size_t>(10), Size - 2));
        
        // Try to get attribute by the generated name
        pugi::xml_attribute found = root.attribute(name3.c_str());
        
        // If we found an attribute, use it in insert operations
        if (found) {
            // Create another name for new attribute
            std::string name4 = "new_" + name3;
            root.insert_attribute_before(name4.c_str(), found);
            
            // Try insert_copy operations with this attribute
            if (attr1) {
                root.insert_copy_before(found, attr1);
            }
        }
        
        // Test prepend with the generated name
        root.prepend_attribute(name3.c_str());
    }
    
    // Test edge cases with empty attributes
    pugi::xml_attribute empty_attr;
    if (attr1) {
        // This should fail gracefully
        root.insert_copy_before(empty_attr, attr1);
        root.insert_copy_after(empty_attr, attr1);
    }
    
    // Clean up - let the document go out of scope
    return 0;
}