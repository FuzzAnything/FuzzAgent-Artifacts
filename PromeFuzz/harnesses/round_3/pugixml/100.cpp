// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    file.write(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // Test pugi::xml_parse_result::description()
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size);
        const char* desc = result.description();
        (void)desc;
    }
    
    // Test pugi::xpath_parse_result::description()
    {
        pugi::xpath_parse_result parse_result;
        parse_result.error = "Test error";
        parse_result.offset = 0;
        const char* xpath_desc = parse_result.description();
        (void)xpath_desc;
    }
    
    // Test pugi::xml_node::select_single_node overloads
    {
        pugi::xml_document doc;
        if (doc.load_buffer(Data, Size)) {
            pugi::xml_node root = doc.root();
            
            // Create XPath query from data (limited to safe size)
            char query[256];
            size_t query_size = Size < sizeof(query) - 1 ? Size : sizeof(query) - 1;
            if (query_size > 0) {
                memcpy(query, Data, query_size);
                query[query_size] = '\0';
                
                // First overload: const char_t* with variables
                pugi::xpath_variable_set vars;
                pugi::xpath_node node1 = root.select_single_node(query, &vars);
                (void)node1;
                
                // Second overload: xpath_query object
                pugi::xpath_query xpath_query_obj(query);
                pugi::xpath_node node2 = root.select_single_node(xpath_query_obj);
                (void)node2;
            }
        }
    }
    
    // Additional test: try parsing the dummy file
    {
        pugi::xml_document doc;
        pugi::xml_parse_result file_result = doc.load_file("./dummy_file");
        const char* file_desc = file_result.description();
        (void)file_desc;
    }
    
    return 0;
}