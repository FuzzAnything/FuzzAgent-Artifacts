// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
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

static pugi::xml_document createTestDocument() {
    pugi::xml_document doc;
    const char* xml_content = 
        "<root>"
        "<node1 attr1=\"value1\">Text1</node1>"
        "<node2 attr2=\"value2\">Text2</node2>"
        "<node3>"
        "<child1>ChildText1</child1>"
        "<child2 attr3=\"value3\">ChildText2</child2>"
        "</node3>"
        "</root>";
    doc.load_string(xml_content);
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }

    // Prepare dummy file if needed
    writeDummyFile(data, size);

    // Create test XML document
    pugi::xml_document doc = createTestDocument();
    pugi::xml_node root = doc.root();
    pugi::xml_node context_node = root.first_child();
    pugi::xpath_node xpath_context(context_node);

    // Create XPath query from fuzzer input (treat as string)
    std::string xpath_str(reinterpret_cast<const char*>(data), size);
    if (xpath_str.empty()) {
        return 0;
    }

    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_str.c_str());
    } catch (...) {
        // If query construction fails, return early
        return 0;
    }

    // Check if query is valid
    if (!query) {
        return 0;
    }

    // Test evaluate_node_set
    try {
        pugi::xpath_node_set node_set = query.evaluate_node_set(xpath_context);
        (void)node_set; // Use result to avoid unused variable warning
    } catch (...) {
        // Handle exceptions if PUGIXML_NO_EXCEPTIONS is not defined
    }

    // Test evaluate_node
    try {
        pugi::xpath_node single_node = query.evaluate_node(xpath_context);
        (void)single_node;
    } catch (...) {
        // Handle exceptions
    }

    // Test evaluate_boolean
    try {
        bool bool_result = query.evaluate_boolean(xpath_context);
        (void)bool_result;
    } catch (...) {
        // Handle exceptions
    }

    // Test evaluate_number
    try {
        double num_result = query.evaluate_number(xpath_context);
        (void)num_result;
    } catch (...) {
        // Handle exceptions
    }

    // Test evaluate_string
    try {
        pugi::string_t str_result = query.evaluate_string(xpath_context);
        (void)str_result;
    } catch (...) {
        // Handle exceptions
    }

    // Test select_nodes on xml_node
    try {
        pugi::xpath_node_set selected_nodes = root.select_nodes(query);
        (void)selected_nodes;
    } catch (...) {
        // Handle exceptions
    }

    // Try with different context nodes
    if (root.first_child()) {
        pugi::xpath_node alt_context(root.first_child());
        
        try {
            pugi::xpath_node_set alt_set = query.evaluate_node_set(alt_context);
            (void)alt_set;
        } catch (...) {}
        
        try {
            pugi::xpath_node alt_node = query.evaluate_node(alt_context);
            (void)alt_node;
        } catch (...) {}
    }

    // Test with attribute as context
    if (root.first_child().first_attribute()) {
        pugi::xml_attribute attr = root.first_child().first_attribute();
        pugi::xml_node owning_node = root.first_child(); // Get the node that owns this attribute
        pugi::xpath_node attr_context(attr, owning_node);
        
        try {
            pugi::xpath_node_set attr_set = query.evaluate_node_set(attr_context);
            (void)attr_set;
        } catch (...) {}
    }

    return 0;
}