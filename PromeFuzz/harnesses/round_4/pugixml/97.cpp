// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
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

static void test_xml_text_empty(const uint8_t* data, size_t size) {
    // Create XML document and parse input
    pugi::xml_document doc;
    if (size > 0) {
        // Use data to create XML content
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        // Try to parse as XML
        pugi::xml_parse_result result = doc.load_string(buffer);
        delete[] buffer;
        
        if (result) {
            // Get root node and its text
            pugi::xml_node root = doc.root();
            pugi::xml_text text = root.text();
            bool empty = text.empty();
            (void)empty; // Use result to avoid optimization
        }
    }
    
    // Test with default-constructed text
    pugi::xml_text default_text;
    bool default_empty = default_text.empty();
    (void)default_empty;
}

static void test_xml_node_empty(const uint8_t* data, size_t size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Test with default-constructed node
    pugi::xml_node default_node;
    bool default_empty = default_node.empty();
    (void)default_empty;
    
    if (size > 0) {
        // Try to parse input as XML
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc.load_string(buffer);
        delete[] buffer;
        
        if (result) {
            // Test with valid nodes
            pugi::xml_node root = doc.root();
            bool root_empty = root.empty();
            (void)root_empty;
            
            // Test child nodes
            for (pugi::xml_node child : root.children()) {
                bool child_empty = child.empty();
                (void)child_empty;
            }
        }
    }
}

static void test_xpath_node_set_empty(const uint8_t* data, size_t size) {
    // Test default-constructed set
    pugi::xpath_node_set default_set;
    bool default_empty = default_set.empty();
    (void)default_empty;
    
    // Create XML document
    pugi::xml_document doc;
    if (size > 0) {
        // Try to parse input
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc.load_string(buffer);
        delete[] buffer;
        
        if (result) {
            // Try to evaluate XPath queries
            try {
                pugi::xpath_node_set set1 = doc.select_nodes("//*");
                bool empty1 = set1.empty();
                (void)empty1;
                
                pugi::xpath_node_set set2 = doc.select_nodes("/nonexistent");
                bool empty2 = set2.empty();
                (void)empty2;
            } catch (...) {
                // Ignore XPath exceptions
            }
        }
    }
}

static void test_xml_attribute_empty(const uint8_t* data, size_t size) {
    // Test default-constructed attribute
    pugi::xml_attribute default_attr;
    bool default_empty = default_attr.empty();
    (void)default_empty;
    
    // Create XML document
    pugi::xml_document doc;
    if (size > 0) {
        // Try to parse input
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc.load_string(buffer);
        delete[] buffer;
        
        if (result) {
            pugi::xml_node root = doc.root();
            
            // Test attributes
            pugi::xml_attribute first_attr = root.first_attribute();
            bool first_empty = first_attr.empty();
            (void)first_empty;
            
            // Test all attributes
            for (pugi::xml_attribute attr : root.attributes()) {
                bool attr_empty = attr.empty();
                (void)attr_empty;
            }
        }
    }
}

static void test_xml_node_remove_attributes(const uint8_t* data, size_t size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Test with default-constructed node
    pugi::xml_node default_node;
    bool default_result = default_node.remove_attributes();
    (void)default_result;
    
    if (size > 0) {
        // Try to parse input
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc.load_string(buffer);
        delete[] buffer;
        
        if (result) {
            pugi::xml_node root = doc.root();
            
            // Try to remove attributes
            bool remove_result = root.remove_attributes();
            (void)remove_result;
            
            // Try on child nodes
            for (pugi::xml_node child : root.children()) {
                bool child_result = child.remove_attributes();
                (void)child_result;
            }
        }
    }
}

static void test_xml_object_range_empty(const uint8_t* data, size_t size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Test with default-constructed ranges
    pugi::xml_node::iterator it1, it2;
    pugi::xml_object_range<pugi::xml_node::iterator> default_range(it1, it2);
    bool default_empty = default_range.empty();
    (void)default_empty;
    
    if (size > 0) {
        // Try to parse input
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        pugi::xml_parse_result result = doc.load_string(buffer);
        delete[] buffer;
        
        if (result) {
            pugi::xml_node root = doc.root();
            
            // Test children range
            pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();
            bool children_empty = children_range.empty();
            (void)children_empty;
            
            // Test attributes range
            pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range = root.attributes();
            bool attr_empty = attr_range.empty();
            (void)attr_empty;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Test all target functions
    test_xml_text_empty(Data, Size);
    test_xml_node_empty(Data, Size);
    test_xpath_node_set_empty(Data, Size);
    test_xml_attribute_empty(Data, Size);
    test_xml_node_remove_attributes(Data, Size);
    test_xml_object_range_empty(Data, Size);
    
    return 0;
}