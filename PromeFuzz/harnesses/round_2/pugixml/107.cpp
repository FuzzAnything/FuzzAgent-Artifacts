// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include "pugixml.hpp"

static void parse_xml_from_data(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        static const char* minimal_xml = "<root/>";
        doc.load_string(minimal_xml);
    } else {
        pugi::xml_parse_result result = doc.load_buffer(data, size);
        if (!result) {
            doc.load_string("<root/>");
            if (size > 0) {
                std::string content(reinterpret_cast<const char*>(data), std::min(size, static_cast<size_t>(1000)));
                doc.child("root").append_child(pugi::node_pcdata).set_value(content.c_str());
            }
        }
    }
}

static pugi::xpath_query create_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) {
        return pugi::xpath_query("*");
    }
    
    size_t xpath_size = std::min(size, static_cast<size_t>(1000));
    std::string xpath_str(reinterpret_cast<const char*>(data), xpath_size);
    
    xpath_str.erase(std::remove(xpath_str.begin(), xpath_str.end(), '\0'), xpath_str.end());
    
    if (xpath_str.empty()) {
        return pugi::xpath_query("*");
    }
    
    return pugi::xpath_query(xpath_str.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t* Data, size_t Size) {
    size_t xml_size = Size / 2;
    size_t xpath_size = Size - xml_size;
    const uint8_t* xml_data = Data;
    const uint8_t* xpath_data = Data + xml_size;
    
    pugi::xml_document doc;
    parse_xml_from_data(xml_data, xml_size, doc);
    
    pugi::xpath_query query = create_xpath_query(xpath_data, xpath_size);
    
    pugi::xml_node root = doc.root();
    pugi::xpath_node context(root);
    
    pugi::xml_node child = root.first_child();
    pugi::xpath_node child_context(child);
    
    try {
        std::string result_str = query.evaluate_string(context);
        (void)result_str;
    } catch (...) {
    }
    
    try {
        bool result_bool = query.evaluate_boolean(context);
        (void)result_bool;
    } catch (...) {
    }
    
    try {
        double result_num = query.evaluate_number(context);
        (void)result_num;
    } catch (...) {
    }
    
    try {
        pugi::xpath_node_set node_set = query.evaluate_node_set(context);
        (void)node_set;
        
        if (child) {
            pugi::xpath_node_set child_set = query.evaluate_node_set(child_context);
            (void)child_set;
        }
    } catch (...) {
    }
    
    try {
        pugi::xpath_node result_node = query.evaluate_node(context);
        (void)result_node;
        
        if (child) {
            pugi::xpath_node child_node = query.evaluate_node(child_context);
            (void)child_node;
        }
    } catch (...) {
    }
    
    try {
        pugi::xpath_node_set selected = root.select_nodes(query);
        (void)selected;
        
        if (child) {
            pugi::xpath_node_set child_selected = child.select_nodes(query);
            (void)child_selected;
        }
    } catch (...) {
    }
    
    if (xpath_size > 0) {
        std::string alt_xpath(reinterpret_cast<const char*>(xpath_data), 
                             std::min(xpath_size, static_cast<size_t>(100)));
        alt_xpath.erase(std::remove(alt_xpath.begin(), alt_xpath.end(), '\0'), alt_xpath.end());
        
        if (!alt_xpath.empty()) {
            alt_xpath = "//" + alt_xpath;
            pugi::xpath_query alt_query(alt_xpath.c_str());
            
            try {
                pugi::xpath_node_set alt_set = alt_query.evaluate_node_set(context);
                (void)alt_set;
            } catch (...) {
            }
        }
    }
    
    return 0;
}