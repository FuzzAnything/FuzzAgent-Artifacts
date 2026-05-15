// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void test_select_single_node(pugi::xml_node& node, const std::string& xpath_str) {
    // First overload: const char_t* with optional variables
    pugi::xpath_variable_set vars;
    if (!xpath_str.empty()) {
        node.select_single_node(xpath_str.c_str());
        node.select_single_node(xpath_str.c_str(), &vars);
    }
    
    // Second overload: xpath_query
    pugi::xpath_query query(xpath_str.c_str());
    node.select_single_node(query);
}

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based parsing
    write_dummy_file(Data, Size);
    
    // Create XML document and parse from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test xml_parse_result::description
    const char* parse_desc = parse_result.description();
    (void)parse_desc;
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Create XPath string from input (ensuring null termination)
    std::string xpath_str;
    if (Size > 0) {
        size_t xpath_len = Size > 100 ? 100 : Size;
        xpath_str.assign(reinterpret_cast<const char*>(Data), xpath_len);
    } else {
        xpath_str = "//node";
    }
    
    // Test select_single_node overloads
    if (root) {
        test_select_single_node(root, xpath_str);
    }
    
    // Test xpath_parse_result::description
    pugi::xpath_parse_result xpath_parse_res;
    const char* xpath_parse_desc = xpath_parse_res.description();
    (void)xpath_parse_desc;
    
    // Create xpath_query to get parse result
    pugi::xpath_query query(xpath_str.c_str());
    const pugi::xpath_parse_result& query_result = query.result();
    const char* query_result_desc = query_result.description();
    (void)query_result_desc;
    
    // Test xpath_exception functions if exceptions are enabled
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        // Force exception by evaluating invalid query
        pugi::xml_node empty_node;
        if (empty_node) {
            pugi::xpath_query invalid_query("invalid[");
            empty_node.select_single_node(invalid_query);
        }
    } catch (const pugi::xpath_exception& e) {
        // Test xpath_exception::what
        const char* what_msg = e.what();
        (void)what_msg;
        
        // Test xpath_exception::result
        const pugi::xpath_parse_result& exc_result = e.result();
        (void)exc_result;
    } catch (...) {
        // Catch any other exceptions
    }
    #endif
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}