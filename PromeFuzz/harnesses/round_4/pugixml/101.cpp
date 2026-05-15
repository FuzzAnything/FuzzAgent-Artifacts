// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    static std::string xml_buffer;
    static std::string xpath_buffer;
    
    // Split input into XML and XPath parts
    size_t xml_size = Size / 2;
    size_t xpath_size = Size - xml_size;
    
    xml_buffer.assign(reinterpret_cast<const char*>(Data), xml_size);
    xpath_buffer.assign(reinterpret_cast<const char*>(Data + xml_size), xpath_size);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.c_str(), xml_buffer.size());
    
    if (!parse_result) {
        // If XML parsing fails, try with a simple dummy document
        const char* dummy_xml = "<root><element>text</element></root>";
        parse_result = doc.load_buffer(dummy_xml, strlen(dummy_xml));
        if (!parse_result) {
            return 0;
        }
    }
    
    // Create context node (use document root)
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.root();
    }
    pugi::xpath_node context(root);
    
    // Create XPath query
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_buffer.c_str());
    } catch (...) {
        // If XPath parsing fails, use a simple query
        query = pugi::xpath_query("//*");
    }
    
    // Test evaluate_string
    try {
        std::string result_str = query.evaluate_string(context).c_str();
        (void)result_str; // Use result to avoid optimization
    } catch (...) {
        // Handle exceptions if PUGIXML_NO_EXCEPTIONS is not defined
    }
    
    // Test evaluate_boolean
    try {
        bool result_bool = query.evaluate_boolean(context);
        (void)result_bool;
    } catch (...) {
        // Handle exceptions
    }
    
    // Test evaluate_number
    try {
        double result_num = query.evaluate_number(context);
        (void)result_num;
    } catch (...) {
        // Handle exceptions
    }
    
    // Test evaluate_node
    try {
        pugi::xpath_node result_node = query.evaluate_node(context);
        (void)result_node;
    } catch (...) {
        // Handle exceptions
    }
    
    // Test evaluate_node_set
    try {
        pugi::xpath_node_set result_set = query.evaluate_node_set(context);
        (void)result_set;
        
        // Iterate through node set to explore more states
        for (pugi::xpath_node_set::const_iterator it = result_set.begin(); it != result_set.end(); ++it) {
            pugi::xml_node node = it->node();
            pugi::xml_attribute attr = it->attribute();
            (void)node;
            (void)attr;
        }
    } catch (...) {
        // Handle exceptions
    }
    
    // Test select_nodes
    try {
        pugi::xpath_node_set selected = root.select_nodes(query);
        (void)selected;
    } catch (...) {
        // Handle exceptions
    }
    
    // Test with empty nodes and default constructors
    pugi::xpath_node empty_xpath_node;
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_attr;
    pugi::xpath_query empty_query;
    
    // Test evaluate functions with empty context
    try {
        (void)query.evaluate_string(empty_xpath_node);
        (void)query.evaluate_boolean(empty_xpath_node);
        (void)query.evaluate_number(empty_xpath_node);
        (void)query.evaluate_node(empty_xpath_node);
        (void)query.evaluate_node_set(empty_xpath_node);
    } catch (...) {
        // Handle exceptions
    }
    
    // Test with file-based XML (if needed)
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(xml_buffer.c_str(), xml_buffer.size());
        file.close();
        
        pugi::xml_document file_doc;
        pugi::xml_parse_result file_result = file_doc.load_file("./dummy_file");
        if (file_result) {
            pugi::xml_node file_root = file_doc.document_element();
            pugi::xpath_node file_context(file_root);
            
            try {
                (void)query.evaluate_string(file_context);
                (void)query.evaluate_boolean(file_context);
                (void)query.evaluate_number(file_context);
                (void)query.evaluate_node(file_context);
                (void)query.evaluate_node_set(file_context);
            } catch (...) {
                // Handle exceptions
            }
        }
    }
    
    // Test xpath_node_set constructor with iterator range
    pugi::xpath_node_set empty_set;
    pugi::xpath_node_set constructed_set(empty_set.begin(), empty_set.end(), pugi::xpath_node_set::type_unsorted);
    (void)constructed_set;
    
    // Test xpath_parse_result default constructor
    pugi::xpath_parse_result parse_res;
    (void)parse_res;
    
    // Test move constructor of xpath_query
    pugi::xpath_query moved_query = std::move(query);
    (void)moved_query;
    
    return 0;
}