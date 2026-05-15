// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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

static void test_xpath_parse_result() {
    pugi::xpath_parse_result result;
    const char* desc = result.description();
    if (desc) {
        volatile auto ignore = desc;
        (void)ignore;
    }
    
    result.error = "Custom error";
    result.offset = 10;
    desc = result.description();
    if (desc) {
        volatile auto ignore = desc;
        (void)ignore;
    }
}

static void test_xml_parse_result() {
    pugi::xml_parse_result result;
    const char* desc = result.description();
    if (desc) {
        volatile auto ignore = desc;
        (void)ignore;
    }
    
    result.status = pugi::status_ok;
    desc = result.description();
    if (desc) {
        volatile auto ignore = desc;
        (void)ignore;
    }
    
    result.status = pugi::status_file_not_found;
    desc = result.description();
    if (desc) {
        volatile auto ignore = desc;
        (void)ignore;
    }
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    pugi::xml_document doc;
    const char* xml_content = "<root><node1>text1</node1><node2 attr=\"value\">text2</node2></root>";
    pugi::xml_parse_result parse_result = doc.load_string(xml_content);
    
    if (!parse_result) return;
    
    pugi::xml_node root = doc.root().first_child();
    if (!root) return;
    
    std::string query_str(reinterpret_cast<const char*>(data), std::min(size, (size_t)1000));
    
    pugi::xpath_variable_set vars;
    
    try {
        pugi::xpath_node result1 = root.select_single_node(query_str.c_str());
        pugi::xpath_node result2 = root.select_single_node(query_str.c_str(), &vars);
        
        pugi::xpath_query query_obj(query_str.c_str());
        pugi::xpath_node result3 = root.select_single_node(query_obj);
        
        volatile auto ignore1 = result1;
        volatile auto ignore2 = result2;
        volatile auto ignore3 = result3;
        (void)ignore1;
        (void)ignore2;
        (void)ignore3;
    } catch (...) {
        // Catch any exception to prevent crashes
    }
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t* data, size_t size) {
    test_xpath_parse_result();
    test_xml_parse_result();
    test_select_single_node(data, size);
    
    return 0;
}