// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test xml_parse_result::description
    {
        pugi::xml_parse_result parse_result;
        parse_result.description();
        
        // Initialize with some values
        parse_result.status = static_cast<pugi::xml_parse_status>(Data[0] % 20);
        parse_result.description();
    }

    // Test xpath_parse_result::description
    {
        pugi::xpath_parse_result xpath_parse_result;
        xpath_parse_result.description();
        
        // Initialize with some values
        xpath_parse_result.error = "test error";
        xpath_parse_result.offset = static_cast<ptrdiff_t>(Data[0]);
        xpath_parse_result.description();
    }

    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test xml_parse_result::description again with actual parse result
    parse_result.description();

    // If parsing succeeded, test node operations
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query from part of fuzzer data
        size_t query_size = Size > 100 ? 100 : Size;
        std::string xpath_query(reinterpret_cast<const char*>(Data), query_size);
        
        // Test first overload of select_single_node (with char*)
        try {
            pugi::xpath_node result = root.select_single_node(xpath_query.c_str());
            (void)result; // Use result to avoid unused variable warning
        } catch (...) {
            // Catch any exception but don't test xpath_exception methods
        }
        
        // Test second overload of select_single_node (with xpath_query)
        try {
            pugi::xpath_query query(xpath_query.c_str());
            pugi::xpath_node result = root.select_single_node(query);
            (void)result;
        } catch (...) {
            // Catch any exception but don't test xpath_exception methods
        }
        
        // Try with empty node
        pugi::xml_node empty_node;
        try {
            pugi::xpath_node result = empty_node.select_single_node(xpath_query.c_str());
            (void)result;
        } catch (...) {
            // Catch any exception but don't test xpath_exception methods
        }
    }

    // Test with file operations using "./dummy_file"
    {
        FILE* file = fopen("./dummy_file", "wb");
        if (file) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, file);
            fclose(file);
            
            pugi::xml_document file_doc;
            pugi::xml_parse_result file_result = file_doc.load_file("./dummy_file");
            file_result.description();
            
            remove("./dummy_file");
        }
    }

    return 0;
}