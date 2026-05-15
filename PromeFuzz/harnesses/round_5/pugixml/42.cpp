// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>

static void writeDummyFile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based operations
    writeDummyFile(Data, Size);
    
    // Initialize XML document
    pugi::xml_document doc;
    
    // Try to load XML from the input data
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get the root node (or empty node if parsing failed)
    pugi::xml_node root = doc.root();
    
    // Create xpath_variable_set for select_node overload
    pugi::xpath_variable_set variables;
    
    // Create xpath_query objects
    // Use a simple XPath query that might work with various XML structures
    const char* query_str = "//*";
    pugi::xpath_query query(query_str);
    
    // First select_node (string overload)
    pugi::xpath_node xnode1 = root.select_node(query_str, &variables);
    
    // Second select_node (xpath_query overload)
    pugi::xpath_node xnode2 = root.select_node(query);
    
    // First node() call
    pugi::xml_node node1 = xnode1.node();
    
    // Second node() call
    pugi::xml_node node2 = xnode2.node();
    
    // Third select_node (string overload) - try with different query
    const char* query_str2 = "/*";
    pugi::xpath_node xnode3 = root.select_node(query_str2, &variables);
    
    // Fourth select_node (xpath_query overload) - create new query
    pugi::xpath_query query2("//node()");
    pugi::xpath_node xnode4 = root.select_node(query2);
    
    // Fifth select_node (string overload) - try with attribute query
    const char* query_str3 = "//@*";
    pugi::xpath_node xnode5 = root.select_node(query_str3, &variables);
    
    // Sixth node() call
    pugi::xml_node node3 = xnode3.node();
    
    // Seventh node() call
    pugi::xml_node node4 = xnode4.node();
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}