// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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

static void test_xpath_functions(pugi::xml_document& doc, const char* xpath_expr) {
    pugi::xpath_query query(xpath_expr);
    pugi::xpath_parse_result parse_result = query.result();
    
    if (!parse_result) {
        return;
    }
    
    pugi::xml_node root = doc.root();
    pugi::xpath_node context(root);
    
    try {
        std::string str_result = query.evaluate_string(context);
        (void)str_result;
    } catch (...) {
    }
    
    try {
        bool bool_result = query.evaluate_boolean(context);
        (void)bool_result;
    } catch (...) {
    }
    
    try {
        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
        (void)node_set_result;
    } catch (...) {
    }
    
    try {
        double num_result = query.evaluate_number(context);
        (void)num_result;
    } catch (...) {
    }
    
    try {
        pugi::xpath_node node_result = query.evaluate_node(context);
        (void)node_result;
    } catch (...) {
    }
    
    try {
        pugi::xpath_node_set select_result = root.select_nodes(query);
        (void)select_result;
    } catch (...) {
    }
}

static const char* get_xml_content(const uint8_t* data, size_t size) {
    static char buffer[4096];
    if (size >= sizeof(buffer)) {
        size = sizeof(buffer) - 1;
    }
    memcpy(buffer, data, size);
    buffer[size] = '\0';
    return buffer;
}

static const char* get_xpath_expr(const uint8_t* data, size_t size, size_t offset) {
    static char buffer[1024];
    if (offset >= size) {
        return "//*";
    }
    size_t remaining = size - offset;
    if (remaining >= sizeof(buffer)) {
        remaining = sizeof(buffer) - 1;
    }
    memcpy(buffer, data + offset, remaining);
    buffer[remaining] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t* data, size_t size) {
    if (size < 2) {
        return 0;
    }
    
    pugi::xml_document doc;
    
    size_t split_point = size / 2;
    const char* xml_content = get_xml_content(data, split_point);
    const char* xpath_expr = get_xpath_expr(data, size, split_point);
    
    pugi::xml_parse_result parse_result = doc.load_string(xml_content);
    
    if (!parse_result) {
        static const char* default_xml = "<root><a>test</a><b attr='value'>text</b></root>";
        doc.load_string(default_xml);
    }
    
    test_xpath_functions(doc, xpath_expr);
    
    return 0;
}