// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <cstring>
#include <string>

static void initialize_test_document(pugi::xml_document& doc) {
    // Create a simple XML structure to work with
    pugi::xml_node root = doc.append_child("root");
    root.append_attribute("attr1") = "value1";
    root.append_attribute("attr2") = "value2";
    root.append_attribute("attr3") = "value3";
    
    pugi::xml_node child = root.append_child("child");
    child.append_attribute("child_attr") = "child_value";
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    initialize_test_document(doc);
    
    // Get the root node to work with
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    // Get some existing attributes for reference
    pugi::xml_attribute attr1 = root.attribute("attr1");
    pugi::xml_attribute attr2 = root.attribute("attr2");
    pugi::xml_attribute attr3 = root.attribute("attr3");
    
    // Use fuzzer input to generate strings for attribute names
    // Limit size to avoid excessive memory usage
    size_t name_len = Size > 100 ? 100 : Size;
    std::string attr_name(reinterpret_cast<const char*>(Data), name_len);
    
    // Test 1: xml_node::attribute
    {
        pugi::xml_attribute result = root.attribute(attr_name.c_str());
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test 2: xml_node::insert_attribute_before
    {
        if (attr2) {
            std::string_view name_sv(attr_name);
            pugi::xml_attribute result = root.insert_attribute_before(name_sv, attr2);
            (void)result;
        }
    }
    
    // Test 3: xml_node::insert_copy_after
    {
        if (attr1 && attr2) {
            pugi::xml_attribute result = root.insert_copy_after(attr1, attr2);
            (void)result;
        }
    }
    
    // Test 4: xml_node::prepend_attribute
    {
        pugi::xml_attribute result = root.prepend_attribute(attr_name.c_str());
        (void)result;
    }
    
    // Test 5: xml_node::insert_attribute_after
    {
        if (attr3) {
            pugi::xml_attribute result = root.insert_attribute_after(attr_name.c_str(), attr3);
            (void)result;
        }
    }
    
    // Test 6: xml_node::insert_copy_before
    {
        if (attr1 && attr3) {
            pugi::xml_attribute result = root.insert_copy_before(attr1, attr3);
            (void)result;
        }
    }
    
    // Additional test: Try to use the functions with empty nodes
    {
        pugi::xml_node empty_node;
        pugi::xml_attribute empty_attr;
        
        // These should all return empty attributes
        pugi::xml_attribute result1 = empty_node.attribute("test");
        pugi::xml_attribute result2 = empty_node.insert_attribute_before("test", empty_attr);
        pugi::xml_attribute result3 = empty_node.insert_copy_after(empty_attr, empty_attr);
        pugi::xml_attribute result4 = empty_node.prepend_attribute("test");
        pugi::xml_attribute result5 = empty_node.insert_attribute_after("test", empty_attr);
        pugi::xml_attribute result6 = empty_node.insert_copy_before(empty_attr, empty_attr);
        
        (void)result1; (void)result2; (void)result3;
        (void)result4; (void)result5; (void)result6;
    }
    
    return 0;
}