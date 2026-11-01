// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
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
#include <pugixml.hpp>

static pugi::xml_document parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.load_string("<root/>");
        return doc;
    }
    
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    if (!result) {
        doc.load_string("<root><item id='1'><child>text</child></item></root>");
    }
    
    return doc;
}

static pugi::xpath_query compile_query(const uint8_t* data, size_t size) {
    pugi::xpath_query query;
    
    if (size == 0) {
        try {
            query = pugi::xpath_query("true()");
        } catch (...) {
            return query;
        }
        return query;
    }
    
    std::string expr(reinterpret_cast<const char*>(data), size);
    try {
        query = pugi::xpath_query(expr.c_str());
    } catch (...) {
        return query;
    }
    
    return query;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    pugi::xml_document doc = parse_document(Data, split);
    pugi::xpath_query query = compile_query(Data + split, Size - split);

    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc;
    }

    // 1. pugi::xml_node::child
    pugi::xml_node node1 = root.child("item");
    if (!node1) {
        node1 = root.child("root");
        if (!node1) {
            node1 = root;
        }
    }

    // 2. pugi::xml_node::attribute
    pugi::xml_attribute attr = node1.attribute("id");
    (void)attr; 

    // 3. pugi::xml_node::child (second call)
    pugi::xml_node node2 = node1.child("child");
    if (!node2) {
        node2 = node1.child("item");
        if (!node2) {
            node2 = node1;
        }
    }

    pugi::xpath_node context_node(node2);

    // 4. pugi::xpath_query::evaluate_boolean
    bool bool_res = false;
    try {
        bool_res = query.evaluate_boolean(context_node);
    } catch (...) {
    }

    // 5. pugi::xpath_query::evaluate_number
    double num_res = 0.0;
    try {
        num_res = query.evaluate_number(context_node);
    } catch (...) {
    }

    // 6. pugi::xpath_query::evaluate_string (overload 1: buffer)
    char buffer[256];
    size_t str_len = 0;
    try {
        str_len = query.evaluate_string(buffer, sizeof(buffer), context_node);
    } catch (...) {
    }

    // 7. pugi::xpath_query::evaluate_string (overload 2: string_t)
    // string_t is defined as pugi::string_t, not inside xpath_query
    pugi::string_t str_res;
    try {
        str_res = query.evaluate_string(context_node);
    } catch (...) {
    }

    // 8. pugi::xpath_query::evaluate_node_set
    pugi::xpath_node_set node_set;
    try {
        node_set = query.evaluate_node_set(context_node);
    } catch (...) {
    }

    // 9. pugi::xpath_query::evaluate_node
    pugi::xpath_node node_res;
    try {
        node_res = query.evaluate_node(context_node);
    } catch (...) {
    }

    (void)bool_res;
    (void)num_res;
    (void)str_len;
    (void)str_res;
    (void)node_set;
    (void)node_res;

    return 0;
}