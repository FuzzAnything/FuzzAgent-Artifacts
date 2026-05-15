// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13458:40 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        // Create minimal valid XML if input is empty
        doc.load_string("<root/>");
    } else {
        // Use input as XML content, but ensure it's null-terminated
        std::string xml_content(reinterpret_cast<const char*>(data), size);
        doc.load_buffer(xml_content.c_str(), xml_content.size());
    }
    return doc;
}

static std::string create_xpath_query(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "//*";
    }
    // Create XPath query from input data
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Split input for different purposes
    size_t split = Size / 2;
    const uint8_t* xml_data = Data;
    size_t xml_size = split;
    const uint8_t* xpath_data = Data + split;
    size_t xpath_size = Size - split;

    // Test 1: xml_parse_result::description
    {
        pugi::xml_document doc;
        pugi::xml_parse_result parse_result;
        
        if (xml_size > 0) {
            std::string xml_string(reinterpret_cast<const char*>(xml_data), xml_size);
            parse_result = doc.load_string(xml_string.c_str());
        } else {
            parse_result = doc.load_string("<root/>");
        }
        
        // Call description() function
        const char* desc = parse_result.description();
        (void)desc; // Use result to avoid optimization
    }

    // Test 2: xpath_query::result and xpath_parse_result::description
    {
        std::string xpath_query_str;
        if (xpath_size > 0) {
            xpath_query_str = std::string(reinterpret_cast<const char*>(xpath_data), 
                                         std::min(xpath_size, (size_t)100));
        } else {
            xpath_query_str = "//node";
        }
        
        try {
            pugi::xpath_query query(xpath_query_str.c_str());
            
            // Get result and call description
            const pugi::xpath_parse_result& parse_result = query.result();
            const char* error_desc = parse_result.description();
            (void)error_desc;
        } catch (const std::exception& e) {
            // We can only call what() on std::exception
            const char* what_msg = e.what();
            (void)what_msg;
        } catch (...) {
            // Ignore other exceptions
        }
    }

    // Test 3: xml_node::select_single_node (deprecated)
    {
        pugi::xml_document doc = create_test_document(xml_data, xml_size);
        pugi::xml_node root = doc.root();
        
        std::string xpath_str;
        if (xpath_size > 0) {
            xpath_str = std::string(reinterpret_cast<const char*>(xpath_data), 
                                   std::min(xpath_size, (size_t)50));
        } else {
            xpath_str = "//*";
        }
        
        // Test with null variables - wrap in try-catch
        try {
            pugi::xpath_node result1 = root.select_single_node(xpath_str.c_str(), nullptr);
            (void)result1;
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test with allocated variables - wrap in try-catch
        try {
            pugi::xpath_variable_set vars;
            pugi::xpath_node result2 = root.select_single_node(xpath_str.c_str(), &vars);
            (void)result2;
        } catch (...) {
            // Ignore exceptions
        }
        
        // Test with empty query - wrap in try-catch
        try {
            pugi::xpath_node result3 = root.select_single_node("");
            (void)result3;
        } catch (...) {
            // Ignore exceptions
        }
    }

    // Test 4: xpath_exception functions (what() and result())
    {
        // Create an xpath_exception by trying to compile invalid XPath
        std::string invalid_xpath;
        if (xpath_size > 0) {
            invalid_xpath = std::string(reinterpret_cast<const char*>(xpath_data), xpath_size);
        } else {
            invalid_xpath = "invalid[";
        }
        
        try {
            pugi::xpath_query query(invalid_xpath.c_str());
            // If we get here, query compiled successfully
            // Try to evaluate it on empty document
            pugi::xml_document doc;
            doc.select_nodes(invalid_xpath.c_str());
        } catch (const std::exception& e) {
            // We can only call what() on std::exception
            const char* what_msg = e.what();
            (void)what_msg;
        } catch (...) {
            // Ignore other exceptions
        }
    }

    // Test 5: Additional coverage for xpath_parse_result::description
    {
        // Create xpath_parse_result directly
        pugi::xpath_parse_result parse_result;
        parse_result.error = "Test error";
        parse_result.offset = 0;
        
        const char* desc = parse_result.description();
        (void)desc;
        
        // Test with null error
        pugi::xpath_parse_result parse_result2;
        parse_result2.error = nullptr;
        parse_result2.offset = 0;
        
        const char* desc2 = parse_result2.description();
        (void)desc2;
    }

    return 0;
}