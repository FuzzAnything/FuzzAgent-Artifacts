// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* Data, size_t Size) {
    // Write fuzzer input to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // 1. Test pugi::xpath_parse_result::description()
    {
        pugi::xpath_parse_result result1;
        const char* desc1 = result1.description();
        (void)desc1; // Use to avoid unused variable warning
        
        // Create another with different state
        pugi::xpath_parse_result result2;
        // Note: error field is const char*, we can't modify it directly
        // but we can create via xpath_query which sets it
        try {
            std::string xpath_str(reinterpret_cast<const char*>(Data), 
                                 Size > 100 ? 100 : Size);
            pugi::xpath_query query(xpath_str.c_str());
            pugi::xpath_parse_result result3 = query.result();
            const char* desc3 = result3.description();
            (void)desc3;
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // 2. Test pugi::xml_node::select_single_node (both overloads)
    {
        pugi::xml_document doc;
        
        // Try to parse from dummy file
        pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
        
        // Also try parsing from buffer
        if (Size > 0) {
            pugi::xml_parse_result parse_result2 = doc.load_buffer(Data, Size);
            (void)parse_result2;
        }
        
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
        if (Size > 0) {
            std::string query_str(reinterpret_cast<const char*>(Data), 
                                 Size > 200 ? 200 : Size);
            pugi::xpath_variable_set vars;
            
            // Call without variables
            pugi::xpath_node node1 = root.select_single_node(query_str.c_str());
            (void)node1;
            
            // Call with variables (nullptr)
            pugi::xpath_node node2 = root.select_single_node(query_str.c_str(), nullptr);
            (void)node2;
            
            // Call with variables set
            pugi::xpath_node node3 = root.select_single_node(query_str.c_str(), &vars);
            (void)node3;
        }
        
        // Test second overload: select_single_node(const xpath_query& query)
        if (Size > 0) {
            std::string query_str(reinterpret_cast<const char*>(Data), 
                                 Size > 150 ? 150 : Size);
            try {
                pugi::xpath_query query(query_str.c_str());
                pugi::xpath_node node4 = root.select_single_node(query);
                (void)node4;
            } catch (...) {
                // Ignore exceptions from invalid xpath_query
            }
        }
        
        // Try with empty node
        pugi::xml_node empty_node;
        pugi::xpath_node empty_result = empty_node.select_single_node("test");
        (void)empty_result;
    }
    
    // 3. Test pugi::xml_parse_result::description()
    {
        pugi::xml_parse_result result1;
        const char* desc1 = result1.description();
        (void)desc1;
        
        // Create parse result from document loading
        pugi::xml_document doc;
        pugi::xml_parse_result result2 = doc.load_file("./dummy_file");
        const char* desc2 = result2.description();
        (void)desc2;
        
        // Try loading from buffer
        if (Size > 0) {
            pugi::xml_parse_result result3 = doc.load_buffer(Data, Size);
            const char* desc3 = result3.description();
            (void)desc3;
        }
        
        // Try with string input
        if (Size > 0) {
            std::string xml_str(reinterpret_cast<const char*>(Data), 
                               Size > 300 ? 300 : Size);
            pugi::xml_parse_result result4 = doc.load_string(xml_str.c_str());
            const char* desc4 = result4.description();
            (void)desc4;
        }
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}