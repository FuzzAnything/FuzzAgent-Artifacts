// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
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
#include <string>

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (!result) {
        // If parsing fails, create minimal valid XML to continue testing
        static const char* minimal_xml = "<root><child attr=\"value\">text</child></root>";
        doc.load_string(minimal_xml);
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // First xml_node::child call
    pugi::xml_node child1 = root.child("child");
    
    // xml_node::attribute call
    pugi::xml_attribute attr = child1.attribute("attr");
    
    // Second xml_node::child call
    pugi::xml_node child2 = root.child("node");
    
    // Create xpath_query from fuzzer input (use first part of data)
    pugi::xpath_query query;
    try {
        // Use up to 256 bytes for XPath expression, ensure null termination
        size_t xpath_size = Size > 256 ? 256 : Size;
        std::string xpath_str(reinterpret_cast<const char*>(Data), xpath_size);
        if (!xpath_str.empty()) {
            query = pugi::xpath_query(xpath_str.c_str());
        } else {
            query = pugi::xpath_query("//child");
        }
    } catch (...) {
        // If XPath compilation fails, use default query
        query = pugi::xpath_query("//child");
    }
    
    // Create xpath_node context
    pugi::xpath_node context(root);
    
    // Evaluate functions
    try {
        // evaluate_boolean
        (void)query.evaluate_boolean(context);
        
        // evaluate_number
        (void)query.evaluate_number(context);
        
        // First evaluate_string overload (returns string)
        (void)query.evaluate_string(context);
        
        // Second evaluate_string overload (writes to buffer)
        const size_t buffer_size = 256;
        char buffer[buffer_size];
        (void)query.evaluate_string(buffer, buffer_size, context);
        
        // Another evaluate_string call (first overload again)
        (void)query.evaluate_string(context);
        
        // evaluate_node_set
        pugi::xpath_node_set node_set = query.evaluate_node_set(context);
        
        // evaluate_node
        pugi::xpath_node node = query.evaluate_node(context);
        
    } catch (...) {
        // Catch all exceptions - fuzzer should continue
    }
    
    return 0;
}