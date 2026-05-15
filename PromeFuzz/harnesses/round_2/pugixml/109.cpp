// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
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
#include <string>

#include "pugixml.hpp"

static void test_xpath_parse_result() {
    pugi::xpath_parse_result result;
    // Test xpath_parse_result::description()
    const char* desc = result.description();
    (void)desc;
}

static void test_xml_node_functions(const pugi::xml_node& node, const char* query) {
    if (!node.empty() && query && *query != '\0') {
        // Test select_single_node with char* query
        pugi::xpath_node result1 = node.select_single_node(query);
        (void)result1;
        
        // Test select_node with char* query
        pugi::xpath_node result2 = node.select_node(query);
        (void)result2;
        
        // Test select_single_node with xpath_query object
        try {
            pugi::xpath_query xpath_query_obj(query);
            pugi::xpath_node result3 = node.select_single_node(xpath_query_obj);
            (void)result3;
        } catch (const std::exception& e) {
            // Test exception functions
            const char* what_msg = e.what();
            (void)what_msg;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    // Create XML document from input data
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test xpath_parse_result functions
    test_xpath_parse_result();
    
    // Convert input to string for XPath queries
    std::string xpath_query;
    if (Size > 0) {
        xpath_query.assign(reinterpret_cast<const char*>(Data), 
                          std::min(Size, static_cast<size_t>(100)));
    } else {
        xpath_query = "//node";
    }
    
    // Test XML node functions on document root
    pugi::xml_node root = doc.root();
    test_xml_node_functions(root, xpath_query.c_str());
    
    // Also test on child nodes
    for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
        test_xml_node_functions(child, xpath_query.c_str());
        
        // Test on grandchildren too
        for (pugi::xml_node grandchild = child.first_child(); grandchild; 
             grandchild = grandchild.next_sibling()) {
            test_xml_node_functions(grandchild, xpath_query.c_str());
        }
    }
    
    // Test with xpath_variable_set
    pugi::xpath_variable_set variables;
    if (!root.empty() && !xpath_query.empty()) {
        pugi::xpath_node result = root.select_single_node(xpath_query.c_str(), &variables);
        (void)result;
        
        pugi::xpath_node result2 = root.select_node(xpath_query.c_str(), &variables);
        (void)result2;
    }
    
    return 0;
}