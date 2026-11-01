// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

static bool is_valid_xml_prefix(const uint8_t* data, size_t size) {
    if (size < 4) return false;
    const char* str = reinterpret_cast<const char*>(data);
    size_t len = size;
    if (len > 100) len = 100;
    for (size_t i = 0; i < len; ++i) {
        if (str[i] == '<') return true;
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    pugi::xml_document doc;
    pugi::xpath_variable_set vars;
    pugi::xpath_query query;
    pugi::xpath_node context_node;

    // 1. pugi::xpath_variable_set::add
    // Try to add variables with different types to explore states
    const char* var_name = (Data[0] >= 32 && Data[0] < 127) ? reinterpret_cast<const char*>(Data) : "var";
    size_t name_len = std::min(Size, static_cast<size_t>(100));
    char var_buf[101];
    std::memcpy(var_buf, Data, name_len);
    var_buf[name_len] = '\0';
    
    pugi::xpath_variable* v1 = vars.add(var_buf, pugi::xpath_type_string);
    pugi::xpath_variable* v2 = vars.add(var_buf, pugi::xpath_type_number);
    pugi::xpath_variable* v3 = vars.add(var_buf, pugi::xpath_type_boolean);
    pugi::xpath_variable* v4 = vars.add(var_buf, pugi::xpath_type_node_set);
    
    (void)v1; (void)v2; (void)v3; (void)v4;

    // 2. pugi::xml_document::load_buffer
    const char* xml_content = nullptr;
    size_t xml_size = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (is_valid_xml_prefix(Data, Size)) {
        xml_content = reinterpret_cast<const char*>(Data);
        xml_size = Size;
    } else {
        xml_content = reinterpret_cast<const char*>(Data);
        xml_size = Size;
    }

    pugi::xml_parse_result result = doc.load_buffer(xml_content, xml_size, pugi::parse_default, encoding);
    
    if (doc.document_element()) {
        context_node = pugi::xpath_node(doc.document_element());
    }

    // Prepare a query string
    char query_buf[256];
    size_t query_len = std::min(Size, static_cast<size_t>(255));
    std::memcpy(query_buf, Data, query_len);
    query_buf[query_len] = '\0';
    
    // Compile the query
    // Note: The constructor requires a pointer to xpath_variable_set
    try {
        query = pugi::xpath_query(query_buf, &vars);
    } catch (...) {
        // If query compilation fails, proceed with potentially invalid query object
    }

    // 3. pugi::xpath_query::evaluate_boolean
    try {
        bool b = query.evaluate_boolean(context_node);
        (void)b;
    } catch (...) {
        // Handle exceptions
    }

    // 4. pugi::xpath_query::evaluate_number
    try {
        double n = query.evaluate_number(context_node);
        (void)n;
    } catch (...) {
        // Handle exceptions
    }

    // 5. pugi::xpath_query::evaluate_string
    // Return type is pugi::string_t (which is char_t*), not xpath_string
    try {
        pugi::string_t s = query.evaluate_string(context_node);
        (void)s;
    } catch (...) {
        // Handle exceptions
    }

    // 6. pugi::xpath_query::evaluate_node
    try {
        pugi::xpath_node node = query.evaluate_node(context_node);
        (void)node;
    } catch (...) {
        // Handle exceptions
    }

    // 7. pugi::xpath_query::evaluate_node_set
    try {
        pugi::xpath_node_set nodes = query.evaluate_node_set(context_node);
        (void)nodes;
    } catch (...) {
        // Handle exceptions
    }

    return 0;
}