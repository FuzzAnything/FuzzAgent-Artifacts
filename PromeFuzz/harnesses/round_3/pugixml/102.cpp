// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
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
#include <string>

static void test_xml_parse_result(const pugi::xml_parse_result& parse_result) {
    // Target function 3: pugi::xml_parse_result::description
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_xpath_parse_result(const pugi::xpath_parse_result& parse_result) {
    // Target function 5: pugi::xpath_parse_result::description
    const char* desc = parse_result.description();
    (void)desc;
}

static void test_select_single_node(pugi::xml_node& node, const char* query) {
    // Target function 4: pugi::xml_node::select_single_node (first overload)
    pugi::xpath_variable_set vars;
    pugi::xpath_node result1 = node.select_single_node(query, &vars);
    (void)result1;
    
    // Target function 4: pugi::xml_node::select_single_node (second overload)
    pugi::xpath_query xpath_query(query);
    pugi::xpath_node result2 = node.select_single_node(xpath_query);
    (void)result2;
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Test xml_parse_result::description
    test_xml_parse_result(parse_result);
    
    // Create default objects for testing
    pugi::xpath_parse_result xpath_parse_result;
    
    // Test xpath_parse_result::description
    test_xpath_parse_result(xpath_parse_result);
    
    // If parsing succeeded, test select_single_node
    if (parse_result) {
        // Create a simple XPath query from the data
        std::string query_str;
        size_t query_len = Size > 100 ? 100 : Size;
        query_str.assign(reinterpret_cast<const char*>(Data), query_len);
        
        // Replace null bytes to avoid early termination
        for (char& c : query_str) {
            if (c == 0) c = 'a';
        }
        
        // Test on document element
        pugi::xml_node root = doc.document_element();
        if (root) {
            test_select_single_node(root, query_str.c_str());
        }
        
        // Also test on first child node
        for (pugi::xml_node child : doc.children()) {
            test_select_single_node(child, query_str.c_str());
            break;
        }
    }
    
    // Additional test: write data to file and parse from file
    std::ofstream out_file("./dummy_file", std::ios::binary);
    if (out_file) {
        out_file.write(reinterpret_cast<const char*>(Data), Size);
        out_file.close();
        
        pugi::xml_document doc_from_file;
        pugi::xml_parse_result file_result = doc_from_file.load_file("./dummy_file");
        test_xml_parse_result(file_result);
    }
    
    return 0;
}