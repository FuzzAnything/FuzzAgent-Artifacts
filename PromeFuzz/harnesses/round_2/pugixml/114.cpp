// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13181:40 in pugixml.hpp
// pugi::xpath_variable_set::set at pugixml.cpp:13187:40 in pugixml.hpp
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
#include <vector>

static void test_xpath_parse_result(const pugi::xpath_parse_result& parse_result) {
    // Test xpath_parse_result::description
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_xpath_queries(pugi::xml_node& node, const char* query, 
                               pugi::xpath_variable_set* variables = nullptr) {
    // Test deprecated select_single_node with char* overload
    pugi::xpath_node single_node1 = node.select_single_node(query, variables);
    (void)single_node1;
    
    // Test select_node
    pugi::xpath_node selected_node = node.select_node(query, variables);
    (void)selected_node;
    
    // Test deprecated select_single_node with xpath_query overload
    pugi::xpath_query xpath_query_obj(query, variables);
    pugi::xpath_node single_node2 = node.select_single_node(xpath_query_obj);
    (void)single_node2;
}

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document from fuzzing input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Generate XPath query from fuzzing input
    std::string xpath_query;
    if (Size > 0) {
        // Use first part of input for query (max 256 chars to avoid overly long queries)
        size_t query_len = Size > 256 ? 256 : Size;
        xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
        
        // Ensure null termination
        if (!xpath_query.empty() && xpath_query.back() != '\0') {
            xpath_query.push_back('\0');
        }
    }
    
    if (!xpath_query.empty()) {
        // Test with null variables
        test_xpath_queries(root, xpath_query.c_str());
        
        // Test with variable set
        pugi::xpath_variable_set variables;
        if (Size > 10) {
            // Add some variables from fuzzing data
            double num_val = static_cast<double>(Data[0]);
            variables.set("var1", num_val);
            
            if (Size > 20) {
                const char* str_val = reinterpret_cast<const char*>(Data + 1);
                size_t str_len = Size > 50 ? 49 : Size - 1;
                std::string safe_str(str_val, str_len);
                variables.set("var2", safe_str.c_str());
            }
        }
        test_xpath_queries(root, xpath_query.c_str(), &variables);
    }
    
    // Test xpath_parse_result::description with various parse results
    pugi::xpath_parse_result parse_result1;
    parse_result1.error = "Test error";
    parse_result1.offset = 10;
    test_xpath_parse_result(parse_result1);
    
    pugi::xpath_parse_result parse_result2;
    parse_result2.error = nullptr;
    parse_result2.offset = 0;
    test_xpath_parse_result(parse_result2);
    
    pugi::xpath_parse_result parse_result3;
    parse_result3.error = "";
    parse_result3.offset = -1;
    test_xpath_parse_result(parse_result3);
    
    // Test with the actual parse result from XML loading
    pugi::xpath_parse_result xpath_parse_result;
    xpath_parse_result.error = parse_result.description();
    xpath_parse_result.offset = parse_result.offset;
    test_xpath_parse_result(xpath_parse_result);
    
    return 0;
}