// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    const char* desc = result.description();
    (void)desc;
    
    result.error = "Test error";
    result.offset = 10;
    const char* desc2 = result.description();
    (void)desc2;
}

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    const char* desc = result.description();
    (void)desc;
    
    result.status = pugi::status_ok;
    const char* desc2 = result.description();
    (void)desc2;
    
    result.status = pugi::status_file_not_found;
    const char* desc3 = result.description();
    (void)desc3;
}

static void test_select_single_node(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    if (parse_result.status == pugi::status_ok) {
        pugi::xml_node root = doc.root();
        
        if (Size > 10) {
            std::string query_str(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)100));
            
            try {
                pugi::xpath_node node1 = root.select_single_node(query_str.c_str());
                (void)node1;
                
                pugi::xpath_query query(query_str.c_str());
                pugi::xpath_node node2 = root.select_single_node(query);
                (void)node2;
            } catch (...) {
                // Catch any exception
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    test_xpath_parse_result_description();
    test_xml_parse_result_description();
    test_select_single_node(Data, Size);
    
    return 0;
}