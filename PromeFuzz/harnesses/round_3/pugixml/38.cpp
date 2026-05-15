// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Only proceed if parsing was successful
    if (parse_result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // Split input into parts for different XPath queries
        size_t quarter = Size / 4;
        const char* query1 = reinterpret_cast<const char*>(Data);
        size_t query1_len = quarter;
        const char* query2 = reinterpret_cast<const char*>(Data + quarter);
        size_t query2_len = quarter;
        const char* query3 = reinterpret_cast<const char*>(Data + 2 * quarter);
        size_t query3_len = quarter;
        const char* query4 = reinterpret_cast<const char*>(Data + 3 * quarter);
        size_t query4_len = Size - 3 * quarter;
        
        // Create null-terminated strings for XPath queries
        static char query1_buf[4096];
        static char query2_buf[4096];
        static char query3_buf[4096];
        static char query4_buf[4096];
        
        size_t copy_len1 = query1_len < 4095 ? query1_len : 4095;
        std::memcpy(query1_buf, query1, copy_len1);
        query1_buf[copy_len1] = '\0';
        
        size_t copy_len2 = query2_len < 4095 ? query2_len : 4095;
        std::memcpy(query2_buf, query2, copy_len2);
        query2_buf[copy_len2] = '\0';
        
        size_t copy_len3 = query3_len < 4095 ? query3_len : 4095;
        std::memcpy(query3_buf, query3, copy_len3);
        query3_buf[copy_len3] = '\0';
        
        size_t copy_len4 = query4_len < 4095 ? query4_len : 4095;
        std::memcpy(query4_buf, query4, copy_len4);
        query4_buf[copy_len4] = '\0';
        
        // First select_single_node call with both overloads
        try {
            pugi::xpath_query xpath_query1(query1_buf);
            pugi::xpath_node result1 = root.select_single_node(xpath_query1);
        } catch (...) {
            // Ignore exceptions from invalid XPath queries
        }
        
        pugi::xpath_node result2 = root.select_single_node(query2_buf);
        
        // Second select_single_node call with both overloads
        try {
            pugi::xpath_query xpath_query3(query3_buf);
            pugi::xpath_node result3 = root.select_single_node(xpath_query3);
        } catch (...) {
            // Ignore exceptions from invalid XPath queries
        }
        
        pugi::xpath_node result4 = root.select_single_node(query4_buf);
        
        // Call node() on the xpath_node results
        pugi::xml_node node1 = result2.node();
        pugi::xml_node node2 = result4.node();
        
        // Exercise the node() function by checking if nodes are valid
        (void)node1.empty();
        (void)node2.empty();
    }
    
    return 0;
}