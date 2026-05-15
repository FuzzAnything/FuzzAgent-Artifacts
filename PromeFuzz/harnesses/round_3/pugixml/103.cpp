// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

static pugi::xml_document create_document_from_data(const uint8_t *data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        doc.load_buffer(data, size);
    }
    return doc;
}

static std::string create_xpath_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return "/";
    }
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input: first half for XML, second half for XPath
    size_t xml_size = Size / 2;
    size_t xpath_size = Size - xml_size;
    const uint8_t* xml_data = Data;
    const uint8_t* xpath_data = Data + xml_size;

    // Create XML document
    pugi::xml_document doc = create_document_from_data(xml_data, xml_size);
    pugi::xml_node root = doc.root();
    pugi::xpath_node context(root);

    // Create XPath query
    std::string xpath_str = create_xpath_string(xpath_data, xpath_size);
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_str.c_str());
    } catch (...) {
        // Invalid XPath, skip further processing
        return 0;
    }

    // Test 1: evaluate_node_set
    try {
        pugi::xpath_node_set node_set = query.evaluate_node_set(context);
        (void)node_set;
    } catch (...) {
        // Exception caught, continue
    }

    // Test 2: select_nodes
    try {
        pugi::xpath_node_set selected_nodes = root.select_nodes(query);
        (void)selected_nodes;
    } catch (...) {
        // Exception caught, continue
    }

    // Test 3: evaluate_string
    try {
        pugi::string_t str_result = query.evaluate_string(context);
        (void)str_result;
    } catch (...) {
        // Exception caught, continue
    }

    // Test 4: evaluate_boolean
    try {
        bool bool_result = query.evaluate_boolean(context);
        (void)bool_result;
    } catch (...) {
        // Exception caught, continue
    }

    // Test 5: evaluate_node
    try {
        pugi::xpath_node node_result = query.evaluate_node(context);
        (void)node_result;
    } catch (...) {
        // Exception caught, continue
    }

    // Test 6: evaluate_number
    try {
        double num_result = query.evaluate_number(context);
        (void)num_result;
    } catch (...) {
        // Exception caught, continue
    }

    return 0;
}