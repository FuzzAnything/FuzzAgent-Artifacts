// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    pugi::xml_document doc;
    // Create a minimal valid XML document to ensure we have a root node to query
    if (!doc.load_string("<root><child>text</child></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Split input data into two parts for two different queries
    size_t split = Size / 2;
    if (split == 0) split = 1;

    // Create null-terminated strings for queries
    // We ensure null termination even if the fuzzer provides binary data
    std::string query1_str(reinterpret_cast<const char*>(Data), split);
    std::string query2_str(reinterpret_cast<const char*>(Data + split), Size - split);
    
    // Ensure null termination for C-string compatibility
    query1_str += '\0';
    query2_str += '\0';

    const char* q1 = query1_str.c_str();
    const char* q2 = query2_str.c_str();

    // 1. Invoke pugi::xml_node::select_single_node (overload 1: xpath_query)
    pugi::xpath_node result1_empty;
    try {
        pugi::xpath_query query_obj1(q1);
        result1_empty = root.select_single_node(query_obj1);
    } catch (...) {
        // Handle potential exceptions if PUGIXML_NO_EXCEPTIONS is not defined
    }

    // 2. Invoke pugi::xml_node::select_single_node (overload 2: const char_t*)
    pugi::xpath_node result2_empty;
    try {
        result2_empty = root.select_single_node(q2);
    } catch (...) {
        // Handle potential exceptions
    }

    // 3. Invoke pugi::xpath_node::node on the first result
    pugi::xml_node node1 = result1_empty.node();
    (void)node1; // Use the result to avoid unused variable warnings

    // 4. Invoke pugi::xpath_node::node on the second result
    pugi::xml_node node2 = result2_empty.node();
    (void)node2; // Use the result to avoid unused variable warnings

    return 0;
}