// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <vector>

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
    
    // If parsing fails, create a simple default document
    if (!result) {
        const char* default_xml = "<root><node1 attr='value'>text</node1><node2>more text</node2></root>";
        doc.load_string(default_xml);
    }
    
    return doc;
}

static pugi::xpath_query createTestQuery(const uint8_t* data, size_t size) {
    // Use input data as XPath query string
    std::vector<char> query_str(size + 1);
    if (size > 0) {
        memcpy(query_str.data(), data, size);
    }
    query_str[size] = '\0';
    
    try {
        return pugi::xpath_query(query_str.data());
    } catch (...) {
        // If query creation fails, return a simple default query
        return pugi::xpath_query("//*");
    }
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Split input for different purposes
    size_t xml_size = Size / 2;
    size_t xpath_size = Size - xml_size;
    
    // Create test XML document
    pugi::xml_document doc = createTestDocument(Data, xml_size);
    
    // Create test XPath query
    pugi::xpath_query query = createTestQuery(Data + xml_size, xpath_size);
    
    // Get a context node for XPath evaluation
    pugi::xml_node root = doc.root();
    pugi::xpath_node xpath_context(root);
    
    // Test each target API function
    try {
        // 1. evaluate_string
        std::string str_result = query.evaluate_string(xpath_context);
        
        // 2. evaluate_boolean
        bool bool_result = query.evaluate_boolean(xpath_context);
        
        // 3. select_nodes
        pugi::xpath_node_set node_set1 = root.select_nodes(query);
        
        // 4. evaluate_node_set
        pugi::xpath_node_set node_set2 = query.evaluate_node_set(xpath_context);
        
        // 5. evaluate_number
        double num_result = query.evaluate_number(xpath_context);
        
        // 6. evaluate_node
        pugi::xpath_node single_node = query.evaluate_node(xpath_context);
        
        // Also test with file input
        writeDummyFile(Data, Size);
        pugi::xml_document file_doc;
        file_doc.load_file("./dummy_file");
        
        if (file_doc) {
            pugi::xml_node file_root = file_doc.root();
            pugi::xpath_node file_context(file_root);
            
            // Test with file-loaded document
            query.evaluate_string(file_context);
            query.evaluate_boolean(file_context);
            file_root.select_nodes(query);
            query.evaluate_node_set(file_context);
            query.evaluate_number(file_context);
            query.evaluate_node(file_context);
        }
        
    } catch (const std::bad_alloc&) {
        // Memory allocation errors - ignore during fuzzing
    } catch (...) {
        // Catch any other exceptions, including xpath_exception if it exists
    }
    
    // Test with empty/edge cases
    pugi::xpath_node empty_context;
    pugi::xml_node empty_node;
    pugi::xpath_query empty_query("");
    
    try {
        empty_query.evaluate_string(empty_context);
        empty_query.evaluate_boolean(empty_context);
        empty_node.select_nodes(empty_query);
        empty_query.evaluate_node_set(empty_context);
        empty_query.evaluate_number(empty_context);
        empty_query.evaluate_node(empty_context);
    } catch (...) {
        // Ignore exceptions in edge cases
    }
    
    return 0;
}