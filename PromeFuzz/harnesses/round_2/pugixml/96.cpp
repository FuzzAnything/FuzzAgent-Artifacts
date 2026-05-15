// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <vector>

#include "pugixml.hpp"

static void test_xpath_node_set_functions(const pugi::xpath_node_set& set) {
    // Test size() function
    size_t set_size = set.size();
    
    // Test empty() function
    bool is_empty = set.empty();
    
    // Test type() function
    pugi::xpath_node_set::type_t set_type = set.type();
    
    // Test first() function
    pugi::xpath_node first_node = set.first();
    
    // Test begin() and end() functions for iteration
    pugi::xpath_node_set::const_iterator it_begin = set.begin();
    pugi::xpath_node_set::const_iterator it_end = set.end();
    
    // Iterate through the set if not empty
    if (!is_empty) {
        for (pugi::xpath_node_set::const_iterator it = it_begin; it != it_end; ++it) {
            // Access each node
            pugi::xpath_node node = *it;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing succeeded, create XPath node sets
    if (result) {
        // Create an empty xpath_node_set using default constructor
        pugi::xpath_node_set empty_set;
        test_xpath_node_set_functions(empty_set);
        
        // Create xpath_node_set from document nodes using XPath query
        // Use try-catch for any exceptions during XPath evaluation
        try {
            // Try to evaluate an XPath query on the document
            pugi::xpath_node_set set = doc.select_nodes("//*");
            test_xpath_node_set_functions(set);
            
            // Test with another XPath query
            pugi::xpath_node_set attr_set = doc.select_nodes("//@*");
            test_xpath_node_set_functions(attr_set);
            
            // Test with specific path
            pugi::xpath_node_set root_set = doc.select_nodes("/");
            test_xpath_node_set_functions(root_set);
            
            // Test move constructor
            pugi::xpath_node_set moved_set(std::move(set));
            test_xpath_node_set_functions(moved_set);
            
        } catch (...) {
            // Catch any exceptions during XPath evaluation
        }
        
        // Create individual xpath_nodes from xml_nodes
        pugi::xml_node root = doc.root();
        if (root) {
            pugi::xpath_node root_xpath_node(root);
            
            // Create xpath_node_set from single node using iterator constructor
            pugi::xpath_node nodes[] = {root_xpath_node};
            pugi::xpath_node_set single_node_set(nodes, nodes + 1, pugi::xpath_node_set::type_unsorted);
            test_xpath_node_set_functions(single_node_set);
            
            // Try to get attributes
            for (pugi::xml_attribute attr : root.attributes()) {
                // Create xpath_node from attribute with its parent
                pugi::xpath_node attr_xpath_node(attr, root);
                pugi::xpath_node attr_nodes[] = {attr_xpath_node};
                pugi::xpath_node_set attr_node_set(attr_nodes, attr_nodes + 1, pugi::xpath_node_set::type_unsorted);
                test_xpath_node_set_functions(attr_node_set);
            }
        }
    }
    
    // Also test with file input
    static const char* filename = "./dummy_file";
    std::ofstream out_file(filename, std::ios::binary);
    if (out_file) {
        out_file.write(reinterpret_cast<const char*>(Data), Size);
        out_file.close();
        
        pugi::xml_document file_doc;
        pugi::xml_parse_result file_result = file_doc.load_file(filename);
        
        if (file_result) {
            try {
                pugi::xpath_node_set file_set = file_doc.select_nodes("//node()");
                test_xpath_node_set_functions(file_set);
            } catch (...) {
                // Catch any exceptions during XPath evaluation
            }
        }
        
        // Clean up the file
        std::remove(filename);
    }
    
    return 0;
}