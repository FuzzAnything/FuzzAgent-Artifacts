// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input to a C-string, ensuring null termination
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';
    
    const char* input_str = buffer.data();
    const pugi::char_t* xpath_query = reinterpret_cast<const pugi::char_t*>(input_str);

    pugi::xml_document doc;
    
    // Load the document from the fuzzer input
    // If parsing fails, create a minimal document to continue testing
    if (!doc.load_string(input_str)) {
        doc.append_child("root");
    }

    pugi::xml_node root = doc.first_child();
    if (root.empty()) {
        root = doc;
    }

    // Prepare variables for the first overload
    pugi::xpath_variable_set variables;
    
    // Invoke select_single_node with query string and variables
    // This function can throw an exception if the query is invalid.
    // We wrap it in a try-catch block to handle exceptions gracefully.
    pugi::xpath_node node1;
    try {
        node1 = root.select_single_node(xpath_query, &variables);
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
        node1 = pugi::xpath_node();
    }
    
    // Invoke select_single_node with pre-compiled query
    // The constructor of xpath_query can also throw if the query is invalid.
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(xpath_query);
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
        query = pugi::xpath_query();
    }
    
    pugi::xpath_node node2;
    try {
        node2 = root.select_single_node(query);
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
        node2 = pugi::xpath_node();
    }
    
    // Invoke xpath_node::node() on the results
    // These calls are safe and won't throw exceptions
    pugi::xml_node xml_node1 = node1.node();
    pugi::xml_node xml_node2 = node2.node();
    
    return 0;
}