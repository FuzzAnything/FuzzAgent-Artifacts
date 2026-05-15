// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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
#include <string>

static void test_xpath_parse_result_description(const pugi::xpath_parse_result& res) {
    const char* desc = res.description();
    (void)desc;
}

static void test_xpath_query_result(const pugi::xpath_query& query) {
    const pugi::xpath_parse_result& res = query.result();
    test_xpath_parse_result_description(res);
}

static void test_xml_parse_result_description(const pugi::xml_parse_result& res) {
    const char* desc = res.description();
    (void)desc;
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test xml_parse_result::description
    test_xml_parse_result_description(parse_result);
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query from part of the input
        size_t xpath_size = Size > 1024 ? 1024 : Size;
        std::string xpath_str(reinterpret_cast<const char*>(Data), xpath_size);
        
        // Test deprecated select_single_node
        pugi::xpath_node result = root.select_single_node(xpath_str.c_str());
        (void)result;
        
        // Test with empty variable set
        pugi::xpath_variable_set vars;
        result = root.select_single_node(xpath_str.c_str(), &vars);
        (void)result;
        
        // Test xpath_query
        pugi::xpath_query query(xpath_str.c_str());
        test_xpath_query_result(query);
        
        // Try to evaluate the query
        pugi::xpath_node_set nodes = query.evaluate_node_set(root);
        (void)nodes;
        
        // Test with xpath_variable_set
        pugi::xpath_query query_with_vars(xpath_str.c_str(), &vars);
        test_xpath_query_result(query_with_vars);
        
        // Create xpath_node to test constructor
        if (root.first_child()) {
            pugi::xml_node child = root.first_child();
            pugi::xpath_node xnode(child);
            (void)xnode;
            
            // Test with attribute
            if (child.first_attribute()) {
                pugi::xml_attribute attr = child.first_attribute();
                pugi::xpath_node xnode_attr(attr, child);
                (void)xnode_attr;
            }
        }
        
        // Test xpath_variable_set move constructor
        pugi::xpath_variable_set vars2(std::move(vars));
        (void)vars2;
    }
    
    // Test xpath_parse_result description with error
    pugi::xpath_parse_result xpath_parse_res;
    xpath_parse_res.error = "Test error";
    xpath_parse_res.offset = 0;
    test_xpath_parse_result_description(xpath_parse_res);
    
    return 0;
}