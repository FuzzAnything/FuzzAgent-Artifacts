// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
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

static void test_xpath_parse_result(const pugi::xpath_parse_result& result) {
    // Test xpath_parse_result::description
    const char* desc = result.description();
    if (desc) {
        volatile char dummy = desc[0];
        (void)dummy;
    }
}

static void test_xml_parse_result(const pugi::xml_parse_result& result) {
    // Test description() function
    const char* desc = result.description();
    if (desc) {
        volatile char dummy = desc[0];
        (void)dummy;
    }
}

static void test_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create XPath query from fuzzer input
    std::string query_str(reinterpret_cast<const char*>(data), size);
    pugi::xpath_query query(query_str.c_str());
    
    // Test xpath_query::result
    const pugi::xpath_parse_result& result = query.result();
    test_xpath_parse_result(result);
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    // Split input into XML and XPath query
    size_t xml_size = size / 2;
    size_t xpath_size = size - xml_size;
    
    std::string xml_content(reinterpret_cast<const char*>(data), xml_size);
    std::string xpath_query(reinterpret_cast<const char*>(data + xml_size), xpath_size);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str());
    
    // Test xml_parse_result::description
    test_xml_parse_result(parse_result);
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.document_element();
        if (root) {
            // Test deprecated select_single_node function
            pugi::xpath_node selected = root.select_single_node(xpath_query.c_str());
            // The result may be empty, but we've exercised the API
            (void)selected;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test 1: xpath_parse_result::description
    pugi::xpath_parse_result xpath_result;
    xpath_result.error = "Fuzzer generated error";
    xpath_result.offset = Size % 100;
    test_xpath_parse_result(xpath_result);
    
    // Test 2: xml_parse_result::description
    pugi::xml_parse_result xml_result;
    xml_result.status = static_cast<pugi::xml_parse_status>(Data[0] % 20);
    xml_result.offset = Size;
    test_xml_parse_result(xml_result);
    
    // Test 3: xpath_query::result
    test_xpath_query(Data, Size);
    
    // Test 4: xml_node::select_single_node
    test_select_single_node(Data, Size);
    
    // Additional test: Create XML document from file
    if (Size > 10) {
        // Write XML content to dummy file
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            // Parse from file
            pugi::xml_document doc;
            pugi::xml_parse_result file_result = doc.load_file("./dummy_file");
            test_xml_parse_result(file_result);
            
            // Clean up
            remove("./dummy_file");
        }
    }
    
    return 0;
}