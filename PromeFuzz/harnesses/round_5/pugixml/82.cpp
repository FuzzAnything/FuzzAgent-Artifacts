// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>

static void test_xml_object_range_empty() {
    // Test with default constructed range
    pugi::xml_node_iterator it1, it2;
    pugi::xml_object_range<pugi::xml_node_iterator> range1(it1, it2);
    bool result1 = range1.empty();
    (void)result1;
    
    // Test with document that may have nodes
    pugi::xml_document doc;
    if (doc.load_string("<root><child1/><child2/></root>")) {
        pugi::xml_node root = doc.root();
        auto children = root.children();
        bool result2 = children.empty();
        (void)result2;
    }
}

static void test_xml_node_remove_attributes(pugi::xml_node& node) {
    bool result = node.remove_attributes();
    (void)result;
}

static void test_xml_text_empty(pugi::xml_text& text) {
    bool result = text.empty();
    (void)result;
}

static void test_xml_node_remove_children(pugi::xml_node& node) {
    bool result = node.remove_children();
    (void)result;
}

static void test_xml_attribute_empty(pugi::xml_attribute& attr) {
    bool result = attr.empty();
    (void)result;
}

static void test_xml_node_empty(pugi::xml_node& node) {
    bool result = node.empty();
    (void)result;
}

static void test_with_fuzzed_data(const uint8_t* data, size_t size) {
    // Try to parse the fuzzed input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    if (parse_result) {
        // Test on document root
        pugi::xml_node root = doc.root();
        test_xml_node_empty(root);
        test_xml_node_remove_attributes(root);
        test_xml_node_remove_children(root);
        
        // Test on all nodes in document
        for (pugi::xml_node node : doc) {
            test_xml_node_empty(node);
            test_xml_node_remove_attributes(node);
            test_xml_node_remove_children(node);
            
            // Test text content
            pugi::xml_text text = node.text();
            test_xml_text_empty(text);
            
            // Test all attributes
            for (pugi::xml_attribute attr : node.attributes()) {
                test_xml_attribute_empty(attr);
            }
            
            // Test children ranges
            auto children = node.children();
            bool range_empty = children.empty();
            (void)range_empty;
        }
        
        // Test xml_object_range::empty on various ranges
        test_xml_object_range_empty();
    }
    
    // Also test with empty/default constructed objects
    pugi::xml_node empty_node;
    test_xml_node_empty(empty_node);
    test_xml_node_remove_attributes(empty_node);
    test_xml_node_remove_children(empty_node);
    
    pugi::xml_attribute empty_attr;
    test_xml_attribute_empty(empty_attr);
    
    pugi::xml_text empty_text;
    test_xml_text_empty(empty_text);
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    // Handle edge cases
    if (Size == 0) {
        // Test with empty input
        test_with_fuzzed_data(Data, Size);
        return 0;
    }
    
    // Make a copy of the data to ensure null termination for string operations
    uint8_t* data_copy = static_cast<uint8_t*>(malloc(Size + 1));
    if (!data_copy) return 0;
    
    memcpy(data_copy, Data, Size);
    data_copy[Size] = 0;
    
    test_with_fuzzed_data(data_copy, Size);
    
    free(data_copy);
    return 0;
}