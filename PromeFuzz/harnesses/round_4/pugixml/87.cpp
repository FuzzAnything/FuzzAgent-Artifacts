// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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

static void test_xpath_parse_result_description(const uint8_t* data, size_t size) {
    pugi::xpath_parse_result result;
    
    // Initialize with fuzzed data if we have enough
    if (size > 0) {
        // Use first byte as offset (mod size to avoid overflow)
        result.offset = static_cast<ptrdiff_t>(data[0] % (size + 1));
        
        // Create error string from remaining data if any
        if (size > 1) {
            // Use a static buffer to avoid allocation issues
            static char error_buffer[256];
            size_t error_len = (size - 1) < 255 ? (size - 1) : 255;
            memcpy(error_buffer, data + 1, error_len);
            error_buffer[error_len] = '\0';
            result.error = error_buffer;
        } else {
            result.error = nullptr;
        }
    }
    
    // Call target function
    const char* desc = result.description();
    (void)desc; // Use result to avoid unused variable warning
}

static void test_xml_parse_result_description(const uint8_t* data, size_t size) {
    pugi::xml_parse_result result;
    
    // Initialize with fuzzed data
    if (size > 0) {
        // Use first byte as status
        result.status = static_cast<pugi::xml_parse_status>(data[0] % 20); // There are 19 status codes
        
        // Use second byte as offset if available
        if (size > 1) {
            result.offset = static_cast<ptrdiff_t>(data[1]);
        }
        
        // Use third byte as encoding if available
        if (size > 2) {
            result.encoding = static_cast<pugi::xml_encoding>(data[2] % 10); // There are 9 encodings
        }
    }
    
    // Call target function
    const char* desc = result.description();
    (void)desc;
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    // Create a dummy XML document
    pugi::xml_document doc;
    
    // Try to parse fuzzed data as XML
    pugi::xml_parse_result parse_result = doc.load_buffer(data, size);
    
    // If parsing succeeded, test select_single_node
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        // Create XPath query from fuzzed data (use first 128 bytes or less)
        static char query_buffer[129];
        size_t query_len = size < 128 ? size : 128;
        if (query_len > 0) {
            memcpy(query_buffer, data, query_len);
            query_buffer[query_len] = '\0';
            
            // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            try {
                pugi::xpath_variable_set variables;
                pugi::xpath_node node1 = root.select_single_node(query_buffer, &variables);
                (void)node1;
            } catch (...) {
                // Ignore exceptions
            }
            
            // Test first overload with null variables
            try {
                pugi::xpath_node node2 = root.select_single_node(query_buffer, nullptr);
                (void)node2;
            } catch (...) {
                // Ignore exceptions
            }
            
            // Test second overload: select_single_node(const xpath_query& query)
            try {
                pugi::xpath_query query(query_buffer);
                pugi::xpath_node node3 = root.select_single_node(query);
                (void)node3;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Also test with empty document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_node = empty_doc.root();
    
    static const char* test_queries[] = {"//node", "/root", "child::*", ""};
    
    for (const char* query : test_queries) {
        // Test first overload
        try {
            pugi::xpath_node node1 = empty_node.select_single_node(query, nullptr);
            (void)node1;
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test second overload
        try {
            pugi::xpath_query xquery(query);
            pugi::xpath_node node2 = empty_node.select_single_node(xquery);
            (void)node2;
        } catch (...) {
            // Ignore exceptions
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* data, size_t size) {
    // Test xpath_parse_result::description
    test_xpath_parse_result_description(data, size);
    
    // Test xml_parse_result::description
    test_xml_parse_result_description(data, size);
    
    // Test xml_node::select_single_node (both overloads)
    test_select_single_node(data, size);
    
    return 0;
}