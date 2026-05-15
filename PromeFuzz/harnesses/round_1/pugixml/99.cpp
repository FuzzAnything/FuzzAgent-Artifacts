// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    // Test xpath_parse_result::description
    pugi::xpath_parse_result xpath_parse_res;
    xpath_parse_res.error = "Test error";
    xpath_parse_res.offset = 0;
    const char* desc1 = xpath_parse_res.description();
    (void)desc1; // Use to avoid unused variable warning
    
    // Test xml_parse_result::description
    pugi::xml_parse_result xml_parse_res;
    xml_parse_res.status = pugi::status_internal_error;
    const char* desc2 = xml_parse_res.description();
    (void)desc2; // Use to avoid unused variable warning
    
    // Test xml_node::select_single_node overloads
    if (Size > 0) {
        // Create XML document from fuzzing input
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
        
        // Only proceed if parsing was somewhat successful or we have a document
        if (parse_result || doc) {
            // Get the root node or create an empty node
            pugi::xml_node node = doc.document_element();
            if (!node) {
                node = doc.append_child("root");
            }
            
            // Create XPath query from part of the input
            std::string xpath_query;
            if (Size > 10) {
                // Use first 10 bytes as XPath query (or less if Size is smaller)
                size_t query_size = Size > 10 ? 10 : Size;
                xpath_query.assign(reinterpret_cast<const char*>(Data), query_size);
            } else {
                xpath_query = "//*";
            }
            
            // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            try {
                pugi::xpath_node result1 = node.select_single_node(xpath_query.c_str());
                (void)result1; // Use to avoid unused variable warning
            } catch (...) {
                // Ignore exceptions from invalid XPath queries
            }
            
            // Test second overload: select_single_node(const xpath_query& query)
            try {
                pugi::xpath_query query(xpath_query.c_str());
                pugi::xpath_node result2 = node.select_single_node(query);
                (void)result2; // Use to avoid unused variable warning
            } catch (...) {
                // Ignore exceptions from invalid XPath queries
            }
        }
    }
    
    return 0;
}