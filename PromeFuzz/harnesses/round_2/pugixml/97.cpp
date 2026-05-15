// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static void test_xpath_node_set_functions(const pugi::xpath_node_set& set) {
    // Test size() function
    size_t set_size = set.size();
    (void)set_size;
    
    // Test first() function
    pugi::xpath_node first_node = set.first();
    (void)first_node;
    
    // Test empty() function
    bool is_empty = set.empty();
    (void)is_empty;
    
    // Test end() function
    pugi::xpath_node_set::const_iterator end_iter = set.end();
    (void)end_iter;
    
    // Test begin() function
    pugi::xpath_node_set::const_iterator begin_iter = set.begin();
    (void)begin_iter;
    
    // Test type() function
    pugi::xpath_node_set::type_t set_type = set.type();
    (void)set_type;
    
    // Test iteration using begin() and end()
    for (pugi::xpath_node_set::const_iterator it = set.begin(); it != set.end(); ++it) {
        pugi::xpath_node node = *it;
        (void)node;
    }
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Parse XML document from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (result) {
        // Create xpath_node_set using XPath query
        pugi::xpath_node_set set1 = doc.select_nodes("//*");
        pugi::xpath_node_set set2 = doc.select_nodes("//node()");
        pugi::xpath_node_set set3 = doc.select_nodes("/");
        
        // Test functions on different sets
        test_xpath_node_set_functions(set1);
        test_xpath_node_set_functions(set2);
        test_xpath_node_set_functions(set3);
        
        // Test empty set
        pugi::xpath_node_set empty_set;
        test_xpath_node_set_functions(empty_set);
        
        // Test copy constructor
        pugi::xpath_node_set copied_set(set1);
        test_xpath_node_set_functions(copied_set);
        
        // Test with single node using range constructor
        pugi::xml_node root = doc.root();
        pugi::xpath_node single_xpath_node(root);
        const pugi::xpath_node* nodes = &single_xpath_node;
        pugi::xpath_node_set single_node_set(nodes, nodes + 1);
        test_xpath_node_set_functions(single_node_set);
        
        // Test with attribute nodes
        pugi::xpath_node_set attr_set = doc.select_nodes("//@*");
        test_xpath_node_set_functions(attr_set);
    } else {
        // Even if parsing fails, test with empty sets
        pugi::xpath_node_set empty_set;
        test_xpath_node_set_functions(empty_set);
        
        // Test default constructed xpath_node_set
        pugi::xpath_node_set default_set;
        test_xpath_node_set_functions(default_set);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}