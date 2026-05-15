// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Create empty xpath_node_set for testing basic functions
    pugi::xpath_node_set empty_set;
    
    // Call functions in specified order on empty set
    empty_set.size();
    empty_set.type();
    empty_set.empty();
    empty_set.first();
    empty_set.end();
    
    // Try to parse input as XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (result) {
        // Create XPath query from input data
        std::string xpath_query;
        if (Size > 0) {
            // Use first part of input as XPath query, but limit length
            size_t query_len = Size > 100 ? 100 : Size;
            xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
            
            // Ensure query is null-terminated
            if (!xpath_query.empty() && xpath_query.back() != '\0') {
                xpath_query.push_back('\0');
            }
        } else {
            xpath_query = "//*";
        }
        
        // Call select_nodes on document element
        pugi::xml_node node = doc.document_element();
        if (node) {
            pugi::xpath_node_set selected_set = node.select_nodes(xpath_query.c_str());
            
            // Also call the functions on the non-empty set
            selected_set.size();
            selected_set.type();
            selected_set.empty();
            selected_set.first();
            selected_set.end();
        }
        
        // Also try select_nodes on the document itself
        doc.select_nodes(xpath_query.c_str());
    }
    
    return 0;
}