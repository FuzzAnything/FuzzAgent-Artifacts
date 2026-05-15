// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Test 1: pugi::xml_attribute::empty
    {
        pugi::xml_attribute attr1;
        bool empty1 = attr1.empty();
        
        // Create a document and try to get an attribute
        pugi::xml_document doc1;
        if (Size > 0) {
            const char* xml_data = reinterpret_cast<const char*>(Data);
            pugi::xml_parse_result result = doc1.load_buffer(xml_data, Size);
            if (result) {
                pugi::xml_node root = doc1.root();
                if (!root.empty()) {
                    pugi::xml_attribute attr2 = root.first_attribute();
                    bool empty2 = attr2.empty();
                }
            }
        }
    }
    
    // Test 2: pugi::xpath_node_set::empty
    {
        pugi::xpath_node_set set1;
        bool empty1 = set1.empty();
        
        // Try to create a set from iterator range
        if (Size > 0) {
            std::vector<pugi::xpath_node> nodes;
            nodes.reserve(2);
            
            // Create some dummy xpath_node objects
            pugi::xml_document doc2;
            const char* xml_data = reinterpret_cast<const char*>(Data);
            pugi::xml_parse_result result = doc2.load_buffer(xml_data, Size);
            
            if (result) {
                pugi::xml_node root = doc2.root();
                if (!root.empty()) {
                    pugi::xpath_node node1(root);
                    nodes.push_back(node1);
                    
                    // Try to add a child node if exists
                    pugi::xml_node child = root.first_child();
                    if (!child.empty()) {
                        pugi::xpath_node node2(child);
                        nodes.push_back(node2);
                    }
                }
            }
            
            // Create xpath_node_set with available nodes
            if (!nodes.empty()) {
                pugi::xpath_node_set set2(nodes.data(), nodes.data() + nodes.size());
                bool empty2 = set2.empty();
                
                // Test with different types
                pugi::xpath_node_set set3(nodes.data(), nodes.data() + nodes.size(), 
                                         pugi::xpath_node_set::type_sorted);
                bool empty3 = set3.empty();
                
                pugi::xpath_node_set set4(nodes.data(), nodes.data() + nodes.size(),
                                         pugi::xpath_node_set::type_sorted_reverse);
                bool empty4 = set4.empty();
            }
        }
    }
    
    // Test 3: pugi::xml_node::remove_attributes
    {
        pugi::xml_node node1;
        bool removed1 = node1.remove_attributes();
        
        if (Size > 0) {
            pugi::xml_document doc3;
            const char* xml_data = reinterpret_cast<const char*>(Data);
            pugi::xml_parse_result result = doc3.load_buffer(xml_data, Size);
            
            if (result) {
                pugi::xml_node root = doc3.root();
                if (!root.empty()) {
                    bool removed2 = root.remove_attributes();
                    
                    // Try on child nodes
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        bool removed3 = child.remove_attributes();
                    }
                }
            }
        }
    }
    
    // Test 4: pugi::xml_object_range::empty
    {
        // Test with xml_node_iterator
        pugi::xml_node node2;
        pugi::xml_object_range<pugi::xml_node_iterator> range1 = node2.children();
        bool empty1 = range1.empty();
        
        // Test with xml_attribute_iterator
        pugi::xml_object_range<pugi::xml_attribute_iterator> range2 = node2.attributes();
        bool empty2 = range2.empty();
        
        if (Size > 0) {
            pugi::xml_document doc4;
            const char* xml_data = reinterpret_cast<const char*>(Data);
            pugi::xml_parse_result result = doc4.load_buffer(xml_data, Size);
            
            if (result) {
                pugi::xml_node root = doc4.root();
                if (!root.empty()) {
                    pugi::xml_object_range<pugi::xml_node_iterator> range3 = root.children();
                    bool empty3 = range3.empty();
                    
                    pugi::xml_object_range<pugi::xml_attribute_iterator> range4 = root.attributes();
                    bool empty4 = range4.empty();
                }
            }
        }
    }
    
    // Test 5: pugi::xml_text::empty
    {
        pugi::xml_text text1;
        bool empty1 = text1.empty();
        
        if (Size > 0) {
            pugi::xml_document doc5;
            const char* xml_data = reinterpret_cast<const char*>(Data);
            pugi::xml_parse_result result = doc5.load_buffer(xml_data, Size);
            
            if (result) {
                pugi::xml_node root = doc5.root();
                if (!root.empty()) {
                    pugi::xml_text text2 = root.text();
                    bool empty2 = text2.empty();
                    
                    // Try on child nodes
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        pugi::xml_text text3 = child.text();
                        bool empty3 = text3.empty();
                    }
                }
            }
        }
    }
    
    // Test 6: pugi::xml_node::empty
    {
        pugi::xml_node node3;
        bool empty1 = node3.empty();
        
        if (Size > 0) {
            pugi::xml_document doc6;
            const char* xml_data = reinterpret_cast<const char*>(Data);
            pugi::xml_parse_result result = doc6.load_buffer(xml_data, Size);
            
            if (result) {
                pugi::xml_node root = doc6.root();
                bool empty2 = root.empty();
                
                if (!root.empty()) {
                    // Test on child nodes
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        bool empty3 = child.empty();
                    }
                }
            }
        }
    }
    
    return 0;
}