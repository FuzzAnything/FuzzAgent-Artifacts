// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // Test xml_parse_result::description
    {
        pugi::xml_parse_result parse_result;
        // Initialize with some values
        parse_result.status = static_cast<pugi::xml_parse_status>(Data[0] % 20);
        parse_result.offset = static_cast<ptrdiff_t>(Size);
        const char* desc = parse_result.description();
        (void)desc; // Use result to avoid unused variable warning
    }
    
    // Test xpath_parse_result::description
    {
        pugi::xpath_parse_result xpath_result;
        // Initialize with some values
        xpath_result.offset = static_cast<ptrdiff_t>(Size);
        if (Size > 1) {
            // Create a temporary string for error message
            std::string error_str(reinterpret_cast<const char*>(Data), Size > 100 ? 100 : Size);
            xpath_result.error = error_str.c_str();
        }
        const char* desc = xpath_result.description();
        (void)desc;
    }
    
    // Create XML document and test node operations
    pugi::xml_document doc;
    
    // Try to parse XML from input data
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test xml_parse_result::description again with actual parse result
    {
        const char* desc = parse_result.description();
        (void)desc;
    }
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test select_single_node overloads
        if (Size > 0) {
            // Create XPath query string from input data
            // Ensure null termination for safety
            size_t query_len = Size > 100 ? 100 : Size;
            char* query_str = new char[query_len + 1];
            memcpy(query_str, Data, query_len);
            query_str[query_len] = '\0';
            
            // First overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            try {
                pugi::xpath_node node1 = root.select_single_node(query_str);
                (void)node1;
            } catch (...) {
                // Catch any exceptions
            }
            
            // Second overload: select_single_node(const xpath_query& query)
            try {
                pugi::xpath_query query(query_str);
                pugi::xpath_node node2 = root.select_single_node(query);
                (void)node2;
                
                // Test xpath_parse_result::description from query result
                pugi::xpath_parse_result query_result = query.result();
                const char* desc = query_result.description();
                (void)desc;
            } catch (...) {
                // Catch all exceptions
            }
            
            delete[] query_str;
        }
    }
    
    return 0;
}