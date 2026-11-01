// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Load XML from fuzz input
    pugi::xml_document doc;
    
    // Try loading as string first
    std::string input(reinterpret_cast<const char*>(Data), Size);
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    
    // If parsing fails, try loading from file (dummy_file)
    if (!result) {
        write_dummy_file(Data, Size);
        result = doc.load_file("./dummy_file");
    }
    
    if (!result) {
        // If both fail, create a minimal valid document to test API behavior
        doc.append_child("root").append_child("child");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare XPath variable set
    pugi::xpath_variable_set variables;

    // Fuzz select_nodes
    // Wrap in try-catch to handle xpath_exception thrown on invalid queries
    // Note: pugixml throws pugi::xpath_exception which is in the pugi namespace
    // but the error suggests it might not be visible or named differently in some builds.
    // However, standard pugixml defines it as pugi::xpath_exception.
    // If the build environment is non-standard, we catch std::exception as a fallback.
    try {
        pugi::xpath_node_set set = root.select_nodes(input.c_str(), &variables);
        (void)set; // Suppress unused variable warning
    } catch (...) {
        // Expected behavior for invalid queries
    }

    // Fuzz select_single_node (deprecated overloads)
    try {
        // Overload 1: const char_t* query
        pugi::xpath_node node1 = root.select_single_node(input.c_str(), &variables);
        if (node1) {
            pugi::xml_node underlying = node1.node();
            (void)underlying;
        }

        // Overload 2: const xpath_query& query
        // Note: xpath_query constructor can also throw, so we wrap it
        pugi::xpath_query query(input.c_str());
        pugi::xpath_node node2 = root.select_single_node(query);
        if (node2) {
            pugi::xml_node underlying = node2.node();
            (void)underlying;
        }
    } catch (...) {
        // Expected behavior for invalid queries
    }

    // Fuzz select_node
    try {
        // Overload 1: const char_t* query
        pugi::xpath_node node1 = root.select_node(input.c_str(), &variables);
        if (node1) {
            pugi::xml_node underlying = node1.node();
            (void)underlying;
        }

        // Overload 2: const xpath_query& query
        pugi::xpath_query query(input.c_str());
        pugi::xpath_node node2 = root.select_node(query);
        if (node2) {
            pugi::xml_node underlying = node2.node();
            (void)underlying;
        }
    } catch (...) {
        // Expected behavior for invalid queries
    }

    // Fuzz xpath_node::node() directly on various nodes
    // Try to get nodes from the document
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
        // Create an xpath_node from an attribute if exists
        if (node.attribute("test")) {
            pugi::xpath_node attr_node(node.attribute("test"), node);
            pugi::xml_node underlying = attr_node.node();
            (void)underlying;
        }
        
        // Create an xpath_node from the element itself (using empty attribute to test empty node case)
        pugi::xpath_node elem_node(pugi::xml_attribute(), node); 
        pugi::xml_node underlying = elem_node.node();
        (void)underlying;
    }

    return 0;
}