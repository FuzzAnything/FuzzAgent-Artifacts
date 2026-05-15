// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include <exception>
#include "pugixml.hpp"

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // 1. Test pugi::xpath_parse_result::description
    {
        pugi::xpath_parse_result result1;
        const char* desc1 = result1.description();
        (void)desc1;
        
        pugi::xpath_parse_result result2;
        result2.error = "Test error";
        result2.offset = 10;
        const char* desc2 = result2.description();
        (void)desc2;
    }
    
    // 2. Test pugi::xml_parse_result::description
    {
        pugi::xml_parse_result xml_result;
        xml_result.status = pugi::status_ok;
        const char* xml_desc = xml_result.description();
        (void)xml_desc;
        
        pugi::xml_parse_result xml_result2;
        xml_result2.status = pugi::status_file_not_found;
        const char* xml_desc2 = xml_result2.description();
        (void)xml_desc2;
    }
    
    // 3. Test pugi::xpath_exception functions (only if exceptions are enabled)
    #ifdef PUGIXML_HAS_EXCEPTIONS
    {
        pugi::xpath_parse_result parse_result;
        parse_result.error = "XPath parsing error";
        parse_result.offset = 20;
        
        // Test by creating an xpath_query that might throw
        if (Size > 0) {
            std::string xpath_str(reinterpret_cast<const char*>(Data), 
                                 Size > 100 ? 100 : Size);
            try {
                pugi::xpath_query query(xpath_str.c_str());
                // If we get here, query is valid, create a dummy document
                pugi::xml_document doc;
                doc.load_string("<root><node>test</node></root>");
                pugi::xml_node root = doc.root();
                pugi::xpath_node node = root.select_single_node(query);
                (void)node;
            } catch (const pugi::xpath_exception& e) {
                // Test what()
                const char* what_msg = e.what();
                (void)what_msg;
                
                // Test result()
                const pugi::xpath_parse_result& exc_result = e.result();
                const char* exc_desc = exc_result.description();
                (void)exc_desc;
            }
        }
    }
    #endif
    
    // 4. Test pugi::xml_node::select_single_node
    {
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
        
        if (parse_result) {
            pugi::xml_node root = doc.root();
            
            // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            if (Size > 0) {
                size_t query_size = Size > 100 ? 100 : Size;
                std::string query_str(reinterpret_cast<const char*>(Data), query_size);
                
                pugi::xpath_node node1 = root.select_single_node(query_str.c_str());
                (void)node1;
                
                pugi::xpath_node node2 = root.select_single_node(query_str.c_str(), nullptr);
                (void)node2;
            }
            
            // Test second overload: select_single_node(const xpath_query& query)
            if (Size > 10) {
                size_t query_size = Size > 50 ? 50 : Size;
                std::string xpath_query_str(reinterpret_cast<const char*>(Data), query_size);
                
                #ifdef PUGIXML_HAS_EXCEPTIONS
                try {
                    pugi::xpath_query query(xpath_query_str.c_str());
                    pugi::xpath_node node3 = root.select_single_node(query);
                    (void)node3;
                } catch (const pugi::xpath_exception& e) {
                    const char* what_msg = e.what();
                    (void)what_msg;
                    const pugi::xpath_parse_result& exc_result = e.result();
                    const char* exc_desc = exc_result.description();
                    (void)exc_desc;
                }
                #else
                // If exceptions are disabled, just try to create the query
                pugi::xpath_query query(xpath_query_str.c_str());
                if (query) {
                    pugi::xpath_node node3 = root.select_single_node(query);
                    (void)node3;
                }
                #endif
            }
        }
    }
    
    // 5. Additional tests
    {
        pugi::xml_node empty_node;
        pugi::xml_attribute empty_attr;
        pugi::xpath_node empty_xpath_node;
        (void)empty_xpath_node;
        
        if (Size > 0) {
            FILE* dummy_file = fopen("./dummy_file", "wb");
            if (dummy_file) {
                fwrite(Data, 1, Size, dummy_file);
                fclose(dummy_file);
                
                pugi::xml_document doc_from_file;
                pugi::xml_parse_result file_result = doc_from_file.load_file("./dummy_file");
                (void)file_result;
            }
        }
    }
    
    return 0;
}