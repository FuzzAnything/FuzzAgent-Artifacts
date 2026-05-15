// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
#include "pugixml.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <algorithm>

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    const char* desc1 = result.description();
    (void)desc1;
    
    for (int i = 0; i <= 20; ++i) {
        pugi::xml_parse_result res;
        res.status = static_cast<pugi::xml_parse_status>(i);
        const char* desc = res.description();
        (void)desc;
    }
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    const char* desc1 = result.description();
    (void)desc1;
    
    result.error = "Test error";
    result.offset = 10;
    const char* desc2 = result.description();
    (void)desc2;
}

static void test_select_single_node(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        if (Size > 0) {
            size_t query_size = std::min(Size, (size_t)100);
            std::string query_str(reinterpret_cast<const char*>(Data), query_size);
            
            pugi::xpath_node node1 = root.select_single_node(query_str.c_str());
            (void)node1;
            
            pugi::xpath_variable_set vars;
            pugi::xpath_node node2 = root.select_single_node(query_str.c_str(), &vars);
            (void)node2;
        }
        
        if (Size > 10) {
            size_t query_size = std::min(Size, (size_t)50);
            std::string query_str(reinterpret_cast<const char*>(Data), query_size);
            
            try {
                pugi::xpath_query query(query_str.c_str());
                if (query) {
                    pugi::xpath_node node = root.select_single_node(query);
                    (void)node;
                }
            } catch (...) {
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    test_xml_parse_result_description();
    
    test_xpath_parse_result_description();
    
    test_select_single_node(Data, Size);
    
    return 0;
}