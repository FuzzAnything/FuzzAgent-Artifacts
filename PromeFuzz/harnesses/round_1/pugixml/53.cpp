// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void test_xml_node_functions(pugi::xml_node node, const uint8_t* data, size_t size) {
    // Use part of the fuzzer input for string parameters
    size_t str_len = size > 256 ? 256 : size;
    std::string str_data;
    if (str_len > 0) {
        str_data.assign(reinterpret_cast<const char*>(data), str_len);
    } else {
        str_data = "";
    }
    
    // Test next_sibling overloads
    pugi::xml_node next_sib = node.next_sibling();
    pugi::xml_node next_sib_named = node.next_sibling(str_data.c_str());
    
    // Test parent
    pugi::xml_node parent = node.parent();
    
    // Test previous_sibling overloads
    pugi::xml_node prev_sib = node.previous_sibling();
    pugi::xml_node prev_sib_named = node.previous_sibling(str_data.c_str());
    
    // Test first_element_by_path
    pugi::xml_node path_node = node.first_element_by_path(str_data.c_str());
    
    // Also test with different delimiter, but ensure delimiter is not null
    if (str_len > 0 && str_data[0] != 0) {
        char delimiter = str_data[0];
        pugi::xml_node path_node_delim = node.first_element_by_path(str_data.c_str(), delimiter);
    }
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* data, size_t size) {
    // Write data to dummy file for potential file-based parsing
    write_dummy_file(data, size);
    
    // Create XML document and parse from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    if (result) {
        // Test functions on document root
        test_xml_node_functions(doc.root(), data, size);
        
        // Test functions on all nodes in the document
        pugi::xml_node node = doc.first_child();
        while (node) {
            test_xml_node_functions(node, data, size);
            
            // Also test on child nodes
            pugi::xml_node child = node.first_child();
            while (child) {
                test_xml_node_functions(child, data, size);
                child = child.next_sibling();
            }
            
            node = node.next_sibling();
        }
    }
    
    // Test functions on empty nodes
    pugi::xml_node empty_node;
    test_xml_node_functions(empty_node, data, size);
    
    // Test functions on nodes from a newly created document
    pugi::xml_document doc2;
    pugi::xml_node node2 = doc2.append_child("test");
    pugi::xml_node child2 = node2.append_child("child");
    test_xml_node_functions(node2, data, size);
    test_xml_node_functions(child2, data, size);
    
    // Test with sibling relationships
    pugi::xml_node sibling1 = node2.append_child("sibling1");
    pugi::xml_node sibling2 = node2.append_child("sibling2");
    test_xml_node_functions(sibling1, data, size);
    test_xml_node_functions(sibling2, data, size);
    
    // Test parent relationships
    pugi::xml_node grandchild = child2.append_child("grandchild");
    test_xml_node_functions(grandchild, data, size);
    
    // Test with attributes
    node2.append_attribute("attr1") = "value1";
    node2.append_attribute("attr2") = "value2";
    
    return 0;
}