// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
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

static void test_xpath_exception(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create xpath_parse_result with error from fuzzer data
    pugi::xpath_parse_result parse_result;
    
    // Use first byte to determine error offset
    parse_result.offset = static_cast<ptrdiff_t>(data[0] % (size + 1));
    
    // Point error to a safe string (using a subset of fuzzer data)
    static char error_buffer[256];
    size_t error_len = size < 255 ? size : 255;
    memcpy(error_buffer, data, error_len);
    error_buffer[error_len] = '\0';
    parse_result.error = error_buffer;
    
    // Test description method
    const char* desc = parse_result.description();
    (void)desc; // Use result to avoid unused variable warning
    
    // Note: xpath_exception constructor may not be available in all builds
    // We'll skip direct instantiation and focus on other API functions
}

static void test_xml_parse_result(const uint8_t* data, size_t size) {
    pugi::xml_parse_result parse_result;
    
    // Set status based on fuzzer data
    if (size > 0) {
        // Use modulo to get a valid enum value (assuming max 20 status codes)
        parse_result.status = static_cast<pugi::xml_parse_status>(data[0] % 20);
    }
    
    // Test description method
    const char* desc = parse_result.description();
    (void)desc; // Use result to avoid unused variable warning
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    // Write fuzzer data to a file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) return;
    
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
    
    // Try to load XML from the file
    pugi::xml_document doc;
    pugi::xml_parse_result load_result = doc.load_file("./dummy_file");
    
    // Get the root node
    pugi::xml_node root = doc.root();
    
    // Create an XPath query from fuzzer data (safe subset)
    static char query_buffer[256];
    size_t query_len = size < 255 ? size : 255;
    memcpy(query_buffer, data, query_len);
    query_buffer[query_len] = '\0';
    
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node node1 = root.select_single_node(query_buffer);
    (void)node1; // Use result to avoid unused variable warning
    
    // Test second overload: select_single_node(const xpath_query& query)
    try {
        pugi::xpath_query xquery(query_buffer);
        pugi::xpath_node node2 = root.select_single_node(xquery);
        (void)node2; // Use result to avoid unused variable warning
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
    }
    
    // Clean up
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Test xpath_parse_result description
    test_xpath_exception(Data, Size);
    
    // Test xml_parse_result description
    test_xml_parse_result(Data, Size);
    
    // Test select_single_node with both overloads
    test_select_single_node(Data, Size);
    
    return 0;
}