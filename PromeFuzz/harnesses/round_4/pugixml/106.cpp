// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static pugi::xml_document createDocumentFromData(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size > 0) {
        std::string xml_string(reinterpret_cast<const char*>(data), size);
        doc.load_string(xml_string.c_str());
    } else {
        doc.load_string("<root/>");
    }
    return doc;
}

static pugi::xpath_query createXPathQuery(const uint8_t* data, size_t size) {
    if (size == 0) {
        return pugi::xpath_query("/*");
    }
    
    std::string xpath_string(reinterpret_cast<const char*>(data), size);
    if (xpath_string.size() > 1000) {
        xpath_string.resize(1000);
    }
    
    try {
        return pugi::xpath_query(xpath_string.c_str());
    } catch (...) {
        return pugi::xpath_query("/*");
    }
}

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t* Data, size_t Size) {
    writeDummyFile(Data, Size);
    
    pugi::xml_document doc = createDocumentFromData(Data, Size);
    pugi::xml_node root = doc.root();
    if (!root) {
        doc.load_string("<root><child>text</child><child attr='value'>content</child></root>");
        root = doc.root();
    }
    
    size_t xpath_size = Size / 2;
    const uint8_t* xpath_data = Data + (Size - xpath_size);
    pugi::xpath_query query = createXPathQuery(xpath_data, xpath_size);
    
    pugi::xpath_node context(root);
    
    pugi::xml_node first_child = root.first_child();
    pugi::xpath_node attr_context;
    if (first_child) {
        pugi::xml_attribute attr = first_child.first_attribute();
        if (attr) {
            attr_context = pugi::xpath_node(attr, first_child);
        }
    }
    
    try {
        std::string str_result = query.evaluate_string(context);
        if (attr_context.node() || attr_context.attribute()) {
            std::string attr_str_result = query.evaluate_string(attr_context);
        }
    } catch (...) {}
    
    try {
        bool bool_result = query.evaluate_boolean(context);
        if (attr_context.node() || attr_context.attribute()) {
            bool attr_bool_result = query.evaluate_boolean(attr_context);
        }
    } catch (...) {}
    
    try {
        double num_result = query.evaluate_number(context);
        if (attr_context.node() || attr_context.attribute()) {
            double attr_num_result = query.evaluate_number(attr_context);
        }
    } catch (...) {}
    
    try {
        pugi::xpath_node node_result = query.evaluate_node(context);
        if (attr_context.node() || attr_context.attribute()) {
            pugi::xpath_node attr_node_result = query.evaluate_node(attr_context);
        }
    } catch (...) {}
    
    try {
        pugi::xpath_node_set node_set_result = query.evaluate_node_set(context);
        if (attr_context.node() || attr_context.attribute()) {
            pugi::xpath_node_set attr_node_set_result = query.evaluate_node_set(attr_context);
        }
    } catch (...) {}
    
    try {
        pugi::xpath_node_set select_result = root.select_nodes(query);
        if (first_child) {
            pugi::xpath_node_set child_select_result = first_child.select_nodes(query);
        }
    } catch (...) {}
    
    try {
        pugi::xpath_query moved_query = std::move(query);
        std::string moved_str_result = moved_query.evaluate_string(context);
    } catch (...) {}
    
    pugi::xml_attribute default_attr;
    pugi::xpath_node_set default_set;
    pugi::xpath_node default_xpath_node(default_attr, root);
    
    try {
        pugi::xpath_query default_query("/*");
        pugi::xpath_node_set default_query_result = default_query.evaluate_node_set(default_xpath_node);
    } catch (...) {}
    
    std::remove("./dummy_file");
    
    return 0;
}