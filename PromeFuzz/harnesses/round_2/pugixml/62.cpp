// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test xml_parse_result::description
    {
        pugi::xml_parse_result result;
        // Initialize with some state
        result.status = static_cast<pugi::xml_parse_status>(Data[0] % 20);
        const char* desc = result.description();
        (void)desc; // Use to avoid unused variable warning
    }

    // Test xpath_parse_result::description
    {
        pugi::xpath_parse_result xpath_result;
        // Initialize with some state
        xpath_result.error = "test error";
        xpath_result.offset = static_cast<ptrdiff_t>(Size);
        const char* xpath_desc = xpath_result.description();
        (void)xpath_desc;
    }

    // Test xml_node::select_single_node and related functions
    {
        // Write data to dummy file for XML parsing
        writeDummyFile(Data, Size);
        
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
        
        // Get description from parse result
        const char* parse_desc = parse_result.description();
        (void)parse_desc;
        
        if (parse_result) {
            pugi::xml_node root = doc.root();
            
            // Create XPath query string from data (limit length to avoid overflow)
            size_t query_len = Size > 100 ? 100 : Size;
            std::string xpath_query(reinterpret_cast<const char*>(Data), query_len);
            
            // Test first overload of select_single_node (with char*)
            pugi::xpath_node node1 = root.select_single_node(xpath_query.c_str());
            (void)node1;
            
            // Test second overload of select_single_node (with xpath_query)
            try {
                pugi::xpath_query query(xpath_query.c_str());
                pugi::xpath_node node2 = root.select_single_node(query);
                (void)node2;
                
                // Test xpath_parse_result::description from valid query
                pugi::xpath_parse_result query_result = query.result();
                const char* query_desc = query_result.description();
                (void)query_desc;
            }
            catch (...) {
                // Catch any exception that might be thrown
                // xpath_exception may not be available in this build configuration
            }
        }
    }

    // Additional test with xpath_query that might fail
    {
        size_t query_len = Size > 50 ? 50 : Size;
        std::string xpath_query2(reinterpret_cast<const char*>(Data), query_len);
        
        try {
            pugi::xpath_query query2(xpath_query2.c_str());
            pugi::xpath_parse_result query_result2 = query2.result();
            const char* query_desc2 = query_result2.description();
            (void)query_desc2;
        }
        catch (...) {
            // Catch any exception
        }
    }

    // Test with empty nodes
    {
        pugi::xml_node empty_node;
        std::string empty_query("//test");
        pugi::xpath_node empty_result = empty_node.select_single_node(empty_query.c_str());
        (void)empty_result;
    }

    return 0;
}