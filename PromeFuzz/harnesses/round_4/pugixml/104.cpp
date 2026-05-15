// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_document createTestDocument(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // If parsing fails, create a minimal valid XML document
    if (!result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child1").text().set("value1");
        root.append_child("child2").text().set("value2");
        pugi::xml_node child3 = root.append_child("child3");
        child3.append_attribute("attr1") = "attr_value1";
        child3.append_attribute("attr2") = "attr_value2";
    }
    
    return doc;
}

static std::string createXPathString(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "//*";
    }
    
    // Use the input data to create an XPath string
    // Limit length to avoid overly long strings
    size_t len = size > 100 ? 100 : size;
    std::string xpath;
    xpath.reserve(len + 1);
    
    for (size_t i = 0; i < len; ++i) {
        char c = static_cast<char>(data[i]);
        // Only include printable ASCII characters
        if (c >= 32 && c <= 126) {
            xpath.push_back(c);
        } else {
            xpath.push_back('*');
        }
    }
    
    // Ensure it's a valid XPath expression
    if (xpath.empty() || xpath.find_first_not_of(" \t\n\r") == std::string::npos) {
        return "//*";
    }
    
    return xpath;
}

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* data, size_t size) {
    // Create a dummy file with the input data
    writeDummyFile(data, size);
    
    // Create XML document from input
    pugi::xml_document doc = createTestDocument(data, size);
    
    // Create XPath query string from input
    std::string xpath_str = createXPathString(data, size);
    
    try {
        // Create XPath query object
        pugi::xpath_query query(xpath_str.c_str());
        
        // Get root node for context
        pugi::xml_node root = doc.document_element();
        if (!root) {
            root = doc.first_child();
        }
        
        // Create xpath_node for context
        pugi::xpath_node context(root);
        
        // Test evaluate_string
        try {
            std::string result_str = query.evaluate_string(context).c_str();
            (void)result_str; // Use result to avoid unused variable warning
        } catch (...) {
            // Ignore exceptions for fuzzing
        }
        
        // Test evaluate_boolean
        try {
            bool result_bool = query.evaluate_boolean(context);
            (void)result_bool;
        } catch (...) {
            // Ignore exceptions for fuzzing
        }
        
        // Test evaluate_number
        try {
            double result_num = query.evaluate_number(context);
            (void)result_num;
        } catch (...) {
            // Ignore exceptions for fuzzing
        }
        
        // Test evaluate_node
        try {
            pugi::xpath_node result_node = query.evaluate_node(context);
            (void)result_node;
        } catch (...) {
            // Ignore exceptions for fuzzing
        }
        
        // Test select_nodes
        try {
            pugi::xpath_node_set node_set = root.select_nodes(query);
            (void)node_set;
        } catch (...) {
            // Ignore exceptions for fuzzing
        }
        
        // Test evaluate_node_set
        try {
            pugi::xpath_node_set result_set = query.evaluate_node_set(context);
            (void)result_set;
        } catch (...) {
            // Ignore exceptions for fuzzing
        }
        
        // Test with different context nodes
        if (root) {
            // Test with attribute as context - create xpath_node from attribute
            pugi::xml_attribute attr = root.first_attribute();
            if (attr) {
                // Create xpath_node from attribute using the correct constructor order
                pugi::xpath_node attr_context(attr, root);
                try {
                    (void)query.evaluate_string(attr_context);
                    (void)query.evaluate_boolean(attr_context);
                    (void)query.evaluate_number(attr_context);
                    (void)query.evaluate_node(attr_context);
                    (void)query.evaluate_node_set(attr_context);
                } catch (...) {
                    // Ignore exceptions
                }
            }
            
            // Test with child node as context
            pugi::xml_node child = root.first_child();
            if (child) {
                pugi::xpath_node child_context(child);
                try {
                    (void)query.evaluate_string(child_context);
                    (void)query.evaluate_boolean(child_context);
                    (void)query.evaluate_number(child_context);
                    (void)query.evaluate_node(child_context);
                    (void)query.evaluate_node_set(child_context);
                } catch (...) {
                    // Ignore exceptions
                }
            }
        }
        
        // Test with empty context
        pugi::xpath_node empty_context;
        try {
            (void)query.evaluate_string(empty_context);
            (void)query.evaluate_boolean(empty_context);
            (void)query.evaluate_number(empty_context);
            (void)query.evaluate_node(empty_context);
            (void)query.evaluate_node_set(empty_context);
        } catch (...) {
            // Ignore exceptions
        }
        
    } catch (...) {
        // If xpath_query constructor throws, try with a simple default query
        try {
            pugi::xpath_query default_query("//*");
            pugi::xpath_node context(doc.document_element());
            
            (void)default_query.evaluate_string(context);
            (void)default_query.evaluate_boolean(context);
            (void)default_query.evaluate_number(context);
            (void)default_query.evaluate_node(context);
            (void)default_query.evaluate_node_set(context);
            
            if (doc.document_element()) {
                (void)doc.document_element().select_nodes(default_query);
            }
        } catch (...) {
            // Ignore all exceptions
        }
    }
    
    return 0;
}