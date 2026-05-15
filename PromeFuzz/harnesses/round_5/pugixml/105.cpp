// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

static void test_select_single_node(pugi::xml_node& node, const char* query, pugi::xpath_variable_set* vars) {
    // First overload: const char* query with optional variables
    pugi::xpath_node result1 = node.select_single_node(query, vars);
    (void)result1;
    
    // Second overload: xpath_query object
    pugi::xpath_query xpath_query_obj(query);
    pugi::xpath_node result2 = node.select_single_node(xpath_query_obj);
    (void)result2;
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    const char* desc = result.description();
    (void)desc;
}

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    const char* desc = result.description();
    (void)desc;
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Write data to a dummy file for XML parsing
    const char* filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    
    // Write the fuzzer input to file
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Parse XML from file
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file(filename);
    
    // Test xml_parse_result::description()
    test_xml_parse_result_description();
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
    
    // Get root node for XPath queries
    pugi::xml_node root = doc.root();
    
    // Create XPath query string from input data (limit length to avoid overflow)
    std::string xpath_query;
    if (Size > 0) {
        size_t query_len = Size > 100 ? 100 : Size;
        // Ensure the query string is null-terminated
        xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
        // Replace any null bytes in the middle to avoid early termination
        for (size_t i = 0; i < query_len; ++i) {
            if (xpath_query[i] == '\0') {
                xpath_query[i] = ' ';
            }
        }
    } else {
        xpath_query = "//node";
    }
    
    // Test xpath_parse_result::description()
    test_xpath_parse_result_description();
    
    // Test select_single_node with both overloads
    pugi::xpath_variable_set vars;
    test_select_single_node(root, xpath_query.c_str(), &vars);
    test_select_single_node(root, xpath_query.c_str(), nullptr);
    
    // Additional test: create xpath_query object and test its result
    pugi::xpath_query query_obj(xpath_query.c_str());
    const pugi::xpath_parse_result& query_result = query_obj.result();
    const char* query_desc = query_result.description();
    (void)query_desc;
    
    // Clean up dummy file
    std::remove(filename);
    
    return 0;
}