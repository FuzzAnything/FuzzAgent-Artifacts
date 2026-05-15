// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xpath_parse_result::description at pugixml.cpp:12914:47 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13469:36 in pugixml.hpp
// pugi::xml_node::select_single_node at pugixml.cpp:13475:36 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_query::result at pugixml.cpp:13428:54 in pugixml.hpp
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

static void test_xpath_exception(const pugi::xpath_parse_result& result) {
    // Test pugi::xpath_exception functions by creating an exception object
    // Note: xpath_exception constructor may not be available in all builds
    // We'll test the functions if the exception type exists
    #ifndef PUGIXML_NO_EXCEPTIONS
    try {
        // Create xpath_exception using constructor
        pugi::xpath_exception exc(result);
        
        // Test pugi::xpath_exception::what
        const char* what_msg = exc.what();
        (void)what_msg; // Use to avoid unused variable warning
        
        // Test pugi::xpath_exception::result
        const pugi::xpath_parse_result& exc_result = exc.result();
        (void)exc_result; // Use to avoid unused variable warning
    } catch (...) {
        // Catch any exceptions that might be thrown
    }
    #endif
}

static void test_xml_parse_result(const pugi::xml_parse_result& result) {
    // Test pugi::xml_parse_result::description
    const char* desc = result.description();
    (void)desc; // Use to avoid unused variable warning
}

static void test_xpath_parse_result(const pugi::xpath_parse_result& result) {
    // Test pugi::xpath_parse_result::description
    const char* desc = result.description();
    (void)desc; // Use to avoid unused variable warning
}

static void test_select_single_node(pugi::xml_node& node, const char* query, pugi::xpath_variable_set* variables) {
    // Test first overload: select_single_node(const char_t* query, xpath_variable_set* variables)
    pugi::xpath_node result1 = node.select_single_node(query, variables);
    (void)result1; // Use to avoid unused variable warning
    
    // Test second overload: select_single_node(const xpath_query& query)
    pugi::xpath_query xpath_query_obj(query);
    pugi::xpath_node result2 = node.select_single_node(xpath_query_obj);
    (void)result2; // Use to avoid unused variable warning
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(Data, Size);
    
    // Test pugi::xml_parse_result::description
    test_xml_parse_result(parse_result);
    
    if (parse_result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Create XPath query from fuzzer input (use first 256 bytes or less)
        size_t query_size = Size > 256 ? 256 : Size;
        std::string xpath_query(reinterpret_cast<const char*>(Data), query_size);
        
        // Test select_single_node with different parameters
        pugi::xpath_variable_set variables;
        
        // Test with variables set to nullptr
        test_select_single_node(root, xpath_query.c_str(), nullptr);
        
        // Test with empty variables set
        test_select_single_node(root, xpath_query.c_str(), &variables);
        
        // Create xpath_query object to test xpath_parse_result
        pugi::xpath_query query_obj(xpath_query.c_str());
        pugi::xpath_parse_result xpath_parse_result = query_obj.result();
        
        // Test pugi::xpath_parse_result::description
        test_xpath_parse_result(xpath_parse_result);
        
        // Test pugi::xpath_exception functions
        test_xpath_exception(xpath_parse_result);
        
        // Test with different nodes in the document
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_select_single_node(child, xpath_query.c_str(), nullptr);
        }
    } else {
        // Even if parsing failed, we can still test xpath_parse_result
        pugi::xpath_parse_result dummy_xpath_result;
        dummy_xpath_result.error = "Fuzzer-generated error";
        dummy_xpath_result.offset = 0;
        
        test_xpath_parse_result(dummy_xpath_result);
        test_xpath_exception(dummy_xpath_result);
    }
    
    // Additional test: create dummy file and load from it
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        pugi::xml_document doc2;
        pugi::xml_parse_result file_result = doc2.load_file(filename);
        test_xml_parse_result(file_result);
        
        // Clean up
        remove(filename);
    }
    
    return 0;
}