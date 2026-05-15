// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
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

static void test_xml_text_empty() {
    // Test with default-constructed xml_text (should be empty)
    pugi::xml_text text1;
    volatile bool result1 = text1.empty();
    
    // Test with text from a node
    pugi::xml_document doc;
    doc.append_child(pugi::node_pcdata).set_value("test");
    pugi::xml_text text2 = doc.first_child().text();
    volatile bool result2 = text2.empty();
    
    // Test with empty node
    pugi::xml_node empty_node;
    pugi::xml_text text3 = empty_node.text();
    volatile bool result3 = text3.empty();
}

static void test_xml_attribute_empty() {
    // Test with default-constructed attribute (should be empty)
    pugi::xml_attribute attr1;
    volatile bool result1 = attr1.empty();
    
    // Test with attribute from a node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("node");
    node.append_attribute("attr") = "value";
    pugi::xml_attribute attr2 = node.attribute("attr");
    volatile bool result2 = attr2.empty();
    
    // Test with non-existent attribute
    pugi::xml_attribute attr3 = node.attribute("nonexistent");
    volatile bool result3 = attr3.empty();
}

static void test_xpath_node_set_empty() {
    // Test with empty set
    pugi::xpath_node_set set1(nullptr, nullptr);
    volatile bool result1 = set1.empty();
    
    // Test with single node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("node");
    pugi::xpath_node xnode(node);
    pugi::xpath_node_set set2(&xnode, &xnode + 1);
    volatile bool result2 = set2.empty();
    
    // Test with multiple nodes
    pugi::xml_node node2 = doc.append_child("node2");
    pugi::xpath_node xnodes[] = {pugi::xpath_node(node), pugi::xpath_node(node2)};
    pugi::xpath_node_set set3(xnodes, xnodes + 2);
    volatile bool result3 = set3.empty();
}

static void test_xml_node_remove_attributes() {
    // Test with empty node
    pugi::xml_node empty_node;
    volatile bool result1 = empty_node.remove_attributes();
    
    // Test with node that has attributes
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("node");
    node.append_attribute("attr1") = "value1";
    node.append_attribute("attr2") = "value2";
    volatile bool result2 = node.remove_attributes();
    
    // Test with node that has no attributes
    pugi::xml_node node2 = doc.append_child("node2");
    volatile bool result3 = node2.remove_attributes();
}

static void test_xml_object_range_empty() {
    // Test with empty range
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("node");
    auto range1 = node.attributes();
    volatile bool result1 = range1.empty();
    
    // Test with non-empty range
    node.append_attribute("attr") = "value";
    auto range2 = node.attributes();
    volatile bool result2 = range2.empty();
    
    // Test with children range
    auto range3 = node.children();
    volatile bool result3 = range3.empty();
    
    node.append_child("child");
    auto range4 = node.children();
    volatile bool result4 = range4.empty();
}

static void test_xml_node_empty() {
    // Test with default-constructed node (should be empty)
    pugi::xml_node node1;
    volatile bool result1 = node1.empty();
    
    // Test with valid node
    pugi::xml_document doc;
    pugi::xml_node node2 = doc.append_child("node");
    volatile bool result2 = node2.empty();
    
    // Test with document node
    volatile bool result3 = doc.empty();
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to create different XML documents
    if (Size > 0) {
        // Create a dummy XML string from the input
        std::string xml_data(reinterpret_cast<const char*>(Data), Size);
        
        // Try to parse the input as XML
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer_inplace(&xml_data[0], xml_data.size());
        
        // Test all functions regardless of parse result
        test_xml_text_empty();
        test_xml_attribute_empty();
        test_xpath_node_set_empty();
        test_xml_node_remove_attributes();
        test_xml_object_range_empty();
        test_xml_node_empty();
        
        // Additional tests with the parsed document if successful
        if (result) {
            // Get the root node and test functions on it
            pugi::xml_node root = doc.document_element();
            if (!root.empty()) {
                // Test remove_attributes on the root
                volatile bool remove_result = root.remove_attributes();
                
                // Test empty() on various objects from the document
                volatile bool node_empty = root.empty();
                volatile bool text_empty = root.text().empty();
                
                // Get first attribute and test empty()
                pugi::xml_attribute first_attr = root.first_attribute();
                volatile bool attr_empty = first_attr.empty();
                
                // Create an xpath_node_set from the document's nodes
                pugi::xpath_node xnode(root);
                pugi::xpath_node_set xset(&xnode, &xnode + 1);
                volatile bool xset_empty = xset.empty();
                
                // Test xml_object_range on attributes and children
                auto attr_range = root.attributes();
                volatile bool attr_range_empty = attr_range.empty();
                
                auto child_range = root.children();
                volatile bool child_range_empty = child_range.empty();
            }
        }
    } else {
        // Test with empty input
        test_xml_text_empty();
        test_xml_attribute_empty();
        test_xpath_node_set_empty();
        test_xml_node_remove_attributes();
        test_xml_object_range_empty();
        test_xml_node_empty();
    }
    
    return 0;
}