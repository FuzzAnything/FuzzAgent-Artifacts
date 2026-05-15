// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for potential XML loading
    writeDummyFile(Data, Size);
    
    // Test xml_parse_result::description()
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string("");
        const char* desc = result.description();
        (void)desc; // Use result to avoid unused variable warning
    }
    
    // Try to parse XML from the input data
    std::string xml_string(reinterpret_cast<const char*>(Data), Size);
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_string.c_str());
    
    // Get description from parse result
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
    
    // If parsing succeeded, test select_single_node
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        // Create xpath_variable_set for first overload
        pugi::xpath_variable_set vars;
        
        // Test first overload of select_single_node with raw query string
        try {
            // Use a simple XPath query that should work with any XML
            pugi::xpath_node node1 = root.select_single_node("//*");
            (void)node1;
            
            // Also test with variables (NULL)
            pugi::xpath_node node2 = root.select_single_node("//*", nullptr);
            (void)node2;
            
            // Test with variable set
            pugi::xpath_node node3 = root.select_single_node("//*", &vars);
            (void)node3;
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test second overload of select_single_node with xpath_query
        try {
            // Create xpath_query from input data (use first 100 bytes or less)
            size_t query_size = Size > 100 ? 100 : Size;
            std::string query_str(reinterpret_cast<const char*>(Data), query_size);
            pugi::xpath_query query(query_str.c_str());
            
            // Get xpath_parse_result from query and test description()
            pugi::xpath_parse_result xpath_result = query.result();
            const char* xpath_desc = xpath_result.description();
            (void)xpath_desc;
            
            // Test select_single_node with xpath_query
            if (query) {
                pugi::xpath_node node4 = root.select_single_node(query);
                (void)node4;
            }
        } catch (...) {
            // Ignore all exceptions
        }
    }
    
    // Test xpath_parse_result::description() with default constructor
    {
        pugi::xpath_parse_result default_result;
        const char* default_desc = default_result.description();
        (void)default_desc;
    }
    
    // Also test with file loading
    {
        pugi::xml_document doc_file;
        pugi::xml_parse_result file_result = doc_file.load_file("./dummy_file");
        const char* file_desc = file_result.description();
        (void)file_desc;
        
        if (file_result) {
            pugi::xml_node root = doc_file.root();
            try {
                pugi::xpath_node node = root.select_single_node("//*");
                (void)node;
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    return 0;
}