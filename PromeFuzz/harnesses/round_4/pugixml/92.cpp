// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
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
#include <string_view>

static pugi::xml_node create_test_node(pugi::xml_document& doc) {
    // Create a root node with some structure for testing
    pugi::xml_node root = doc.append_child("root");
    
    // Add some attributes
    root.append_attribute("attr1") = "value1";
    root.append_attribute("attr2") = "value2";
    root.append_attribute("attr3") = "value3";
    
    // Add some child nodes
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    
    return root;
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    // Create a document and test node
    pugi::xml_document doc;
    pugi::xml_node test_node = create_test_node(doc);
    
    // Test 1: remove_attributes
    (void)test_node.remove_attributes();
    
    // Re-add attributes for next tests
    test_node.append_attribute("attr1") = "value1";
    test_node.append_attribute("attr2") = "value2";
    
    // Test 2: remove_children
    (void)test_node.remove_children();
    
    // Re-add children for next tests
    test_node.append_child("child1");
    test_node.append_child("child2");
    
    // Test 3: remove_attribute - both overloads
    if (Size > 0) {
        // Create a string from the fuzzer input for attribute name
        std::string attr_name(reinterpret_cast<const char*>(Data), 
                             std::min(Size, static_cast<size_t>(100)));
        
        // Test with std::string_view
        std::string_view attr_sv(attr_name);
        (void)test_node.remove_attribute(attr_sv);
        
        // Test with const char*
        (void)test_node.remove_attribute(attr_name.c_str());
    }
    
    // Test 4: remove_child - both overloads
    if (Size > 0) {
        // Create a string from the fuzzer input for child name
        std::string child_name(reinterpret_cast<const char*>(Data), 
                              std::min(Size, static_cast<size_t>(100)));
        
        // Test with std::string_view
        std::string_view child_sv(child_name);
        (void)test_node.remove_child(child_sv);
        
        // Test with const char*
        (void)test_node.remove_child(child_name.c_str());
    }
    
    // Additional test: Try to remove attributes and children from empty node
    pugi::xml_node empty_node;
    (void)empty_node.remove_attributes();
    (void)empty_node.remove_children();
    
    // Test with different node states
    pugi::xml_document doc2;
    pugi::xml_node node2 = doc2.append_child("test");
    
    // Add many attributes and children to test memory allocation paths
    for (int i = 0; i < 10; ++i) {
        std::string attr_name = "attr" + std::to_string(i);
        node2.append_attribute(attr_name.c_str()) = "value";
        
        std::string child_name = "child" + std::to_string(i);
        node2.append_child(child_name.c_str());
    }
    
    // Test remove_attributes on node with many attributes
    (void)node2.remove_attributes();
    
    // Test remove_children on node with many children
    (void)node2.remove_children();
    
    return 0;
}