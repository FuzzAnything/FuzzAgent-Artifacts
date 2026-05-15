// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include <algorithm>

#include "pugixml.hpp"

static void test_xml_attribute_empty() {
    // Test with default-constructed attribute (should be empty)
    pugi::xml_attribute attr1;
    bool empty1 = attr1.empty();
    
    // Test with attribute from a document
    pugi::xml_document doc;
    doc.append_child("root").append_attribute("attr") = "value";
    pugi::xml_attribute attr2 = doc.child("root").attribute("attr");
    bool empty2 = attr2.empty();
    
    // Test with non-existent attribute
    pugi::xml_attribute attr3 = doc.child("root").attribute("nonexistent");
    bool empty3 = attr3.empty();
}

static void test_xpath_node_set_functions(const uint8_t* data, size_t size) {
    // Create a document and populate with some nodes
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Use fuzzer data to create variable number of nodes
    size_t num_nodes = size > 0 ? (data[0] % 10) + 1 : 1;
    
    std::vector<pugi::xpath_node> nodes;
    for (size_t i = 0; i < num_nodes && i < size; i++) {
        char node_name[32];
        snprintf(node_name, sizeof(node_name), "node%zu", i);
        pugi::xml_node child = root.append_child(node_name);
        
        // Add some attributes based on fuzzer data
        if (i + 1 < size) {
            char attr_name[32];
            snprintf(attr_name, sizeof(attr_name), "attr%zu", i);
            child.append_attribute(attr_name) = "value";
            
            // Create xpath_node from attribute
            pugi::xml_attribute attr = child.attribute(attr_name);
            nodes.push_back(pugi::xpath_node(attr, child));
        }
        
        // Also add node itself
        nodes.push_back(pugi::xpath_node(child));
    }
    
    // Create xpath_node_set with different types
    if (!nodes.empty()) {
        // Test unsorted set
        pugi::xpath_node_set set1(nodes.data(), nodes.data() + nodes.size(), 
                                  pugi::xpath_node_set::type_unsorted);
        
        // Test empty() and begin()
        bool empty1 = set1.empty();
        pugi::xpath_node_set::const_iterator begin1 = set1.begin();
        
        // Test sorting
        set1.sort(false);  // ascending
        set1.sort(true);   // descending
        
        // Test with empty set
        pugi::xpath_node_set set2(nullptr, nullptr);
        bool empty2 = set2.empty();
        pugi::xpath_node_set::const_iterator begin2 = set2.begin();
        
        // Test sorted set
        pugi::xpath_node_set set3(nodes.data(), nodes.data() + nodes.size(),
                                  pugi::xpath_node_set::type_sorted);
        bool empty3 = set3.empty();
        set3.sort(true);  // should re-sort to reverse
        
        // Test sorted_reverse set
        pugi::xpath_node_set set4(nodes.data(), nodes.data() + nodes.size(),
                                  pugi::xpath_node_set::type_sorted_reverse);
        bool empty4 = set4.empty();
        set4.sort(false);  // should re-sort to ascending
    }
}

static void test_xml_object_range_empty() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Test with non-empty range
    pugi::xml_object_range<pugi::xml_node_iterator> range1 = root.children();
    bool empty1 = range1.empty();
    
    // Test with empty range
    pugi::xml_node empty_node;
    pugi::xml_object_range<pugi::xml_node_iterator> range2 = empty_node.children();
    bool empty2 = range2.empty();
    
    // Test with attributes range
    root.append_attribute("attr1") = "value1";
    root.append_attribute("attr2") = "value2";
    pugi::xml_object_range<pugi::xml_attribute_iterator> range3 = root.attributes();
    bool empty3 = range3.empty();
    
    // Test with empty attributes range
    pugi::xml_node child = root.append_child("child");
    pugi::xml_object_range<pugi::xml_attribute_iterator> range4 = child.attributes();
    bool empty4 = range4.empty();
}

static void test_xml_text_empty() {
    // Test with default-constructed text
    pugi::xml_text text1;
    bool empty1 = text1.empty();
    
    // Test with node that has text
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    root.append_child(pugi::node_pcdata).set_value("some text");
    pugi::xml_text text2 = root.text();
    bool empty2 = text2.empty();
    
    // Test with node that has no text
    pugi::xml_node child = root.append_child("child");
    pugi::xml_text text3 = child.text();
    bool empty3 = text3.empty();
    
    // Test with node that has CDATA text
    pugi::xml_node cdata_node = root.append_child("cdata_node");
    cdata_node.append_child(pugi::node_cdata).set_value("CDATA content");
    pugi::xml_text text4 = cdata_node.text();
    bool empty4 = text4.empty();
    
    // Test with text from non-existent node
    pugi::xml_node non_existent = doc.child("non_existent");
    pugi::xml_text text5 = non_existent.text();
    bool empty5 = text5.empty();
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    // Test xml_attribute::empty
    test_xml_attribute_empty();
    
    // Test xpath_node_set functions with fuzzer data
    test_xpath_node_set_functions(Data, Size);
    
    // Test xml_object_range::empty
    test_xml_object_range_empty();
    
    // Test xml_text::empty
    test_xml_text_empty();
    
    return 0;
}