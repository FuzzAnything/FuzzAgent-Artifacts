// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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

static void writeDummyFile(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    writeDummyFile(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // Prepare XPath query string from input data
    size_t query_len = Size > 100 ? 100 : Size;
    char query[101];
    memcpy(query, Data, query_len);
    query[query_len] = '\0';

    // First overload: select_single_node with xpath_query
    try {
        pugi::xpath_query xpath_query_obj(query);
        if (xpath_query_obj) {
            pugi::xpath_node xnode1 = root.select_single_node(xpath_query_obj);
            pugi::xml_node node1 = xnode1.node();
            if (node1) {
                // Access node to explore code paths
                const char* name = node1.name();
                (void)name;
            }
        }
    } catch (...) {
        // Ignore XPath compilation errors
    }

    // Second overload: select_single_node with char_t* and optional variables
    try {
        pugi::xpath_variable_set vars;
        pugi::xpath_node xnode2 = root.select_single_node(query, &vars);
        pugi::xml_node node2 = xnode2.node();
        if (node2) {
            // Access node to explore code paths
            const char* name = node2.name();
            (void)name;
        }
    } catch (...) {
        // Ignore XPath evaluation errors
    }

    // Clean up dummy file
    remove("./dummy_file");
    return 0;
}