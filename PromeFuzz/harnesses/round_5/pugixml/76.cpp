// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
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
#include <string>

static void initialize_dummy_node(pugi::xml_node& node) {
    // Create some initial attributes for the node to work with
    node.append_attribute("attr1") = "value1";
    node.append_attribute("attr2") = "value2";
    node.append_attribute("attr3") = "value3";
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("test_node");
    
    // Initialize with some dummy attributes
    initialize_dummy_node(node);
    
    // Get existing attributes for reference
    pugi::xml_attribute attr1 = node.attribute("attr1");
    pugi::xml_attribute attr2 = node.attribute("attr2");
    pugi::xml_attribute attr3 = node.attribute("attr3");
    
    // Use the fuzzer input to create attribute names
    // Ensure we have at least 1 byte for name
    if (Size >= 1) {
        // Create a string from the fuzzer data (limited to reasonable length)
        size_t name_len = Size > 100 ? 100 : Size;
        std::string attr_name(reinterpret_cast<const char*>(Data), name_len);
        
        // Test 1: attribute with hint
        pugi::xml_attribute hint = attr1;
        pugi::xml_attribute found_attr = node.attribute(attr_name.c_str(), hint);
        (void)found_attr; // Use result to avoid unused variable warning
        
        // Test 2: prepend_attribute
        pugi::xml_attribute prepended = node.prepend_attribute(attr_name.c_str());
        (void)prepended;
        
        // Test 3: insert_copy_before
        if (attr2 && attr3) {
            pugi::xml_attribute copied = node.insert_copy_before(attr2, attr3);
            (void)copied;
        }
        
        // Test 4: insert_attribute_before
        if (attr1) {
            pugi::xml_attribute inserted_before = node.insert_attribute_before(attr_name.c_str(), attr1);
            (void)inserted_before;
        }
        
        // Test 5: insert_copy_after
        if (attr1 && attr2) {
            pugi::xml_attribute copied_after = node.insert_copy_after(attr1, attr2);
            (void)copied_after;
        }
        
        // Test 6: insert_attribute_after
        if (attr2) {
            pugi::xml_attribute inserted_after = node.insert_attribute_after(attr_name.c_str(), attr2);
            (void)inserted_after;
        }
    }
    
    // Additional test: try with different hint values
    if (Size >= 2) {
        std::string another_name(reinterpret_cast<const char*>(Data), Size > 50 ? 50 : Size);
        
        // Test attribute with different hints
        pugi::xml_attribute hint2 = attr2;
        node.attribute(another_name.c_str(), hint2);
        
        pugi::xml_attribute hint3 = attr3;
        node.attribute(another_name.c_str(), hint3);
        
        // Test with empty hint
        pugi::xml_attribute empty_hint;
        node.attribute(another_name.c_str(), empty_hint);
    }
    
    // Test edge cases with empty strings
    if (Size >= 3) {
        // Test with empty attribute name
        node.attribute("");
        
        // Test prepend with empty name
        node.prepend_attribute("");
        
        // Test insert with empty name
        if (attr1) {
            node.insert_attribute_before("", attr1);
            node.insert_attribute_after("", attr1);
        }
    }
    
    // Test with self-referential cases (if data allows)
    if (Size >= 4 && attr1) {
        // Try to insert a copy before itself (should fail but shouldn't crash)
        node.insert_copy_before(attr1, attr1);
    }
    
    return 0;
}