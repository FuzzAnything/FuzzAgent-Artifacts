// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Only proceed if parsing succeeded
    if (result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // Create an xpath_node_set by evaluating a simple XPath query
        // Use a fixed query to ensure we get a node set to work with
        const char* query = "//*";
        pugi::xpath_node_set node_set = root.select_nodes(query);
        
        // Now call the functions in the specified order
        // 1. size()
        size_t set_size = node_set.size();
        
        // 2. type()
        pugi::xpath_node_set::type_t set_type = node_set.type();
        
        // 3. empty()
        bool is_empty = node_set.empty();
        
        // 4. first()
        pugi::xpath_node first_node = node_set.first();
        
        // 5. end()
        pugi::xpath_node_set::const_iterator end_iter = node_set.end();
        
        // 6. select_nodes() - call again with different query
        // Use part of the fuzzer input as query string if possible
        std::string xpath_query;
        if (Size > 0) {
            // Use first 100 bytes or less for query
            size_t query_len = Size > 100 ? 100 : Size;
            xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
            
            // Ensure the query is null-terminated
            if (xpath_query.find('\0') == std::string::npos) {
                xpath_query.push_back('\0');
            }
            
            // Try to select nodes with the generated query
            try {
                pugi::xpath_node_set another_set = root.select_nodes(xpath_query.c_str());
                
                // Call the same functions on the new set
                another_set.size();
                another_set.type();
                another_set.empty();
                another_set.first();
                another_set.end();
            } catch (...) {
                // Ignore exceptions from invalid XPath queries
            }
        }
        
        // Also test with empty xpath_node_set (default constructed)
        pugi::xpath_node_set empty_set;
        empty_set.size();
        empty_set.type();
        empty_set.empty();
        empty_set.first();
        empty_set.end();
    }
    
    return 0;
}