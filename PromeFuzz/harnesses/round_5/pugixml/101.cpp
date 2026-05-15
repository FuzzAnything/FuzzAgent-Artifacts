// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document for testing
    static const char* dummy_xml = 
        "<?xml version=\"1.0\"?>\n"
        "<root>\n"
        "  <node1 attr=\"value1\">Text1</node1>\n"
        "  <node2 attr=\"value2\">Text2</node2>\n"
        "  <node3 attr=\"value3\">Text3</node3>\n"
        "</root>";
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(dummy_xml);
    
    if (parse_result) {
        pugi::xml_node root = doc.root().first_child();
        
        // Test xml_node::select_single_node overloads
        if (Size > 0) {
            // Create null-terminated XPath query from fuzz data
            size_t query_len = Size < 100 ? Size : 100;
            char* query = new char[query_len + 1];
            memcpy(query, Data, query_len);
            query[query_len] = '\0';
            
            // First overload: select_single_node(const char_t* query, xpath_variable_set* variables)
            pugi::xpath_node result1 = root.select_single_node(query);
            (void)result1; // Use result to avoid unused variable warning
            
            // Second overload: select_single_node(const xpath_query& query)
            pugi::xpath_query xpath_query_obj(query);
            pugi::xpath_parse_result parse_res = xpath_query_obj.result();
            if (parse_res) {
                pugi::xpath_node result2 = root.select_single_node(xpath_query_obj);
                (void)result2; // Use result to avoid unused variable warning
            }
            
            // Test xpath_parse_result::description() on successful parse
            const char* parse_desc = parse_res.description();
            (void)parse_desc;
            
            // Test with exception handling if PUGIXML_NO_EXCEPTIONS is not defined
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
                // Try to create an xpath_query that might throw
                pugi::xpath_query xpath_query_obj2("invalid xpath [");
                pugi::xpath_node result3 = root.select_single_node(xpath_query_obj2);
                (void)result3;
            } catch (const pugi::xpath_exception& e) {
                // Test xpath_exception::what()
                const char* what_msg = e.what();
                (void)what_msg;
                
                // Test xpath_exception::result()
                const pugi::xpath_parse_result& ex_result = e.result();
                const char* ex_desc = ex_result.description();
                (void)ex_desc;
            }
            #endif
            
            delete[] query;
        }
        
        // Test xpath_parse_result::description() on default object
        pugi::xpath_parse_result xpath_parse_res;
        const char* xpath_desc = xpath_parse_res.description();
        (void)xpath_desc;
        
        // Test xml_parse_result::description()
        const char* xml_desc = parse_result.description();
        (void)xml_desc;
        
        // Test with invalid XML to trigger different parse results
        if (Size > 0) {
            // Write fuzz data to dummy file
            std::ofstream outfile("./dummy_file", std::ios::binary);
            if (outfile.is_open()) {
                outfile.write(reinterpret_cast<const char*>(Data), Size);
                outfile.close();
                
                pugi::xml_document doc2;
                pugi::xml_parse_result file_result = doc2.load_file("./dummy_file");
                
                const char* file_desc = file_result.description();
                (void)file_desc;
                
                // Try to select nodes even if parsing failed
                if (file_result) {
                    pugi::xml_node root2 = doc2.root().first_child();
                    if (root2) {
                        pugi::xpath_node dummy_result = root2.select_single_node("//*");
                        (void)dummy_result;
                    }
                }
            }
        }
    }
    
    return 0;
}