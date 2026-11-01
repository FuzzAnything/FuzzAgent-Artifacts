// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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

// Helper function to parse input data into a valid XML document
static pugi::xml_document parse_xml_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // If data is empty or too small, create a minimal valid XML
    if (size == 0) {
        doc.load_string("<root/>");
        return doc;
    }

    // Ensure the data is null-terminated for string operations
    std::string str(reinterpret_cast<const char*>(data), size);
    
    // Try to load as XML string
    if (doc.load_string(str.c_str())) {
        return doc;
    }

    // If parsing fails, create a minimal valid XML to ensure we have a node to test
    doc.load_string("<root><child>test</child></root>");
    return doc;
}

// Helper function to generate a simple XPath query from fuzz data
static std::string generate_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "//child";
    }

    // Create a safe query string by filtering out problematic characters
    std::string query;
    query.reserve(size);
    
    for (size_t i = 0; i < size; ++i) {
        uint8_t c = data[i];
        // Allow alphanumeric, some common XPath characters, and whitespace
        if (c >= 32 && c < 127 && c != '"' && c != '\'' && c != '<' && c != '>' && c != '&' && c != '/') {
            query.push_back(static_cast<char>(c));
        } else if (c == '/' || c == '.' || c == '@' || c == '*' || c == '[' || c == ']' || c == '(' || c == ')') {
            query.push_back(static_cast<char>(c));
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            query.push_back(static_cast<char>(c));
        }
    }

    // Ensure we have at least a simple valid query
    if (query.empty()) {
        query = "//child";
    }

    return query;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse input into XML document
    pugi::xml_document doc = parse_xml_from_fuzz_data(Data, Size);
    
    // Get root node for testing
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // If no document element, create one
        root = doc.append_child("root");
    }

    // Generate XPath query from fuzz data
    std::string query_str = generate_xpath_query(Data, Size);
    const pugi::char_t* query = query_str.c_str();

    // Test 1: select_single_node with xpath_query object
    // Wrap in try-catch to handle potential exceptions from invalid queries
    pugi::xpath_node result1;
    try {
        pugi::xpath_query compiled_query(query, nullptr);
        result1 = root.select_single_node(compiled_query);
    } catch (...) {
        // If query compilation fails, result1 remains empty
        result1 = pugi::xpath_node();
    }
    
    // Test 2: select_single_node with string (deprecated overload)
    // This overload also throws exceptions on compilation errors in some builds
    // We need to wrap it in try-catch as well
    pugi::xpath_variable_set variables;
    pugi::xpath_node result2;
    try {
        result2 = root.select_single_node(query, &variables);
    } catch (...) {
        // If query compilation fails, result2 remains empty
        result2 = pugi::xpath_node();
    }

    // Test 3: xpath_node::node() on first result
    pugi::xml_node node1 = result1.node();
    
    // Test 4: xpath_node::node() on second result
    pugi::xml_node node2 = result2.node();

    // Additional exploration: try with different nodes
    pugi::xml_node child = root.first_child();
    if (child) {
        pugi::xpath_node result3;
        try {
            pugi::xpath_query compiled_query2(query, nullptr);
            result3 = child.select_single_node(compiled_query2);
        } catch (...) {
            result3 = pugi::xpath_node();
        }
        pugi::xml_node node3 = result3.node();
        
        pugi::xpath_node result4;
        try {
            result4 = child.select_single_node(query, &variables);
        } catch (...) {
            result4 = pugi::xpath_node();
        }
        pugi::xml_node node4 = result4.node();
    }

    // Check node properties to explore more states
    (void)node1.empty();
    (void)node2.empty();
    
    if (node1) {
        (void)node1.name();
        (void)node1.type();
    }
    
    if (node2) {
        (void)node2.name();
        (void)node2.type();
    }

    return 0;
}