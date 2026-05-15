// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13447:36 in pugixml.hpp
// pugi::xml_node::select_node at pugixml.cpp:13453:36 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // If parsing failed, we can still test with empty document
    // but many operations will be on empty nodes
    
    // Get the document element (root node)
    pugi::xml_node root = doc.document_element();
    
    // Create some XPath query strings from the fuzzer data
    // Use a portion of the data for XPath queries
    size_t xpath_size = Size > 100 ? 100 : Size;
    std::string xpath_str1, xpath_str2, xpath_str3;
    
    if (xpath_size > 0) {
        xpath_str1.assign(reinterpret_cast<const char*>(Data), xpath_size);
        // Ensure null termination
        if (xpath_str1.size() > 0) {
            xpath_str1[xpath_str1.size() - 1] = '\0';
        }
        
        // Create another XPath string using different portion
        size_t offset = xpath_size / 2;
        if (offset < Size) {
            size_t len = Size - offset;
            len = len > 50 ? 50 : len;
            xpath_str2.assign(reinterpret_cast<const char*>(Data + offset), len);
            if (xpath_str2.size() > 0) {
                xpath_str2[xpath_str2.size() - 1] = '\0';
            }
        }
        
        // Create third XPath string
        if (Size > 10) {
            xpath_str3.assign(reinterpret_cast<const char*>(Data), 10);
            if (xpath_str3.size() > 0) {
                xpath_str3[xpath_str3.size() - 1] = '\0';
            }
        }
    }
    
    // If we couldn't create valid strings, use simple defaults
    if (xpath_str1.empty()) xpath_str1 = "//*";
    if (xpath_str2.empty()) xpath_str2 = "//node";
    if (xpath_str3.empty()) xpath_str3 = "/";
    
    try {
        // Create xpath_query objects for the overload that takes xpath_query
        pugi::xpath_query query1(xpath_str1.c_str());
        pugi::xpath_query query2(xpath_str2.c_str());
        pugi::xpath_query query3(xpath_str3.c_str());
        
        // Create xpath_variable_set for the overload that takes variables
        pugi::xpath_variable_set variables;
        
        // 1. First select_node (const char_t* overload)
        pugi::xpath_node result1 = root.select_node(xpath_str1.c_str());
        
        // 2. Second select_node (xpath_query overload)
        pugi::xpath_node result2 = root.select_node(query1);
        
        // 3. First node() call
        pugi::xml_node node1 = result1.node();
        
        // 4. Second node() call
        pugi::xml_node node2 = result2.node();
        
        // 5. Third select_node (const char_t* overload with variables)
        pugi::xpath_node result3 = root.select_node(xpath_str2.c_str(), &variables);
        
        // 6. Fourth select_node (const char_t* overload)
        pugi::xpath_node result4 = root.select_node(xpath_str3.c_str());
        
        // 7. Fifth select_node (xpath_query overload)
        pugi::xpath_node result5 = root.select_node(query2);
        
        // 8. Third node() call
        pugi::xml_node node3 = result3.node();
        
        // 9. Fourth node() call
        pugi::xml_node node4 = result4.node();
        
        // Additional: try select_node on the nodes we got from node() calls
        if (node1) {
            pugi::xpath_node nested1 = node1.select_node(query3);
            pugi::xml_node nested_node1 = nested1.node();
        }
        
        if (node2) {
            pugi::xpath_node nested2 = node2.select_node(xpath_str1.c_str());
            pugi::xml_node nested_node2 = nested2.node();
        }
        
    } catch (...) {
        // Catch all exceptions to prevent the fuzzer from crashing
    }
    
    return 0;
}