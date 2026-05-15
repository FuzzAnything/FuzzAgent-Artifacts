// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <memory>

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // Test xml_parse_result::description()
    {
        pugi::xml_parse_result parse_result;
        parse_result.status = static_cast<pugi::xml_parse_status>(Data[0] % 10);
        const char* desc = parse_result.description();
        (void)desc;
    }

    // Test xpath_parse_result::description()
    {
        pugi::xpath_parse_result xpath_result;
        xpath_result.error = "test error";
        xpath_result.offset = Size % 100;
        const char* desc = xpath_result.description();
        (void)desc;
    }

    // Create a dummy XML document for testing select_single_node
    if (Size > 0) {
        // Write data to a dummy file
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            fwrite(Data, 1, Size, file);
            fclose(file);
            
            // Parse the XML document
            pugi::xml_document doc;
            pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
            
            if (parse_result) {
                // Create a simple XPath query from the input data
                std::string xpath_query;
                if (Size > 0) {
                    // Use first few bytes to create a simple XPath query
                    size_t query_len = Size % 100;
                    if (query_len > Size) query_len = Size;
                    xpath_query = std::string(reinterpret_cast<const char*>(Data), query_len);
                } else {
                    xpath_query = "/*";
                }
                
                // Get the root node
                pugi::xml_node root = doc.document_element();
                if (root) {
                    // Test first overload of select_single_node (with char_t*)
                    pugi::xpath_node node1 = root.select_single_node(xpath_query.c_str());
                    (void)node1;
                    
                    // Test second overload of select_single_node (with xpath_query)
                    pugi::xpath_query query(xpath_query.c_str());
                    pugi::xpath_node node2 = root.select_single_node(query);
                    (void)node2;
                }
            }
            
            // Clean up dummy file
            remove("./dummy_file");
        }
    }

    return 0;
}