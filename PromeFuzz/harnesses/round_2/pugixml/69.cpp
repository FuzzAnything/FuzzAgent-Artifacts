// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
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

static void test_xml_parse_result_description() {
    pugi::xml_parse_result result;
    const char* desc = result.description();
    (void)desc;
    
    result.status = pugi::status_ok;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_file_not_found;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_io_error;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_out_of_memory;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_internal_error;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_unrecognized_tag;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_pi;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_comment;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_cdata;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_doctype;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_pcdata;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_start_element;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_attribute;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_bad_end_element;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_end_element_mismatch;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_append_invalid_root;
    desc = result.description();
    (void)desc;
    
    result.status = pugi::status_no_document_element;
    desc = result.description();
    (void)desc;
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    const char* desc = result.description();
    (void)desc;
    
    result.error = "Test error";
    result.offset = 10;
    desc = result.description();
    (void)desc;
    
    result.error = nullptr;
    desc = result.description();
    (void)desc;
}

static void test_select_single_node(const pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    std::string query_str(reinterpret_cast<const char*>(data), size);
    
    try {
        pugi::xpath_node result1 = node.select_single_node(query_str.c_str());
        (void)result1;
        
        pugi::xpath_variable_set vars;
        pugi::xpath_node result2 = node.select_single_node(query_str.c_str(), &vars);
        (void)result2;
        
        pugi::xpath_query query(query_str.c_str());
        pugi::xpath_node result3 = node.select_single_node(query);
        (void)result3;
    } catch (...) {
        // Catch any exception but don't use xpath_exception specifically
    }
}

static void test_xpath_exception(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    pugi::xpath_parse_result parse_result;
    parse_result.error = "Test error";
    parse_result.offset = static_cast<ptrdiff_t>(size % 100);
    
    // We can't create xpath_exception directly, so we'll test the functions
    // by creating an xpath_query that might throw
    std::string query_str(reinterpret_cast<const char*>(data), size);
    try {
        pugi::xpath_query query(query_str.c_str());
        // If query creation succeeds, try to evaluate it on a dummy document
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child");
        
        pugi::xpath_node_set nodes = query.evaluate_node_set(root);
        (void)nodes;
    } catch (const std::exception& e) {
        const char* what_msg = e.what();
        (void)what_msg;
    }
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* data, size_t size) {
    static bool initialized = false;
    if (!initialized) {
        test_xml_parse_result_description();
        test_xpath_parse_result_description();
        initialized = true;
    }
    
    if (size == 0) return 0;
    
    std::string xml_content(reinterpret_cast<const char*>(data), size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str());
    
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
    
    if (parse_result) {
        pugi::xml_node root = doc.root();
        if (root) {
            test_select_single_node(root, data, size);
            
            pugi::xml_node child = root.first_child();
            if (child) {
                test_select_single_node(child, data, size);
            }
        }
    }
    
    test_xpath_exception(data, size);
    
    pugi::xpath_parse_result xpath_parse_result;
    xpath_parse_result.error = parse_result ? nullptr : "XML parse error";
    xpath_parse_result.offset = static_cast<ptrdiff_t>(size % 100);
    const char* xpath_desc = xpath_parse_result.description();
    (void)xpath_desc;
    
    std::ofstream dummy_file("./dummy_file", std::ios::binary);
    if (dummy_file) {
        dummy_file.write(reinterpret_cast<const char*>(data), size);
        dummy_file.close();
        
        pugi::xml_document file_doc;
        pugi::xml_parse_result file_result = file_doc.load_file("./dummy_file");
        const char* file_desc = file_result.description();
        (void)file_desc;
    }
    
    return 0;
}