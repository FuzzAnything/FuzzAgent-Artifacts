// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <fstream>

#include "pugixml.hpp"

static void test_xml_parse_result() {
    pugi::xml_parse_result parse_result;
    
    // Test description() function
    const char* desc = parse_result.description();
    if (desc) {
        volatile char c = desc[0];
        (void)c;
    }
    
    // Set different statuses to explore different descriptions
    parse_result.status = pugi::status_ok;
    desc = parse_result.description();
    
    parse_result.status = pugi::status_file_not_found;
    desc = parse_result.description();
    
    parse_result.status = pugi::status_io_error;
    desc = parse_result.description();
    
    parse_result.status = pugi::status_out_of_memory;
    desc = parse_result.description();
    
    parse_result.status = pugi::status_internal_error;
    desc = parse_result.description();
}

static void test_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Create XPath query from fuzzer input
    std::string query_str(reinterpret_cast<const char*>(data), std::min(size, (size_t)1000));
    
    // Test xpath_query::result()
    pugi::xpath_query query;
    pugi::xpath_parse_result parse_result = query.result();
    
    // Test description() on the parse result
    const char* desc = parse_result.description();
    if (desc) {
        volatile char c = desc[0];
        (void)c;
    }
    
    // Try to construct xpath_query with the fuzzer input
    try {
        pugi::xpath_query query2(query_str.c_str());
        const pugi::xpath_parse_result& result = query2.result();
        
        // Test description() on the parse result
        const char* desc2 = result.description();
        if (desc2) {
            volatile char c = desc2[0];
            (void)c;
        }
        
        // Access error and offset fields
        if (result.error) {
            volatile char c = result.error[0];
            (void)c;
        }
        volatile ptrdiff_t offset = result.offset;
        (void)offset;
    } catch (const std::exception& e) {
        // Test exception handling
        const char* error_msg = e.what();
        if (error_msg) {
            volatile char c = error_msg[0];
            (void)c;
        }
    }
}

static void test_select_single_node(const uint8_t* data, size_t size) {
    if (size == 0) return;
    
    // Write fuzzer data to a file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();
    
    // Try to parse the file as XML
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
    
    // Test xml_parse_result::description()
    const char* desc = parse_result.description();
    if (desc) {
        volatile char c = desc[0];
        (void)c;
    }
    
    // If parsing succeeded, try to select nodes
    if (parse_result) {
        pugi::xml_node root = doc.root();
        
        // Try some XPath queries
        const char* queries[] = {
            "//*",
            "/",
            "node",
            "@*",
            "text()",
            "*",
            "."
        };
        
        for (const char* query : queries) {
            try {
                pugi::xpath_node node = root.select_single_node(query);
                (void)node;
            } catch (const std::exception& e) {
                // Test exception functions
                const char* error_msg = e.what();
                if (error_msg) {
                    volatile char c = error_msg[0];
                    (void)c;
                }
            }
        }
    }
    
    // Clean up
    remove("./dummy_file");
}

static void test_xpath_parse_result_description() {
    pugi::xpath_parse_result result;
    
    // Test with no error
    result.error = nullptr;
    result.offset = 0;
    const char* desc = result.description();
    if (desc) {
        volatile char c = desc[0];
        (void)c;
    }
    
    // Test with an error
    result.error = "XPath syntax error";
    result.offset = 5;
    desc = result.description();
    if (desc) {
        volatile char c = desc[0];
        (void)c;
    }
    
    // Test with different error messages
    const char* test_errors[] = {
        "Error",
        "",
        "Invalid expression",
        "Unexpected end of expression"
    };
    
    for (const char* error : test_errors) {
        result.error = error;
        desc = result.description();
        if (desc) {
            volatile char c = desc[0];
            (void)c;
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* data, size_t size) {
    // Test all target API functions
    
    // Test xml_parse_result::description()
    test_xml_parse_result();
    
    // Test xpath_query::result() and xpath_parse_result::description()
    test_xpath_query(data, size);
    
    // Test xml_node::select_single_node()
    test_select_single_node(data, size);
    
    // Test xpath_parse_result::description() directly
    test_xpath_parse_result_description();
    
    return 0;
}