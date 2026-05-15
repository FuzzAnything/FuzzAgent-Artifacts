// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include <vector>

// Check if xpath_exception is available (depends on PUGIXML_NO_EXCEPTIONS)
#ifdef PUGIXML_NO_EXCEPTIONS
// xpath_exception is not available when exceptions are disabled
static void test_xpath_exception_alternative() {
    // Create xpath_parse_result directly
    pugi::xpath_parse_result parse_result;
    parse_result.error = "Test error";
    parse_result.offset = 0;
    
    // Test pugi::xpath_parse_result::description
    const char* desc = parse_result.description();
    if (desc) {
        volatile auto dummy = desc[0];
        (void)dummy;
    }
}
#else
static void test_xpath_exception(const pugi::xpath_exception& e) {
    // Test pugi::xpath_exception::what
    const char* what_msg = e.what();
    if (what_msg) {
        volatile auto dummy = what_msg[0];
        (void)dummy;
    }
    
    // Test pugi::xpath_exception::result
    const pugi::xpath_parse_result& parse_result = e.result();
    
    // Test pugi::xpath_parse_result::description
    const char* desc = parse_result.description();
    if (desc) {
        volatile auto dummy = desc[0];
        (void)dummy;
    }
}
#endif

static void test_xml_parse_result(const pugi::xml_parse_result& result) {
    // Test pugi::xml_parse_result::description
    const char* desc = result.description();
    if (desc) {
        volatile auto dummy = desc[0];
        (void)dummy;
    }
}

static void test_select_single_node(pugi::xml_node& node, const char* query_str) {
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node xnode1 = node.select_single_node(query_str, nullptr);
    (void)xnode1;
    
    // Test second overload: select_single_node(const xpath_query& query)
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        pugi::xpath_query query(query_str);
        pugi::xpath_node xnode2 = node.select_single_node(query);
        (void)xnode2;
    } catch (const pugi::xpath_exception& e) {
        test_xpath_exception(e);
    }
    #else
    pugi::xpath_query query(query_str);
    pugi::xpath_node xnode2 = node.select_single_node(query);
    (void)xnode2;
    #endif
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test xml_parse_result::description
    test_xml_parse_result(parse_result);
    
    // Create a simple XPath query from the data
    std::string xpath_query;
    if (Size > 0) {
        // Use first part of data for XPath query (max 256 chars)
        size_t query_len = Size > 256 ? 256 : Size;
        xpath_query.assign(reinterpret_cast<const char*>(Data), query_len);
        
        // Ensure null termination
        if (!xpath_query.empty() && xpath_query.back() != '\0') {
            xpath_query.push_back('\0');
        }
    }
    
    // Test XPath parsing
    if (!xpath_query.empty()) {
        #ifndef PUGIXML_NO_EXCEPTIONS
        try {
            pugi::xpath_query query(xpath_query.c_str());
            
            // Test xpath_parse_result::description from valid query
            const pugi::xpath_parse_result& xpath_result = query.result();
            const char* desc = xpath_result.description();
            if (desc) {
                volatile auto dummy = desc[0];
                (void)dummy;
            }
            
            // Test select_single_node on document root
            pugi::xml_node root = doc.root();
            if (root) {
                test_select_single_node(root, xpath_query.c_str());
            }
            
        } catch (...) {
            // Catch any exception from xpath_query constructor
            // Don't rethrow or call test_xpath_exception since we can't guarantee
            // the exception type when PUGIXML_NO_EXCEPTIONS might be defined differently
        }
        #else
        pugi::xpath_query query(xpath_query.c_str());
        
        // Test xpath_parse_result::description from query
        const pugi::xpath_parse_result& xpath_result = query.result();
        const char* desc = xpath_result.description();
        if (desc) {
            volatile auto dummy = desc[0];
            (void)dummy;
        }
        
        // Test select_single_node on document root
        pugi::xml_node root = doc.root();
        if (root) {
            test_select_single_node(root, xpath_query.c_str());
        }
        #endif
    }
    
    // Test with empty node
    pugi::xml_node empty_node;
    if (!xpath_query.empty()) {
        test_select_single_node(empty_node, xpath_query.c_str());
    }
    
    // Test xpath_parse_result default constructor
    pugi::xpath_parse_result default_xpath_result;
    const char* default_desc = default_xpath_result.description();
    if (default_desc) {
        volatile auto dummy = default_desc[0];
        (void)dummy;
    }
    
    // Test creating xpath_exception from parse result (only if exceptions are enabled)
    #ifndef PUGIXML_NO_EXCEPTIONS
    pugi::xpath_parse_result error_result;
    error_result.error = "Test error";
    error_result.offset = 0;
    
    try {
        throw pugi::xpath_exception(error_result);
    } catch (const pugi::xpath_exception& e) {
        test_xpath_exception(e);
    } catch (...) {
        // Catch any other exception
    }
    #else
    test_xpath_exception_alternative();
    #endif
    
    return 0;
}