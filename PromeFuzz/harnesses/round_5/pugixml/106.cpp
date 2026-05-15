// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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
#include <fstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for XML parsing
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
    
    // Test xml_parse_result::description
    static_cast<void>(parse_result.description());
    
    // Test xpath_parse_result::description
    pugi::xpath_parse_result xpath_parse_result;
    static_cast<void>(xpath_parse_result.description());
    
    // Create XPath query from input data
    std::string query_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test select_single_node overload 1: with char_t* query
    pugi::xml_node node = doc.root();
    if (node) {
        pugi::xpath_node xpath_node = node.select_single_node(query_str.c_str());
        static_cast<void>(xpath_node);
    }
    
    // Test select_single_node overload 2: with xpath_query object
    // First check if we can create an xpath_query
    pugi::xpath_query query(query_str.c_str());
    pugi::xpath_parse_result query_result = query.result();
    static_cast<void>(query_result.description());
    
    if (node && query) {
        pugi::xpath_node xpath_node2 = node.select_single_node(query);
        static_cast<void>(xpath_node2);
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}